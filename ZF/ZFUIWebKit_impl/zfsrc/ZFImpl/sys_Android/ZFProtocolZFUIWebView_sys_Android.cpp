/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIWebKit.h"
#include "ZFUIWebKit/protocol/ZFProtocolZFUIWebView.h"

#if ZF_ENV_sys_Android

#define ZFImpl_sys_Android_JNI_ID_ZFUIWebView ZFImpl_sys_Android_JNI_ID(ZFUIWebKit_ZFUIWebView)
#define ZFImpl_sys_Android_JNI_NAME_ZFUIWebView ZFImpl_sys_Android_JNI_NAME(ZFUIWebKit.ZFUIWebView)

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIWebViewImpl_sys_Android, ZFUIWebView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:WebView"))

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFUIWebView).c_str());
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
    virtual void *nativeWebViewCreate(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeWebViewCreate"),
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, webView->toObject()));
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ZFCastStatic(void *, ret);
    }
    virtual void nativeWebViewDestroy(ZF_IN ZFUIWebView *webView,
                                      ZF_IN void *nativeWebView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_nativeWebViewDestroy"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject nativeWebViewTmp = ZFCastStatic(jobject, nativeWebView);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, nativeWebViewTmp);
        JNIUtilDeleteGlobalRef(jniEnv, nativeWebViewTmp);
    }

    virtual void webLoadUrl(ZF_IN ZFUIWebView *webView,
                            ZF_IN const zfchar *url)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webLoadUrl"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject urlTmp = JNIUtilNewStringUTF(jniEnv, url);
        JNIBlockedDeleteLocalRef(urlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()),
            urlTmp);
    }
    virtual void webLoadHtml(ZF_IN ZFUIWebView *webView,
                             ZF_IN const zfchar *html,
                             ZF_IN_OPT const zfchar *baseUrl = zfnull)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webLoadHtml"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        jobject htmlTmp = JNIUtilNewStringUTF(jniEnv, html);
        JNIBlockedDeleteLocalRef(htmlTmp);
        jobject baseUrlTmp = ((baseUrl == zfnull) ? NULL : JNIUtilNewStringUTF(jniEnv, baseUrl));
        JNIBlockedDeleteLocalRef(baseUrlTmp);
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()),
            htmlTmp,
            baseUrlTmp);
    }
    virtual void webReload(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webReload"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual void webLoadStop(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webLoadStop"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

    virtual void webGoBack(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webGoBack"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual void webGoForward(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webGoForward"),
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

    virtual zfbool webLoading(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webLoading"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual zfbool webGoBackAvailable(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webGoBackAvailable"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }
    virtual zfbool webGoForwardAvailable(ZF_IN ZFUIWebView *webView)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, zfTextA("native_webGoForwardAvailable"),
            JNIGetMethodSig(JNIType::S_boolean, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        return JNIUtilCallStaticBooleanMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, webView->nativeImplView()));
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIWebViewImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIWebViewImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE(void, ZFImpl_sys_Android_JNI_ID_ZFUIWebView, native_1ZFUIWebView_1notifyWebLoadStateChanged,
                   JNIEnv *jniEnv, jclass jniCls,
                   JNIPointer zfjniPointerOwnerZFUIWebView)
{
    ZFPROTOCOL_ACCESS(ZFUIWebView)->notifyWebLoadStateChanged(
        ZFCastZFObject(ZFUIWebView *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerOwnerZFUIWebView)));
}

#endif // #if ZF_ENV_sys_Android

