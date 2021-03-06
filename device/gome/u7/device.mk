

# PRODUCT_COPY_FILES += $(LOCAL_PATH)/egl.cfg:$(TARGET_COPY_OUT_VENDOR)/lib/egl/egl.cfg:mtk
# PRODUCT_COPY_FILES += $(LOCAL_PATH)/ueventd.mt6757.rc:root/ueventd.mt6757.rc

PRODUCT_COPY_FILES += $(LOCAL_PATH)/factory_init.project.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/hw/factory_init.project.rc
PRODUCT_COPY_FILES += $(LOCAL_PATH)/init.project.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/hw/init.project.rc
PRODUCT_COPY_FILES += $(LOCAL_PATH)/meta_init.project.rc:$(TARGET_COPY_OUT_VENDOR)/etc/init/hw/meta_init.project.rc

ifeq ($(MTK_SMARTBOOK_SUPPORT),yes)
PRODUCT_COPY_FILES += $(LOCAL_PATH)/sbk-kpd.kl:system/usr/keylayout/sbk-kpd.kl:mtk \
                      $(LOCAL_PATH)/sbk-kpd.kcm:system/usr/keychars/sbk-kpd.kcm:mtk
endif

# Add FlashTool needed files
#PRODUCT_COPY_FILES += $(LOCAL_PATH)/EBR1:EBR1
#ifneq ($(wildcard $(LOCAL_PATH)/EBR2),)
#  PRODUCT_COPY_FILES += $(LOCAL_PATH)/EBR2:EBR2
#endif
#PRODUCT_COPY_FILES += $(LOCAL_PATH)/MBR:MBR
#PRODUCT_COPY_FILES += $(LOCAL_PATH)/MT6757_Android_scatter.txt:MT6757_Android_scatter.txt

PRODUCT_PROPERTY_OVERRIDES += ro.sf.lcd_density=480

ifeq ($(TARGET_BUILD_VARIANT),eng)
    PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.eng.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/thermal.conf:mtk
else
    PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/thermal.conf:mtk
endif
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.wfd.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_00:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.pip.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_01:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.fdvrgis.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_02:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.VSDOF.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_03:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.denoise.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_04:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.meta.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_meta.conf:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.SW4KVP.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_05:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.SW4KVR.6757.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/.thermal_policy_06:mtk

PRODUCT_COPY_FILES += $(LOCAL_PATH)/thermal.off.conf:$(TARGET_COPY_OUT_VENDOR)/etc/.tp/thermal.off.conf:mtk






# alps/vendor/mediatek/proprietary/external/GeoCoding/Android.mk

# alps/vendor/mediatek/proprietary/frameworks-ext/native/etc/Android.mk

# touch related file for CTS
ifeq ($(strip $(CUSTOM_KERNEL_TOUCHPANEL)),generic)
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.touchscreen.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.touchscreen.xml
else
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.faketouch.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.faketouch.xml
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.touchscreen.multitouch.distinct.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.touchscreen.multitouch.distinct.xml
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.touchscreen.multitouch.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.touchscreen.multitouch.xml
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.touchscreen.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.touchscreen.xml
endif

# USB OTG
PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.usb.host.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.usb.host.xml

# GPS relative file
ifeq ($(MTK_GPS_SUPPORT),yes)
  PRODUCT_COPY_FILES += frameworks/native/data/etc/android.hardware.location.gps.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.location.gps.xml
endif

# alps/external/libnfc-opennfc/open_nfc/hardware/libhardware/modules/nfcc/nfc_hal_microread/Android.mk
# PRODUCT_COPY_FILES += external/libnfc-opennfc/open_nfc/hardware/libhardware/modules/nfcc/nfc_hal_microread/driver/open_nfc_driver.ko:$(TARGET_COPY_OUT_VENDOR)/lib/open_nfc_driver.ko:mtk

# alps/frameworks/av/media/libeffects/factory/Android.mk
PRODUCT_COPY_FILES += frameworks/av/media/libeffects/data/audio_effects.conf:system/etc/audio_effects.conf

# alps/mediatek/config/$project
PRODUCT_COPY_FILES += $(LOCAL_PATH)/android.hardware.telephony.gsm.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.telephony.gsm.xml

# Set default USB interface

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += persist.service.acm.enable=0
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += ro.mount.fs=EXT4


# meta tool
PRODUCT_PROPERTY_OVERRIDES += ro.mediatek.chip_ver=S01
PRODUCT_PROPERTY_OVERRIDES += ro.mediatek.platform=MT6757

# set Telephony property - SIM count
SIM_COUNT := 2
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += ro.telephony.sim.count=$(SIM_COUNT)
PRODUCT_SYSTEM_DEFAULT_PROPERTIES += persist.radio.default.sim=0

# Audio Related Resource
PRODUCT_COPY_FILES += vendor/mediatek/proprietary/custom/u7/factory/res/sound/testpattern1.wav:$(TARGET_COPY_OUT_VENDOR)/res/sound/testpattern1.wav:mtk
PRODUCT_COPY_FILES += vendor/mediatek/proprietary/custom/u7/factory/res/sound/ringtone.wav:$(TARGET_COPY_OUT_VENDOR)/res/sound/ringtone.wav:mtk

# Keyboard layout
PRODUCT_COPY_FILES += device/mediatek/mt6757/ACCDET.kl:system/usr/keylayout/ACCDET.kl:mtk
PRODUCT_COPY_FILES += $(LOCAL_PATH)/mtk-kpd.kl:system/usr/keylayout/mtk-kpd.kl:mtk

