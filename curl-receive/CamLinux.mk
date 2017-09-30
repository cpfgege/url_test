LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:=post.cpp
LOCAL_SHARED_LIBRARIES:= libcurl
LOCAL_MODULE:= post01
include $(BUILD_EXECUTABLE)
