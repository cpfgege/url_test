LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=get.cpp
LOCAL_SHARED_LIBRARIES:= libcurl
LOCAL_MODULE:= get
include $(BUILD_EXECUTABLE)
