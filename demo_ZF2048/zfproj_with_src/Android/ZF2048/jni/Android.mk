# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

LOCAL_PATH := $(call my-dir)

ifeq ($(OS),Windows_NT)
    ZF_ROOT_PATH = ..\..\..\..\..\ZFFramework
    $(info $(shell call ..\..\..\ZFSetup.bat))
    $(info $(shell call $(ZF_ROOT_PATH)\zf3rd_setup.bat))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\common\zfsh_if_exist.bat ..\..\..\zfscript\zfmodule_setup.zfsh Android))

    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat ..\..\..\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_fix.bat assets\zfres >nul 2>&1))

    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFCore\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFAlgorithm\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUtility\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUIKit\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFAddition\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUIWidget\zfres assets\zfres >nul 2>&1))

    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZF_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZF_loader_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFCore_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFAlgorithm_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUtility_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUIKit_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFAddition_impl\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat $(ZF_ROOT_PATH)\ZF\ZFUIWidget_impl\zfres assets\zfres >nul 2>&1))
else
    ZF_ROOT_PATH = ../../../../../ZFFramework
    $(info $(shell sh ../../../ZFSetup.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/zf3rd_setup.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/common/zfsh_if_exist.sh ../../../zfscript/zfmodule_setup.zfsh Android))

    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh ../../../zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_fix.sh assets/zfres >/dev/null 2>&1))

    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFCore/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFAlgorithm/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUtility/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUIKit/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFAddition/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUIWidget/zfres assets/zfres >/dev/null 2>&1))

    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZF_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZF_loader_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFCore_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFAlgorithm_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUtility_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUIKit_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFAddition_impl/zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh $(ZF_ROOT_PATH)/ZF/ZFUIWidget_impl/zfres assets/zfres >/dev/null 2>&1))
endif

include jni/jni_ZF2048/Android.mk

