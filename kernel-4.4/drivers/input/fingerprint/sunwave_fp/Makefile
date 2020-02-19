
# If KERNELRELEASE is defined, we've been invoked from the
# kernel build system and can use its language.
#ifneq ($(KERNELRELEASE),)

#include $(srctree)/drivers/misc/mediatek/Makefile.custom
#ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
#ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
#ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
#3ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
#ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
#ccflags-y += -I$(srctree)/drivers/misc/mediatek/teei/V1.0/tz_vfs
#subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/tkcore/core

#obj-y += sunwave_fp.o
#sunwave_fp-objs          := sf_ctl.o sf_spi.o

# Otherwise we were called directly from the command
# line; invoke the kernel build system.
