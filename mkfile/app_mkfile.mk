CLANG ?= clang

KLC_SOURCES ?= $(wildcard *.c)
KLC_OBJECTS = ${KLC_SOURCES:.c=.o}
USR_TARGETS = ${USR_SOURCES:.c=}

CFLAGS := -O2 -target bpf \
          -I $(KLC_DIR)/h \
          -I /usr/include/aarch64-linux-gnu \
          -fno-builtin-memset \
          -fno-builtin-memmove \
          -fno-builtin-memcpy \
          -fno-builtin-strlen

all: $(KLC_OBJECTS) $(USR_TARGETS)

clean:
	rm -f $(KLC_OBJECTS)

$(KLC_OBJECTS): %.o: %.c
	@$(CLANG) $(CFLAGS) -c $<
	@mv *.o $(KLC_DST)

$(USR_TARGETS): %: %.c
	@gcc $< -lbpf -o $@
	@mv $@ $(USR_DST)
