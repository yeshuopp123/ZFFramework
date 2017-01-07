/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIScrollView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIScrollView ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIScrollView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIScrollView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIScrollViewImpl_sys_Android, ZFUIScrollView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:View"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIScrollView).c_str());
        this->jclsZFUIScrollView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIScrollView);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_nativeScrollViewCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, view));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *view,
                                         ZF_IN void *nativeView)
    {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = ZFCastStatic(jobject, nativeView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

public:
    virtual void scrollViewScrollEnableSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollViewScrollEnableSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jboolean)scrollEnable);
    }
    virtual void scrollViewScrollBounceSet(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN zfbool scrollBounceHorizontal,
                                           ZF_IN zfbool scrollBounceVertical,
                                           ZF_IN zfbool scrollBounceHorizontalAlways,
                                           ZF_IN zfbool scrollBounceVerticalAlways)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollViewScrollBounceSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jboolean)scrollBounceHorizontal,
            (jboolean)scrollBounceVertical,
            (jboolean)scrollBounceHorizontalAlways,
            (jboolean)scrollBounceVerticalAlways);
    }
    virtual void scrollViewScrollContentFrameSet(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN const ZFUIRect &frame)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollViewScrollContentFrameSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()),
            (jint)frame.point.x,
            (jint)frame.point.y,
            (jint)frame.size.width,
            (jint)frame.size.height);
    }
    virtual zftimet scrollViewScrollAnimationStart(ZF_IN ZFUIScrollView *scrollView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollViewScrollAnimationStart"),
            JNIGetMethodSig(JNIType::S_long, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return (zftimet)JNIUtilCallStaticLongMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()));
    }
    virtual void scrollViewScrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollViewScrollAnimationStop"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, scrollView->nativeImplView()));
    }

public:
    virtual void scrollChildAdd(ZF_IN ZFUIScrollView *parent,
                                ZF_IN ZFUIView *child,
                                ZF_IN zfindex atIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollChildAdd"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, parent->nativeImplView()),
            ZFCastStatic(jobject, child->nativeView()),
            (jint)atIndex);
    }
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIScrollView, zfTextA("native_scrollChildRemove"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIScrollView, jmId,
            ZFCastStatic(jobject, parent->nativeImplView()),
            (jint)atIndex);
    }

private:
    jclass jclsZFUIScrollView;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIScrollViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIScrollViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIScrollView
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1notifyScrollViewDragBegin,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIScrollView,
                   jint mousePosX,
                   jint mousePosY,
                   jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragBegin(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1notifyScrollViewDrag,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIScrollView,
                   jint mousePosX,
                   jint mousePosY,
                   jlong mouseTime)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDrag(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        ZFUIPointMake(mousePosX, mousePosY), (zftimet)mouseTime);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1notifyScrollViewDragEnd,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIScrollView,
                   jlong mouseTime,
                   jboolean needScrollAni)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragEnd(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        (zftimet)mouseTime,
        (zfbool)needScrollAni);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIScrollView, native_1notifyScrollViewScrollAnimation,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIScrollView,
                   jlong relativeTimeInMiliseconds)
{
    ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewScrollAnimation(
        ZFCastZFObject(ZFUIScrollView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIScrollView)),
        (zftimet)relativeTimeInMiliseconds);
}

#endif // #if ZF_ENV_sys_Android

