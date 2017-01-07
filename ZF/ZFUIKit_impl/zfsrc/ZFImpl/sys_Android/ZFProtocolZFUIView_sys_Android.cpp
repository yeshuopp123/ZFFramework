/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIKeyboardState.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUIView ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUIView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIView ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUIView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewImpl_sys_Android, ZFUIView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:View"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIView).c_str());
        this->jclsZFUIView = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsZFUIView);
        zfsuper::protocolOnDealloc();
    }

    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        ZFUIKeyboardStateBuiltinImplRegister();
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        ZFUIKeyboardStateBuiltinImplUnregister();
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual void *nativeViewCreate(ZF_IN ZFUIView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_nativeViewCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, view));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeViewDestroy(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeView)
    {
        // for performance, we won't have JNI call to destroy, simply delete the global ref

        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeViewTmp = ZFCastStatic(jobject, nativeView);
        JNIUtilDeleteGlobalRef(jniEnv, nativeViewTmp);
    }

    virtual void nativeImplViewSet(ZF_IN ZFUIView *view,
                                   ZF_IN void *nativeImplViewOld,
                                   ZF_IN void *nativeImplView,
                                   ZF_IN zfindex virtualIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_nativeImplViewSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            ZFCastStatic(jobject, nativeImplView),
            (jint)virtualIndex);
    }
    virtual zffloat nativeViewScaleForImpl(ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_nativeViewScaleForImpl"),
            JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView));
    }
    virtual zffloat nativeViewScaleForPhysicalPixel(ZF_IN void *nativeView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_nativeViewScaleForPhysicalPixel"),
            JNIGetMethodSig(JNIType::S_float, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticFloatMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView));
    }

    // ============================================================
    // properties
public:
    virtual void viewVisibleSet(ZF_IN ZFUIView *view,
                                ZF_IN zfbool viewVisible)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewVisibleSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewVisible);
    }
    virtual void viewAlphaSet(ZF_IN ZFUIView *view,
                              ZF_IN zffloat viewAlpha)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewAlphaSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_float)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jfloat)viewAlpha);
    }
    virtual void viewUIEnableSet(ZF_IN ZFUIView *view,
                                 ZF_IN zfbool viewUIEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewUIEnableSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnable);
    }
    virtual void viewUIEnableTreeSet(ZF_IN ZFUIView *view,
                                     ZF_IN zfbool viewUIEnableTree)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewUIEnableTreeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            (jboolean)viewUIEnableTree);
    }
    virtual void viewBackgroundColorSet(ZF_IN ZFUIView *view,
                                        ZF_IN const ZFUIColor &viewBackgroundColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewBackgroundColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            // must ensure it is ARGB format in Color of Java code
            (jint)viewBackgroundColor.colorValue());
    }

public:
    virtual void childAdd(ZF_IN ZFUIView *parent,
                          ZF_IN ZFUIView *child,
                          ZF_IN zfindex virtualIndex,
                          ZF_IN ZFUIViewChildLayerEnum childLayer,
                          ZF_IN zfindex childLayerIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_childAdd"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, parent->nativeView()),
            ZFCastStatic(jobject, child->nativeView()),
            (jint)virtualIndex,
            (jint)childLayer,
            (jint)childLayerIndex);
    }
    virtual void childRemove(ZF_IN ZFUIView *parent,
                             ZF_IN ZFUIView *child,
                             ZF_IN zfindex virtualIndex,
                             ZF_IN ZFUIViewChildLayerEnum childLayer,
                             ZF_IN zfindex childLayerIndex)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_childRemove"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, parent->nativeView()),
            (jint)virtualIndex,
            (jint)childLayer,
            (jint)childLayerIndex);
    }

public:
    virtual void viewFrameSet(ZF_IN ZFUIView *view,
                              ZF_IN const ZFUIRect &rect)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_viewFrameSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()),
            rect.point.x,
            rect.point.y,
            rect.size.width,
            rect.size.height);
    }

    virtual void layoutRequest(ZF_IN ZFUIView *view)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_layoutRequest"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, view->nativeView()));
    }

    virtual void measureNativeView(ZF_OUT ZFUISize &ret,
                                   ZF_IN void *nativeView,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsZFUIView, zfTextA("native_measureNativeView"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jobject jobjSize = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsZFUIView, jmId,
            ZFCastStatic(jobject, nativeView),
            (jint)sizeHint.width,
            (jint)sizeHint.height);
        JNIBlockedDeleteLocalRef(jobjSize);
        ret = ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(jobjSize);
    }

private:
    jclass jclsZFUIView;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// native methods for ZFUIView
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1notifyNeedLayout,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyNeedLayout(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1notifyLayoutRootView,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jobject jobjRect)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutRootView(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(jobjRect));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1notifyLayoutNativeImplView,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jobject jobjRect)
{
    ZFUIRect nativeImplViewRect;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyLayoutNativeImplView(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        nativeImplViewRect);
    ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(nativeImplViewRect, jobjRect);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1notifyUIEvent_1mouse,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint mouseId,
                   jint mouseAction,
                   jint mousePointX,
                   jint mousePointY)
{
    ZFCACHEABLE_ACCESS(ZFUIMouseEvent, ZFUIMouseEvent, event);
    event->mouseId = mouseId;
    event->mouseAction = (ZFUIMouseActionEnum)mouseAction;
    event->mousePoint = ZFUIPointMake(mousePointX, mousePointY);
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
}

JNI_METHOD_DECLARE(jboolean, ZFImpl_sys_Android_JNI_ID_ZFUIView, native_1notifyUIEvent_1key,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIView,
                   jint keyId,
                   jint keyAction,
                   jint keyCode,
                   jint keyCodeRaw)
{
    ZFCACHEABLE_ACCESS(ZFUIKeyEvent, ZFUIKeyEvent, event);
    event->keyId = keyId;
    event->keyAction = (ZFUIKeyActionEnum)keyAction;
    event->keyCode = (ZFUIKeyCodeEnum)keyCode;
    event->keyCodeRaw = keyCodeRaw;
    ZFPROTOCOL_ACCESS(ZFUIView)->notifyUIEvent(
        ZFCastZFObject(ZFUIView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIView)),
        event);
    return event->eventResolved();
}

#endif // #if ZF_ENV_sys_Android

