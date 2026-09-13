/******************************************************************************
* Copyright (C), Xingang.Li
* Author: Xingang.Li  Version: 1.0
* Date: 2026-09-12
* Description: 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "bpf/bpf.h"
#include "luaxdp_def.h"

static void _luaxdp_help(void)
{
    printf("Usage: luaxdp lua-file\r\n");
}

static char * _luaxdp_read_file(const char* filename, long *out_size)
{
    FILE* fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }

    
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        return NULL;
    }

    
    long size = ftell(fp);
    if (size < 0) {
        fclose(fp);
        return NULL;
    }

    
    rewind(fp);

    
    char* buffer = (char*)malloc(size + 1);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    
    size_t read_size = fread(buffer, 1, size, fp);
    buffer[read_size] = '\0';  

    fclose(fp);

    if (out_size) *out_size = (long)read_size;

    return buffer;
}

int main(int argc, char **argv)
{
    int fd;
    char *instance = NULL;
    char *file = argv[1];
    LUAXDP_CODE_S cfg = {0};
    void *content;
    long file_size;

    if (argc < 2) {
        _luaxdp_help();
        return -1;
    }

    fd = bpf_obj_get("/sys/fs/bpf/xdp/globals/g_luaxdp_code_cfg");
    if (fd < 0) {
        fprintf(stderr, "Can't open map\n");
        return -1;
    }

    content = _luaxdp_read_file(file, &file_size);
    if (! content) {
        fprintf(stderr, "Can't read lua-file\n");
        return -1;
    }

    if (file_size > sizeof(cfg.lua_code)) {
        fprintf(stderr, "File size exceed\n");
        return -1;
    }

    memcpy(cfg.lua_code, content, file_size);
    cfg.valid = 1;

    int key = 0;
    bpf_map_update_elem(fd, &key, &cfg, BPF_ANY);

    return 0;
}

