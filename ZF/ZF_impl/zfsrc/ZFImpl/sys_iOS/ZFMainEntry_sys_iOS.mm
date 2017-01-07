/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_iOS_ZF.h"
#include "ZFMainEntry_sys_iOS.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFArray.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

static UIApplication *_ZFP_ZFImpl_sys_iOS_application = nil;
static UIWindow *_ZFP_ZFImpl_sys_iOS_rootWindow = nil;

UIApplication *ZFImpl_sys_iOS_application(void)
{
    return _ZFP_ZFImpl_sys_iOS_application;
}
UIWindow *ZFImpl_sys_iOS_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_iOS_rootWindow;
}

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// app entry
@interface _ZFP_ZFImpl_sys_iOS_AppEventHolder : NSObject
@end
@implementation _ZFP_ZFImpl_sys_iOS_AppEventHolder
- (void)appOnCreate
{
    _ZFP_ZFImpl_sys_iOS_application = [UIApplication sharedApplication];
    _ZFP_ZFImpl_sys_iOS_rootWindow = _ZFP_ZFImpl_sys_iOS_application.keyWindow;

    ZFFrameworkInit();
    ZFMainExecute();

#if ZF_ENV_DEBUG && 0 // framework's init and dealloc
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        NSLog(@"test cleanup");
        ZFFrameworkCleanup();
        NSLog(@"test cleanup end");
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            NSLog(@"test re-init");
            ZFFrameworkInit();
            NSLog(@"test re-init end");
            ZFMainExecute();
            NSLog(@"test main re-enter end");
        });
    });
#endif
}
- (void)appOnDestroy
{
    ZFFrameworkCleanup();

    _ZFP_ZFImpl_sys_iOS_application = nil;
    _ZFP_ZFImpl_sys_iOS_rootWindow = nil;
}
- (void)appOnPause
{
    [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillDisappear:NO];
}
- (void)appOnResume
{
    [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillAppear:NO];
}
- (void)appOnReceiveMemoryWarning
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnReceiveMemoryWarning());
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZF_STATIC_INITIALIZER_INIT(_ZFP_ZFImpl_sys_iOS_AppEventHolder)
{
    appEventHolder = [_ZFP_ZFImpl_sys_iOS_AppEventHolder new];

    [[NSNotificationCenter defaultCenter] addObserver:appEventHolder selector:@selector(appOnCreate) name:UIApplicationDidFinishLaunchingNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:appEventHolder selector:@selector(appOnDestroy) name:UIApplicationWillTerminateNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:appEventHolder selector:@selector(appOnPause) name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:appEventHolder selector:@selector(appOnResume) name:UIApplicationWillEnterForegroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:appEventHolder selector:@selector(appOnReceiveMemoryWarning) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
}
ZF_STATIC_INITIALIZER_DESTROY(_ZFP_ZFImpl_sys_iOS_AppEventHolder)
{
    [[NSNotificationCenter defaultCenter] removeObserver:appEventHolder];
    appEventHolder = nil;
}
_ZFP_ZFImpl_sys_iOS_AppEventHolder *appEventHolder;
ZF_STATIC_INITIALIZER_END(_ZFP_ZFImpl_sys_iOS_AppEventHolder)

ZF_NAMESPACE_GLOBAL_END

#endif

