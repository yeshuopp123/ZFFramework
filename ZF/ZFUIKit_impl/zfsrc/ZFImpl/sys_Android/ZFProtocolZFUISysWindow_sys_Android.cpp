/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUISysWindow ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUISysWindow)
#define ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUISysWindow)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_Android, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:Activity"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, zfText("Android:View"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUISysWindow).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);

        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).to<ZFUISysWindow *>());

            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeMainWindowCreate"),
                JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                    .add(JNIPointerJNIType)
                ).c_str());
            JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
                JNIConvertZFObjectToJNIType(jniEnv, this->_mainWindow));
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            ZFUISysWindow *mainWindowTmp = this->_mainWindow;
            this->_mainWindow = zfnull;
            zfRelease(mainWindowTmp);
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    virtual void nativeWindowOnRootViewAdd(ZF_IN ZFUISysWindow *window)
    {
        {
            JNIEnv *jniEnv = JNIGetJNIEnv();
            static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeWindowContainerView"),
                JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                    .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                ).c_str());
            jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId, ZFCastStatic(jobject, window->nativeWindow()));
            ZFUIView::nativeViewNotifyBeforeAdd(window->rootView(), tmp);
        }

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeWindowOnRootViewAdd"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, window->nativeWindow()),
            ZFCastStatic(jobject, window->rootView()->nativeView())
            );
    }
    virtual void nativeWindowOnRootViewRemove(ZF_IN ZFUISysWindow *window)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeWindowOnRootViewRemove"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, window->nativeWindow()),
            ZFCastStatic(jobject, window->rootView()->nativeView())
            );

        ZFUIView::nativeViewNotifyAfterRemove(window->rootView());
    }

    virtual ZFUISysWindow *modalWindowShow(ZF_IN ZFUISysWindow *ownerWindow)
    {
        ZFUISysWindow *modalWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance(ZF_CALLER_FILE, ZF_CALLER_FUNCTION, ZF_CALLER_LINE).to<ZFUISysWindow *>());

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_modalWindowShow"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, ownerWindow->nativeWindow()),
            JNIConvertZFObjectToJNIType(jniEnv, modalWindow));

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *ownerWindow,
                                   ZF_IN ZFUISysWindow *windowToFinish)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_modalWindowFinish"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, windowToFinish->nativeWindow()));
    }

    virtual void updateSuggestedWindowLayoutParam(ZF_IN ZFUISysWindow *window)
    {
        // default is fill parent, nothing to do
    }
    virtual void windowLayoutParamOnChange(ZF_IN ZFUISysWindow *window)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_windowLayoutParamOnChange"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, window->nativeWindow()));
    }

    virtual ZFUIOrientationEnum windowOrientation(ZF_IN ZFUISysWindow *window)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_windowOrientation"),
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, window->nativeWindow()));
        return ZFCastStatic(ZFUIOrientationEnum, ret);
    }
    virtual void windowOrientationFlagsSet(ZF_IN ZFUISysWindow *window,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_windowOrientationFlagsSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, window->nativeWindow()),
            (jint)flags);
    }

private:
    jclass jclsOwner;
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyMeasureWindow,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow,
                   jint refWidth,
                   jint refHeight,
                   jobject resultRect)
{
    ZFUIRect result = ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyMeasureWindow(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)),
        ZFUIRectMake(0, 0, refWidth, refHeight));
    ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(result, resultRect);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyOnCreate,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow,
                   jobject nativeWindow)
{
    jobject nativeWindowGlobalRef = JNIUtilNewGlobalRef(jniEnv, nativeWindow);
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnCreate(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)),
        ZFCastStatic(void *, nativeWindowGlobalRef));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyOnDestroy,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFUISysWindow *window = ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow));
    jobject nativeWindowGlobalRef = ZFCastStatic(jobject, window->nativeWindow());
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnDestroy(window);
    JNIUtilDeleteGlobalRef(jniEnv, nativeWindowGlobalRef);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyOnResume,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnResume(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyOnPause,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnPause(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUISysWindow, native_1notifyOnRotate,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUISysWindow)
{
    ZFPROTOCOL_ACCESS(ZFUISysWindow)->notifyOnRotate(
        ZFCastZFObject(ZFUISysWindow *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUISysWindow)));
}

#endif // #if ZF_ENV_sys_Android

