modname ?=
arch ?=

ifeq ($(arch),arm)

kerneldir := /home/linux/linux-5.10.61/
else
kerneldir := /lib/modules/$(shell uname -r)/build
endif

PWD := $(shell pwd)


all:
	make -C $(kerneldir) M=$(PWD) modules

clean:
	make -C $(kerneldir) M=$(PWD) clean

obj-m := $(modname).o
