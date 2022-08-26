QCDG_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_PATH := $(QCDG_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../include
LOCAL_MODULE := qcdg
LOCAL_SRC_FILES := main.c
include $(BUILD_EXECUTABLE)
