/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"
#include "ZFUIKit/protocol/ZFProtocolZFUITextEdit.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFUITextEdit ZFImpl_sys_Android_JNI_ID(ZFUIKit_ZFUITextEdit)
#define ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit ZFImpl_sys_Android_JNI_NAME(ZFUIKit.ZFUITextEdit)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUITextEditImpl_sys_Android, ZFUITextEdit, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:EditText"))
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

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUITextEdit).c_str());
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
    virtual void *nativeTextEditCreate(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeTextEditCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, textEdit));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeTextEditDestroy(ZF_IN ZFUITextEdit *textEdit,
                                       ZF_IN void *nativeTextEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeTextEditDestroy"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject nativeTextEditTmp = ZFCastStatic(jobject, nativeTextEdit);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, nativeTextEditTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTextEditTmp);
    }

    // ============================================================
    // properties
public:
    virtual void textEditEnableSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN zfbool textEditEnable)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditEnableSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jboolean)textEditEnable);
    }
    virtual void textEditSecuredSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN zfbool textEditSecured)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditSecuredSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jboolean)textEditSecured);
    }
    virtual void textEditKeyboardTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                         ZF_IN ZFUITextEditKeyboardTypeEnum textEditKeyboardType)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditKeyboardTypeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textEditKeyboardType);
    }
    virtual void textEditKeyboardReturnTypeSet(ZF_IN ZFUITextEdit *textEdit,
                                               ZF_IN ZFUITextEditKeyboardReturnTypeEnum textEditKeyboardReturnType)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditKeyboardReturnTypeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textEditKeyboardReturnType);
    }

    virtual void textSelectRange(ZF_IN ZFUITextEdit *textEdit, ZF_OUT zfindexRange &textSelectRange)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId_start = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSelectRange_start"),
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        static jmethodID jmId_count = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSelectRange_count"),
            JNIGetMethodSig(JNIType::S_int, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        textSelectRange.start = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId_start,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
        textSelectRange.count = JNIUtilCallStaticIntMethod(jniEnv, this->jclsOwner, jmId_count,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }
    virtual void textSelectRangeSet(ZF_IN ZFUITextEdit *textEdit, ZF_IN const zfindexRange &textSelectRange)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSelectRangeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            (jint)textSelectRange.start,
            (jint)textSelectRange.count);
    }

public:
    virtual void textContentSet(ZF_IN ZFUITextEdit *textEdit,
                                ZF_IN ZFString *text)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textContentSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jobject, (text == zfnull) ? zfnull : text->nativeString()));
    }
    virtual void textAppearanceSet(ZF_IN ZFUITextEdit *textEdit,
                                   ZF_IN ZFUITextAppearanceEnum const &textAppearance)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textAppearanceSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textAppearance));
    }
    virtual void textAlignSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIAlignFlags const &textAlign)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textAlignSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textAlign));
    }
    virtual void textColorSet(ZF_IN ZFUITextEdit *textEdit,
                              ZF_IN ZFUIColor const &textColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textColor));
    }
    virtual void textShadowColorSet(ZF_IN ZFUITextEdit *textEdit,
                                    ZF_IN ZFUIColor const &textShadowColor)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textShadowColorSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textShadowColor));
    }
    virtual void textShadowOffsetSet(ZF_IN ZFUITextEdit *textEdit,
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
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textShadowOffset.width),
            ZFCastStatic(jint, textShadowOffset.height));
    }
    virtual void textSizeSet(ZF_IN ZFUITextEdit *textEdit,
                             ZF_IN zfint textSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textSizeSet"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, textSize));
    }

    // ============================================================
    // layout
public:
    virtual ZFUISize measureNativeTextEdit(ZF_IN ZFUITextEdit *textEdit,
                                           ZF_IN const ZFUISize &sizeHint,
                                           ZF_IN zfint textSize)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_measureNativeTextEdit"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_int)
                .add(JNIType::S_int)
                .add(JNIType::S_int)
            ).c_str());
        jobject jobjSize = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()),
            ZFCastStatic(jint, sizeHint.width),
            ZFCastStatic(jint, sizeHint.height),
            ZFCastStatic(jint, textSize));
        JNIBlockedDeleteLocalRef(jobjSize);
        return ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(jobjSize);
    }

    // ============================================================
    // edit
public:
    virtual void textEditBegin(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditBegin"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }
    virtual void textEditEnd(ZF_IN ZFUITextEdit *textEdit)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_textEditEnd"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, textEdit->nativeImplView()));
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUITextEditImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUITextEditImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(jboolean, ZFImpl_sys_Android_JNI_ID_ZFUITextEdit, native_1notifyCheckTextShouldChange,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUITextEdit,
                   jobject jobjNewString)
{
    zfblockedAlloc(ZFString, newString, ZFCastStatic(void *, ZFCastStatic(jstring, jobjNewString)));
    return (jboolean)ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyCheckTextShouldChange(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)),
        newString);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUITextEdit, native_1notifyTextChange,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUITextEdit,
                   jobject jobjNewString)
{
    zfblockedAlloc(ZFString, newString, ZFCastStatic(void *, ZFCastStatic(jstring, jobjNewString)));
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextChange(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)),
        newString);
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUITextEdit, native_1notifyTextReturnClicked,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextReturnClicked(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUITextEdit, native_1notifyTextEditBegin,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditBegin(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}
JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUITextEdit, native_1notifyTextEditEnd,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUITextEdit)
{
    ZFPROTOCOL_ACCESS(ZFUITextEdit)->notifyTextEditEnd(
        ZFCastZFObject(ZFUITextEdit *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUITextEdit)));
}

#endif // #if ZF_ENV_sys_Android

