/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl/sys_Qt/ZFMainEntry_sys_Qt.h"

int main(int argc, char **argv)
{

    ZFImpl_sys_Qt_requireLib(ZFCore)
    ZFImpl_sys_Qt_requireLib(ZFAlgorithm)
    ZFImpl_sys_Qt_requireLib(ZFUtility)
    ZFImpl_sys_Qt_requireLib(ZFUIKit)
    ZFImpl_sys_Qt_requireLib(ZFAddition)
    ZFImpl_sys_Qt_requireLib(ZFUIWidget)

    ZFImpl_sys_Qt_requireLib(ZF_impl)
    ZFImpl_sys_Qt_requireLib(ZF_loader_impl)
    ZFImpl_sys_Qt_requireLib(ZFCore_impl)
    ZFImpl_sys_Qt_requireLib(ZFAlgorithm_impl)
    ZFImpl_sys_Qt_requireLib(ZFUtility_impl)
    ZFImpl_sys_Qt_requireLib(ZFUIKit_impl)
    ZFImpl_sys_Qt_requireLib(ZFAddition_impl)
    ZFImpl_sys_Qt_requireLib(ZFUIWidget_impl)
    return ZFMainEntry_sys_Qt(argc, argv);
}

