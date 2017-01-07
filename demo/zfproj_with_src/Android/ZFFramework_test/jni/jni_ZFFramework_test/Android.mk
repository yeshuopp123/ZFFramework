# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

#============================================================
# project settings
#============================================================

# module name
# use System.loadLibrary("module_name"); to load it
ZF_MODULE_NAME          = ZFFramework_test

# root path of ZFFramework
ZF_ROOT_PATH            = ../../../../../ZFFramework

# source directories to search, under project_path,
# set to "jni" to search whole project_path/jni path
# support relative path
# path must not contain spaces
# path must use '/' as path separator
# may contain more than one directories, separated by space
ZF_SRC_DIRS             =
ZF_SRC_DIRS             += ../../../zfsrc

ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFCore/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFAlgorithm/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUtility/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIKit/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFAddition/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIWidget/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIWebKit/zfsrc

ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZF_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZF_loader_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFCore_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFAlgorithm_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUtility_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIKit_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFAddition_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIWidget_impl/zfsrc
ZF_SRC_DIRS             += $(ZF_ROOT_PATH)/ZF/ZFUIWebKit_impl/zfsrc

# whether use unity build
ZF_UNITY_BUILD          = true
ifeq ($(OS),Windows_NT)
ZF_UNITY_BUILD_SCRIPT   = $(ZF_ROOT_PATH)/tools/common/unity_build.bat
else
ZF_UNITY_BUILD_SCRIPT   = $(ZF_ROOT_PATH)/tools/common/unity_build.sh
endif

# source extensions, separated by space
ZF_SRC_EXTS             = c cpp cxx

# true or false
ZF_BUILD_SHARED         = true

# extra include path setting, separated by space
ZF_INCLUDES             =
ZF_INCLUDES             += ../../../zfsrc

ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFCore/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFAlgorithm/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUtility/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIKit/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFAddition/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIWidget/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIWebKit/zfsrc

ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZF_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZF_loader_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFCore_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFAlgorithm_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUtility_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIKit_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFAddition_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIWidget_impl/zfsrc
ZF_INCLUDES             += $(ZF_ROOT_PATH)/ZF/ZFUIWebKit_impl/zfsrc

# compiler flags
ZF_CFLAGS               = -Os

# linker flags
ZF_LFLAGS               = -landroid -llog

# third-party libs to load
ZF_LOAD_STATIC_LIB      =
ZF_LOAD_SHARED_LIB      =
# ZF dependency
# ZF_LOAD_SHARED_LIB      += ZFDependencyLib


#============================================================
# LOCAL_PATH set only once, before any other code
#============================================================
#LOCAL_PATH := $(call my-dir)

#============================================================
# other custom settings
#============================================================
# ZF dependency
# include $(CLEAR_VARS)
# LOCAL_MODULE := ZFDependencyLib
# LOCAL_SRC_FILES := ../$(ZF_ROOT_PATH)/_release/Android/all/libs/$(TARGET_ARCH_ABI)/libZFDependencyLib.so
# include $(PREBUILT_SHARED_LIBRARY)






#============================================================
# no need to change these
#============================================================

#============================================================
# find source files recursively
# usage: $(call zf_find_src_files,$(src_dirs),$(src_exts))
# e.g.:
#   # dirs to search, relative to project_path
#   # set to jni to use whole project_path/jni path
#   SRCDIRS := dir1 dir2
#   # file exts to search
#   SRCEXTS := c cpp
#   # result
#   FILES   := $(call zf_find_src_files, $(SRCDIRS), $(SRCEXTS))
# note:
#   * assume your project is under "c:\project\",
#     with jni lib whose path is "c:\project\jni\some_path\my_lib",
#     then you should cd to "c:\project\",
#     and set SRCDIRS to "jni\some_path\my_lib",
#     then ndk-build
ifeq ($(OS),Windows_NT)
_zf_ls_win = ../$(subst \,/,$(1))$(subst \,/,$(subst $(abspath $(1)),,$(2)))
_zf_ls = $(foreach file,$(shell dir $(subst /,\,$(1)) /a-d /b /s),$(call _zf_ls_win,$(subst \,/,$(1)),$(subst \,/,$(file))))
else
_zf_ls = $(shell find $(1) -type f)
endif
_zf_find_src_files = $(subst \,/,$(filter %.$(2),$(call _zf_ls, $(1))))
zf_find_src_files = $(foreach file,$(foreach srcdir,$(1),$(foreach srcext,$(2),$(call _zf_find_src_files,$(subst \,/,$(srcdir)),$(srcext)))),../$(file))
#============================================================

include $(CLEAR_VARS)

LOCAL_MODULE := $(ZF_MODULE_NAME)
LOCAL_C_INCLUDES := $(ZF_INCLUDES)
ifeq ($(ZF_UNITY_BUILD),true)
zf_unique_name = $(subst __,_,$(subst __,_,$(subst ___,_,$(subst ___,_,$(subst :,_,$(subst .,_,$(subst /,_,$(subst \,_,$(1)))))))))
ifeq ($(OS),Windows_NT)
LOCAL_SRC_FILES := $(foreach src_path,$(ZF_SRC_DIRS),$(subst $(info $(shell call $(subst /,\,$(ZF_UNITY_BUILD_SCRIPT)) $(subst /,\,gen/zfgensrc_$(ZF_MODULE_NAME)_$(call zf_unique_name,$(src_path)).cpp) $(subst /,\,$(src_path)))),,../gen/zfgensrc_$(ZF_MODULE_NAME)_$(call zf_unique_name,$(src_path)).cpp))
else
LOCAL_SRC_FILES := $(foreach src_path,$(ZF_SRC_DIRS),$(subst $(info $(shell sh $(ZF_UNITY_BUILD_SCRIPT) gen/zfgensrc_$(ZF_MODULE_NAME)_$(call zf_unique_name,$(src_path)).cpp $(src_path))),,../gen/zfgensrc_$(ZF_MODULE_NAME)_$(call zf_unique_name,$(src_path)).cpp))
endif
else
LOCAL_SRC_FILES := $(call zf_find_src_files,$(ZF_SRC_DIRS),$(ZF_SRC_EXTS))
endif
LOCAL_CFLAGS := $(ZF_CFLAGS)
LOCAL_LDLIBS := $(ZF_LFLAGS)

#LOCAL_SHORT_COMMANDS = true

LOCAL_STATIC_LIBRARIES := $(ZF_LOAD_STATIC_LIB)
LOCAL_SHARED_LIBRARIES := $(ZF_LOAD_SHARED_LIB)
ifeq ($(ZF_BUILD_SHARED),true)
include $(BUILD_SHARED_LIBRARY)
else
include $(BUILD_STATIC_LIBRARY)
endif
