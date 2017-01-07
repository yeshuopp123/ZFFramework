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
    $(info $(shell call $(ZF_ROOT_PATH)\tools\release\release_Android_check.bat))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\common\zfsh_if_exist.bat ..\..\..\zfscript\zfmodule_setup.zfsh Android))

    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_copy.bat ..\..\..\zfres assets\zfres >nul 2>&1))
    $(info $(shell call $(ZF_ROOT_PATH)\tools\spec\Android\res_fix.bat assets\zfres >nul 2>&1))

    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFCore . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAlgorithm . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUtility . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIKit . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAddition . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIWidget . >nul 2>&1))

    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZF_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZF_loader_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFCore_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAlgorithm_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUtility_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIKit_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFAddition_impl . >nul 2>&1))
    $(info $(shell xcopy /s/e/y/r/h $(ZF_ROOT_PATH)\_release\Android\module\ZFUIWidget_impl . >nul 2>&1))
else
    ZF_ROOT_PATH = ../../../../../ZFFramework
    $(info $(shell sh ../../../ZFSetup.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/release/release_Android_check.sh))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/common/zfsh_if_exist.sh ../../../zfscript/zfmodule_setup.zfsh Android))

    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_copy.sh ../../../zfres assets/zfres >/dev/null 2>&1))
    $(info $(shell sh $(ZF_ROOT_PATH)/tools/spec/Android/res_fix.sh assets/zfres >/dev/null 2>&1))

    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFCore/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAlgorithm/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUtility/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIKit/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAddition/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIWidget/. ./ >/dev/null 2>&1))

    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZF_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZF_loader_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFCore_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAlgorithm_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUtility_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIKit_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFAddition_impl/. ./ >/dev/null 2>&1))
    $(info $(shell cp -rf $(ZF_ROOT_PATH)/_release/Android/module/ZFUIWidget_impl/. ./ >/dev/null 2>&1))
endif

include jni/jni_ZF2048/Android.mk

