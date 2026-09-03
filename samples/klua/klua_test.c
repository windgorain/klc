/*********************************************************
*   Copyright (C) LiXingang
*
********************************************************/
#include "utl/ulc_user.h"

static int _klua_test(void)
{
    void *L = (void*)ulc_namefunc_run(0, "luaL_newstate");
    if (! L) {
        return -1;
    }

    ulc_namefunc_run(0, "luaL_openlibs", L);

    const char *s = "print(\'Hello by klua!\')";

    if (ulc_namefunc_run(-1, "luaL_dostring", L, s) != 0) {  
        const char *message = (void*)ulc_namefunc_run((long)"", "lua_tostring", L, -1);  
        printf("Error executing Lua script: %s\n", message);  
        ulc_namefunc_run(0, "lua_pop", L, 1);  
    }
  
    ulc_namefunc_run(0, "lua_close", L);

    return 0;
}

SEC(".spf.cmd/main")
int main()
{
    _klua_test();
    return 0;
}