# Microphone
PRODUCT_COPY_FILES += $(LOCAL_PATH)/android.hardware.microphone.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.microphone.xml

# Camera
PRODUCT_COPY_FILES += $(LOCAL_PATH)/android.hardware.camera.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.camera.xml

# Audio Policy
PRODUCT_COPY_FILES += $(LOCAL_PATH)/audio_policy.conf:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy.conf:mtk


#Images for LCD test in factory mode
PRODUCT_COPY_FILES += vendor/mediatek/proprietary/custom/u7/factory/res/images/lcd_test_00.png:$(TARGET_COPY_OUT_VENDOR)/res/images/lcd_test_00.png:mtk
PRODUCT_COPY_FILES += vendor/mediatek/proprietary/custom/u7/factory/res/images/lcd_test_01.png:$(TARGET_COPY_OUT_VENDOR)/res/images/lcd_test_01.png:mtk
PRODUCT_COPY_FILES += vendor/mediatek/proprietary/custom/u7/factory/res/images/lcd_test_02.png:$(TARGET_COPY_OUT_VENDOR)/res/images/lcd_test_02.png:mtk


# overlay has priorities. high <-> low.

DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/sd_in_ex_otg

DEVICE_PACKAGE_OVERLAYS += $(LOCAL_PATH)/overlay
ifdef OPTR_SPEC_SEG_DEF
  ifneq ($(strip $(OPTR_SPEC_SEG_DEF)),NONE)
    OPTR := $(word 1,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    SPEC := $(word 2,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    SEG  := $(word 3,$(subst _,$(space),$(OPTR_SPEC_SEG_DEF)))
    DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/operator/$(OPTR)/$(SPEC)/$(SEG)
  endif
endif
ifneq (yes,$(strip $(MTK_TABLET_PLATFORM)))
  ifeq (480,$(strip $(LCM_WIDTH)))
    ifeq (854,$(strip $(LCM_HEIGHT)))
      DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/FWVGA
    endif
  endif
  ifeq (540,$(strip $(LCM_WIDTH)))
    ifeq (960,$(strip $(LCM_HEIGHT)))
      DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/qHD
    endif
  endif
endif
ifeq (yes,$(strip $(MTK_GMO_ROM_OPTIMIZE)))
  DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/slim_rom
endif
ifeq (yes,$(strip $(MTK_GMO_RAM_OPTIMIZE)))
  DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/slim_ram
endif
DEVICE_PACKAGE_OVERLAYS += device/mediatek/common/overlay/navbar

ifeq ($(strip $(OPTR_SPEC_SEG_DEF)),NONE)
    PRODUCT_PACKAGES += DangerDash
endif

$(call inherit-product, device/mediatek/mt6757/device.mk)

$(call inherit-product-if-exists, vendor/mediatek/libs/$(MTK_TARGET_PROJECT)/device-vendor.mk)

# setup dm-verity configs.
PRODUCT_SYSTEM_VERITY_PARTITION := /dev/block/platform/mtk-msdc.0/11230000.msdc0/by-name/system
$(call inherit-product, build/target/product/verity.mk)

# Add msensor lib

PRODUCT_COPY_FILES +=\
	$(call find-copy-subdir-files,*,$(LOCAL_PATH)/libcalmodule_qmcX983,vendor)

# Start NFC

DEVICE_MANIFEST_FILE += device/gome/u7/project_manifest/manifest_nfc.xml

# These are the hardware-specific features
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.nfc.hce.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.nfc.hce.xml \
    frameworks/native/data/etc/android.hardware.nfc.hcef.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.nfc.hcef.xml \
    frameworks/native/data/etc/android.hardware.nfc.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/android.hardware.nfc.xml \
    frameworks/native/data/etc/com.nxp.mifare.xml:$(TARGET_COPY_OUT_VENDOR)/etc/permissions/com.nxp.mifare.xml

# nfc packages
PRODUCT_PACKAGES += \
	libnfc \
    com.android.nfc_extras \
    NfcNci \
    Tag	\
    android.hardware.nfc@1.0-impl \
	android.hardware.nfc@1.0-service
	
# NFC config files
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/nfc/etc/libnfc-nci.conf:$(TARGET_COPY_OUT_VENDOR)/etc/libnfc-nci.conf \
    $(LOCAL_PATH)/nfc/etc/libnfc-nci-20797b00.conf:$(TARGET_COPY_OUT_VENDOR)/etc/libnfc-nci-20797b00.conf \
    $(LOCAL_PATH)/nfc/firmware/BCM20797B0_002.001.043.0005.0009_Generic_NCD_Unsigned_configdata.ncd:$(TARGET_COPY_OUT_VENDOR)/firmware/BCM20797B0_002.001.043.0005.0009_Generic_NCD_Unsigned_configdata.ncd \
    $(LOCAL_PATH)/nfc/firmware/Generic_unsigned_512_3.ncd:$(TARGET_COPY_OUT_VENDOR)/firmware/Generic_unsigned_512_3.ncd

# NFC Init Files
PRODUCT_COPY_FILES += \
     $(LOCAL_PATH)/nfc/etc/init/init.bcm2079x.nfc.rc:vendor/etc/init/init.bcm2079x.nfc.rc

# NFC stock HAL
PRODUCT_COPY_FILES += \
     $(LOCAL_PATH)/nfc/nfc_nci.bcm2079x.default.so:$(TARGET_COPY_OUT_VENDOR)/lib64/hw/nfc_nci.bcm2079x.default.so \

# End NFC