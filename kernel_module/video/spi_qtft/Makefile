# Makefile2.6

ifneq ($(KERNELRELEASE),)
	# kbuild syntax. dependency relationshsip of files and target modules are listed here.
	PWD  := $(shell pwd)
	KDIR := ~/project/git/rpi_linux

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules  ARCH=arm  CROSS_COMPILE=arm-bcm2708-linux-gnueabi-
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions *.order *.symvers

.PHONY: all clean

else
	# 指定驱动模块的名字
	MODULE_NAME := spi_qtft_dev_module
	# 指定驱动模块的核心文件（有init 和 exit）
	RESMAIN_CORE_OBJS := spi_qtft_dev.o
	# 依赖文件
	RESMAIN_GLUE_OBJS := ops.o
	# xxx-objs := 指定驱动模块的所有依赖文件
	$(MODULE_NAME)-objs := $(RESMAIN_GLUE_OBJS) $(RESMAIN_CORE_OBJS)

	obj-m := spi_qtft_dev_module.o

endif
