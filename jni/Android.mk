APP_ALLOW_MISSING_DEPS := true
APP_PLATFORM := android-19
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),arm))
include $(call all-subdir-makefiles)
endif

