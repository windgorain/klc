KLC_DIR = $(PWD)/..

BASE_DIR = $(KLC_DIR)/../..

ccflags-y := -I$(BASE_DIR)/h -I$(KLC_DIR)/h -I$(KLC_DIR)/ko -O2

obj-m := klc_impl.o klc_base.o klc_oscomp.o

klc_base-objs := base/klcko_base.o

klc_oscomp-objs := oscomp/klcko_oscomp_init.o \
                   oscomp/klcko_nl.o \
                   oscomp/klcko_inline.o

klc_impl-objs := impl/klcko_impl_init.o \
                   impl/klcko_lib.o \
                   impl/klcko_func.o \
                   impl/klcko_config.o \
                   impl/klcko_bpf_helper.o \
                   impl/klcko_loader.o \
                   impl/klcko_bare.o \
                   impl/klcko_kv.o \
                   impl/klcko_setjmp.o

CROSS_COMPILE=''
KDIR := /lib/modules/$(shell uname -r)/build

all:  
	make -C $(KDIR) M=$(PWD) modules
	strip -S *.ko

clean:  
	rm -f *.ko *.o *.mod.o *.mod.c .*.cmd *.symvers  modul* base/*.o  oscomp/*.o impl/*.o utl/*.o

