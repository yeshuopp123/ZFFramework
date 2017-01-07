/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextView.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUITextView ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUITextView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextView ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUITextView)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextViewImpl_sys_Android, ZFUITextView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:TextView"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFString, zfText("Android:String"))
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUITextView).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

public:
    virtual void *nativeTextViewCreate(ZF_IN ZFUITextView *textView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeTextViewCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeTextViewDestroy(ZF_IN ZFUITextView *textView,
                                       ZF_IN void *nativeTextView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeTextViewDestroy"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject nativeTextViewTmp = ZFCastStatic(jobject, nativeTextView);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, nativeTextViewTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTextViewTmp);
    }

    // ============================================================
    // properties
public:
    virtual void textContentSet(ZF_IN ZFUITextView *textView,
                                ZF_IN ZFString *text)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textContentSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jobject, (text == zfnull) ? zfnull : text->nativeString()));
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textAppearanceSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textAppearance));
    }
    virtual void textAlignSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textAlignSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textAlign));
    }
    virtual void textColorSet(ZF_IN ZFUITextView *textView,
                              ZF_IN ZFUIColor const &textColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textColor));
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextView *textView,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textShadowColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textShadowColor));
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUISize const &textShadowOffset)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textShadowOffsetSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textShadowOffset.width),
            ZFCastStatic(jint, textShadowOffset.height));
    }
    virtual void textSizeSet(ZF_IN ZFUITextView *textView,
                             ZF_IN zfint textSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMinSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMinSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSizeAutoChangeMaxSizeSet(ZF_IN ZFUITextView *textView,
                                              ZF_IN zfint textSizeAutoChangeMaxSize)
    {
        // changed during layoutNativeTextView
    }
    virtual void textSingleLineSet(ZF_IN ZFUITextView *textView,
                                   ZF_IN zfbool textSingleLine)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSingleLineSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jboolean, textSingleLine));
    }
    virtual void textTruncateModeSet(ZF_IN ZFUITextView *textView,
                                     ZF_IN ZFUITextTruncateModeEnum const &textTruncateMode)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textTruncateModeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, textTruncateMode));
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextView(ZF_IN ZFUITextView *textView,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_measureNativeTextView"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jobject jobjSize = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            ZFCastStatic(jint, sizeHint.width),
            ZFCastStatic(jint, sizeHint.height),
            ZFCastStatic(jint, textSize));
        JNIBlockedDeleteLocalRef(jobjSize);
        return ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(jobjSize);
    }

    virtual zfint textSizeCurrent(ZF_IN ZFUITextView *textView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSizeCurrent"),
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jint ret = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()));
        return ZFCastStatic(zfint, ret);
    }

    virtual void layoutNativeTextView(ZF_IN ZFUITextView *textView,
                                      ZF_IN const ZFUISize &viewSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSizeAutoChangeCurrentValueSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        zfint fixedTextSize = this->calcTextSizeAutoChange(textView, viewSize);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textView->nativeImplView()),
            (jint)fixedTextSize);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Android

