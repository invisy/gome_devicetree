#Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
# MediaTek Inc. (C) 2012. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#
# The following software/firmware and/or related documentation ("MediaTek Software")
# have been modified by MediaTek Inc. All revisions are subject to any receiver's
# applicable license agreements with MediaTek Inc.
#
#$(info CUSTOM_KERNEL_ACCELEROMETER = $(CUSTOM_KERNEL_ACCELEROMETER))
LOCAL_PATH := $(call my-dir)

ifeq ($(MTK_SENSORS_1_0), yes)

# HAL module implemenation, not prelinked and stored in
# hw/<SENSORS_HARDWARE_MODULE_ID>.<ro.hardware>.so
include $(CLEAR_VARS)
LOCAL_PRELINK_MODULE := false
#LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_RELATIVE_PATH := hw
LOCAL_SHARED_LIBRARIES := \
    liblog \
    libcutils \
    libutils \
    libhardware \
    libstagefright_foundation \
    libksensor 

LOCAL_REQUIRED_MODULES := \
    libsensor_custom 

ifneq ($(wildcard vendor/mediatek/proprietary/hardware/libsensor),)
$(foreach custom_hal_msensorlib,$(CUSTOM_HAL_MSENSORLIB),$(eval LOCAL_REQUIRED_MODULES += lib$(custom_hal_msensorlib)))
endif

LOCAL_MULTILIB := first 

LOCAL_SRC_FILES := \
    SensorBase.cpp \
    sensors.cpp \
    SensorList.cpp \
    SensorContext.cpp \
    SensorManager.cpp \
    VendorInterface.cpp \
    MtkInterface.cpp \
    SensorCalibration.cpp \
    SensorEventReader.cpp \
    file.cpp \
    JSONObject.cpp \
    SensorSaved.cpp \
    DirectChannelManager.cpp \
    DirectChannel.cpp \
    Acceleration.cpp \
    Magnetic.cpp \
    Gyroscope.cpp \
    AmbienteLight.cpp \
    Proximity.cpp \
    Pressure.cpp \
    StepCounter.cpp \
    Activity.cpp \
    Pedometer.cpp \
    Humidity.cpp \
    Situation.cpp \
    Fusion.cpp \
    Biometric.cpp \
    WakeUpSet.cpp 

LOCAL_C_INCLUDES += \
        $(MTK_PATH_SOURCE)/hardware/sensor/ \
        $(MTK_PATH_CUSTOM)/hal/sensors/sensor \
        $(TOP)/frameworks/av/media/libstagefright/include \
        $(LOCAL_PATH)/include 

LOCAL_EXPORT_C_INCLUDE_DIRS := \
        $(LOCAL_PATH)/include 

ifeq ($(USE_SENSOR_MULTI_HAL), true)
    LOCAL_CFLAGS += -DUSE_SENSOR_MULTI_HAL
endif
ifeq ($(SENSOR_BATCH_SUPPORT), yes)
    LOCAL_CFLAGS += -DSENSOR_BATCH_SUPPORT
endif
ifeq ($(CUSTOM_KERNEL_SENSORHUB), yes)
    LOCAL_CFLAGS += -DSENSORHUB_MAX_INPUT_READER_NBEVENTS
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_SENSORHUB
endif
ifeq ($(CUSTOM_KERNEL_ACCELEROMETER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ACCELEROMETER
endif
ifeq ($(CUSTOM_KERNEL_MAGNETOMETER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_MAGNETOMETER
endif
ifeq ($(CUSTOM_KERNEL_ORIENTATION_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ORIENTATION_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_GYROSCOPE), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_GYROSCOPE
endif
ifeq ($(CUSTOM_KERNEL_ALSPS), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ALSPS
endif
ifeq ($(CUSTOM_KERNEL_ALS), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ALS
endif
ifeq ($(CUSTOM_KERNEL_PS), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_PS
endif
ifeq ($(CUSTOM_KERNEL_BAROMETER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_BAROMETER
endif
ifeq ($(CUSTOM_KERNEL_TEMPURATURE), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_TEMPURATURE
endif
ifeq ($(CUSTOM_KERNEL_GRAVITY_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_GRAVITY_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_LINEARACCEL_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_LINEARACCEL_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_RV_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_RV_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_HUMIDITY), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_HUMIDITY
endif
ifeq ($(CUSTOM_KERNEL_UNCALI_MAG_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_UNCALI_MAG_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_GRV_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_GRV_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_UNCALI_ACC_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_UNCALI_ACC_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_UNCALI_GYRO_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_UNCALI_GYRO_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_SIGNIFICANT_MOTION_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_SIGNIFICANT_MOTION_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_STEP_COUNTER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_STEP_COUNTER
endif
ifeq ($(CUSTOM_KERNEL_FLOOR_COUNTER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_FLOOR_COUNTER
endif
ifeq ($(CUSTOM_KERNEL_GMRV_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_GMRV_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_HEART), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_HEART
endif
ifeq ($(CUSTOM_KERNEL_TILT_DETECTOR_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_TILT_DETECTOR_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_WAKE_GESTURE_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_WAKE_GESTURE_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_GLANCE_GESTURE_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_GLANCE_GESTURE_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_PICK_UP_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_PICK_UP_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_PEDOMETER), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_PEDOMETER
endif
ifeq ($(CUSTOM_KERNEL_ACTIVITY_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ACTIVITY_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_STATIONARY_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_STATIONARY_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_MOTION_DETECT), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_MOTION_DETECT
endif
ifeq ($(CUSTOM_KERNEL_DEVICE_ORIENTATION), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_DEVICE_ORIENTATION
endif
ifeq ($(CUSTOM_KERNEL_IN_POCKET_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_IN_POCKET_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_ANSWER_CALL_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_ANSWER_CALL_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_PDR_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_PDR_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_FACE_DOWN_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_FACE_DOWN_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_SHAKE_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_SHAKE_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_BRINGTOSEE_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_BRINGTOSEE_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_BIOMETRIC_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_BIOMETRIC_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_FLAT_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_FLAT_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_RGBW_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_RGBW_SENSOR
endif
ifeq ($(CUSTOM_KERNEL_SAR_SENSOR), yes)
    LOCAL_CFLAGS += -DCUSTOM_KERNEL_SAR_SENSOR
endif

LOCAL_C_INCLUDES += $(TOP)/$(MTK_PATH_SOURCE)/external/aee/binary/inc
ifeq ($(HAVE_AEE_FEATURE),yes)
    LOCAL_CFLAGS += -DHAVE_AEE_FEATURE
    LOCAL_SHARED_LIBRARIES += libaedv
endif

LOCAL_MODULE := sensors.$(TARGET_BOARD_PLATFORM)
LOCAL_PROPRIETARY_MODULE := true
LOCAL_MODULE_OWNER := mtk
LOCAL_MODULE_TAGS := optional

include $(MTK_SHARED_LIBRARY)
include $(LOCAL_PATH)/algorithm/calibration/Android.mk
endif
