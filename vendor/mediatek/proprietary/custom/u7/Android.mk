LOCAL_PATH:= $(call my-dir)
ifeq ($(MTK_PROJECT), u7)
include $(call all-makefiles-under,$(LOCAL_PATH))
endif
