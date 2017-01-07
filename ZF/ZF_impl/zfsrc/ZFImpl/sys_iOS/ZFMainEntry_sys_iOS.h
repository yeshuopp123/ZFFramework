/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFMainEntry_sys_iOS_h_
#define _ZFI_ZFMainEntry_sys_iOS_h_

#include "ZFCore.h"
#include "ZFImpl_sys_iOS_ZF.h"

/*
 * main entry steps:
 * none for iOS, the default template created by XCode would work
 */
#if ZF_ENV_sys_iOS

#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// implementation may use this to access the application
extern ZF_ENV_EXPORT UIApplication *ZFImpl_sys_iOS_application(void);
// implementation may use this to access the root window
extern ZF_ENV_EXPORT UIWindow *ZFImpl_sys_iOS_rootWindow(void);

ZF_NAMESPACE_GLOBAL_END

#endif
#endif // #ifndef _ZFI_ZFMainEntry_sys_iOS_h_

