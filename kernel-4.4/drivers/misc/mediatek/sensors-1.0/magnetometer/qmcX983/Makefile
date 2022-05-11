ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/magnetometer/inc
ccflags-y += -I$(srctree)/drivers/misc/mediatek/sensors-1.0/hwmon/include
obj-y	:=  qmcX983.o

ifeq ($(CONFIG_MTK_PLATFORM), "mt6739")
ccflags-y += -DCONFIG_MSENSOR_FOR_MT6739
endif
