/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_h_
#define _ZFI_ZFImpl_sys_Android_ZFUIKit_h_

#include "ZFImpl.h"

#include "ZFImpl/sys_Android/ZFImpl_sys_Android_ZF.h"
#include "ZFUIKit/ZFUITypeDef.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// Point
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidPoint ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidPoint)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidPoint)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidPoint(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point, ZF_IN_OUT jobject jobjPoint);
// note returned value must be released by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_OUT ZFUIPoint &ret, ZF_IN jobject jobjPoint);
inline ZFUIPoint ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_IN jobject jobjPoint)
{
    ZFUIPoint ret = ZFUIPointZero;
    ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ret, jobjPoint);
    return ret;
}

// ============================================================
// Size
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidSize ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidSize)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidSize)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidSize(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size, ZF_IN_OUT jobject jobjSize);
// note returned value must be released by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_OUT ZFUISize &ret, ZF_IN jobject jobjSize);
inline ZFUISize ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_IN jobject jobjSize)
{
    ZFUISize ret = ZFUISizeZero;
    ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ret, jobjSize);
    return ret;
}

// ============================================================
// Margin
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidMargin ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidMargin)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidMargin)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidMargin(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin, ZF_IN_OUT jobject jobjMargin);
// note returned value must be released by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_OUT ZFUIMargin &ret, ZF_IN jobject jobjMargin);
inline ZFUIMargin ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_IN jobject jobjMargin)
{
    ZFUIMargin ret = ZFUIMarginZero;
    ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ret, jobjMargin);
    return ret;
}

// ============================================================
// Rect
#define ZFImpl_sys_Android_JNI_ID_ZFAndroidRect ZFImpl_sys_Android_JNI_ID(NativeUtil_ZFAndroidRect)
#define ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect ZFImpl_sys_Android_JNI_NAME(NativeUtil.ZFAndroidRect)
extern ZF_ENV_EXPORT jclass ZFImpl_sys_Android_jclassZFAndroidRect(void);
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect, ZF_IN_OUT jobject jobjRect);
// note returned value must be released by DeleteLocalRef
extern ZF_ENV_EXPORT jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect);
extern ZF_ENV_EXPORT void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_OUT ZFUIRect &ret, ZF_IN jobject jobjRect);
inline ZFUIRect ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_IN jobject jobjRect)
{
    ZFUIRect ret = ZFUIRectZero;
    ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ret, jobjRect);
    return ret;
}

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Android
#endif // #ifndef _ZFI_ZFImpl_sys_Android_ZFUIKit_h_

