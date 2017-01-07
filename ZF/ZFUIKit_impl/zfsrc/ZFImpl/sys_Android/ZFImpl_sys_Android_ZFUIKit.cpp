/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFUIKit.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFImpl_sys_Android_ZFUIKit_jclsHolder, ZFLevelZFFrameworkHigh)
{
    jobject tmp = zfnull;
    JNIEnv *jniEnv = JNIGetJNIEnv();

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidPoint).c_str());
    this->jclsPoint = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidSize).c_str());
    this->jclsSize = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidMargin).c_str());
    this->jclsMargin = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);

    tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFAndroidRect).c_str());
    this->jclsRect = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFImpl_sys_Android_ZFUIKit_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsPoint);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsSize);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsMargin);
    JNIUtilDeleteGlobalRef(jniEnv, this->jclsRect);
}
public:
    jclass jclsPoint;
    jclass jclsSize;
    jclass jclsMargin;
    jclass jclsRect;
ZF_GLOBAL_INITIALIZER_END(ZFImpl_sys_Android_ZFUIKit_jclsHolder)

// ============================================================
// Point
jclass ZFImpl_sys_Android_jclassZFAndroidPoint(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_jclsHolder)->jclsPoint;
}
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point, ZF_IN_OUT jobject jobjPoint)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, zfTextA("x"), JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, zfTextA("y"), JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdX, point.x);
    JNIUtilSetIntField(jniEnv, jobjPoint, jfIdY, point.y);
    return jobjPoint;
}
jobject ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(ZF_IN const ZFUIPoint &point)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsPoint, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjPoint = JNIUtilNewObject(jniEnv, jclsPoint, jmId);
    return ZFImpl_sys_Android_ZFUIPointToZFAndroidPoint(point, jobjPoint);
}
void ZFImpl_sys_Android_ZFUIPointFromZFAndroidPoint(ZF_OUT ZFUIPoint &ret, ZF_IN jobject jobjPoint)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsPoint = ZFImpl_sys_Android_jclassZFAndroidPoint();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsPoint, zfTextA("x"), JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsPoint, zfTextA("y"), JNIType::S_int.getId());
    ret.x = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdX);
    ret.y = JNIUtilGetIntField(jniEnv, jobjPoint, jfIdY);
}

// ============================================================
// Size
jclass ZFImpl_sys_Android_jclassZFAndroidSize(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_jclsHolder)->jclsSize;
}
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size, ZF_IN_OUT jobject jobjSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, zfTextA("width"), JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, zfTextA("height"), JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdWidth, size.width);
    JNIUtilSetIntField(jniEnv, jobjSize, jfIdHeight, size.height);
    return jobjSize;
}
jobject ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(ZF_IN const ZFUISize &size)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsSize, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjSize = JNIUtilNewObject(jniEnv, jclsSize, jmId);
    return ZFImpl_sys_Android_ZFUISizeToZFAndroidSize(size, jobjSize);
}
void ZFImpl_sys_Android_ZFUISizeFromZFAndroidSize(ZF_OUT ZFUISize &ret, ZF_IN jobject jobjSize)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsSize = ZFImpl_sys_Android_jclassZFAndroidSize();
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsSize, zfTextA("width"), JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsSize, zfTextA("height"), JNIType::S_int.getId());
    ret.width = JNIUtilGetIntField(jniEnv, jobjSize, jfIdWidth);
    ret.height = JNIUtilGetIntField(jniEnv, jobjSize, jfIdHeight);
}

// ============================================================
// Margin
jclass ZFImpl_sys_Android_jclassZFAndroidMargin(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_jclsHolder)->jclsMargin;
}
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin, ZF_IN_OUT jobject jobjMargin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("left"), JNIType::S_int.getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("top"), JNIType::S_int.getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("right"), JNIType::S_int.getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("bottom"), JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdLeft, margin.left);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdTop, margin.top);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdRight, margin.right);
    JNIUtilSetIntField(jniEnv, jobjMargin, jfIdBottom, margin.bottom);
    return jobjMargin;
}
jobject ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(ZF_IN const ZFUIMargin &margin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsMargin, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjMargin = JNIUtilNewObject(jniEnv, jclsMargin, jmId);
    return ZFImpl_sys_Android_ZFUIMarginToZFAndroidMargin(margin, jobjMargin);
}
void ZFImpl_sys_Android_ZFUIMarginFromZFAndroidMargin(ZF_OUT ZFUIMargin &ret, ZF_IN jobject jobjMargin)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsMargin = ZFImpl_sys_Android_jclassZFAndroidMargin();
    static jfieldID jfIdLeft = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("left"), JNIType::S_int.getId());
    static jfieldID jfIdTop = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("top"), JNIType::S_int.getId());
    static jfieldID jfIdRight = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("right"), JNIType::S_int.getId());
    static jfieldID jfIdBottom = JNIUtilGetFieldID(jniEnv, jclsMargin, zfTextA("bottom"), JNIType::S_int.getId());
    ret.left = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdLeft);
    ret.top = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdTop);
    ret.right = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdRight);
    ret.bottom = JNIUtilGetIntField(jniEnv, jobjMargin, jfIdBottom);
}

// ============================================================
// Rect
jclass ZFImpl_sys_Android_jclassZFAndroidRect(void)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFImpl_sys_Android_ZFUIKit_jclsHolder)->jclsRect;
}
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect, ZF_IN_OUT jobject jobjRect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("x"), JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("y"), JNIType::S_int.getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("width"), JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("height"), JNIType::S_int.getId());
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdX, rect.point.x);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdY, rect.point.y);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdWidth, rect.size.width);
    JNIUtilSetIntField(jniEnv, jobjRect, jfIdHeight, rect.size.height);
    return jobjRect;
}
jobject ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(ZF_IN const ZFUIRect &rect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jmethodID jmId = JNIUtilGetMethodID(jniEnv, jclsRect, JNIConstructorName,
        JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
        ).c_str());
    jobject jobjRect = JNIUtilNewObject(jniEnv, jclsRect, jmId);
    return ZFImpl_sys_Android_ZFUIRectToZFAndroidRect(rect, jobjRect);
}
void ZFImpl_sys_Android_ZFUIRectFromZFAndroidRect(ZF_OUT ZFUIRect &ret, ZF_IN jobject jobjRect)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jclass jclsRect = ZFImpl_sys_Android_jclassZFAndroidRect();
    static jfieldID jfIdX = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("x"), JNIType::S_int.getId());
    static jfieldID jfIdY = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("y"), JNIType::S_int.getId());
    static jfieldID jfIdWidth = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("width"), JNIType::S_int.getId());
    static jfieldID jfIdHeight = JNIUtilGetFieldID(jniEnv, jclsRect, zfTextA("height"), JNIType::S_int.getId());
    ret.point.x = JNIUtilGetIntField(jniEnv, jobjRect, jfIdX);
    ret.point.y = JNIUtilGetIntField(jniEnv, jobjRect, jfIdY);
    ret.size.width = JNIUtilGetIntField(jniEnv, jobjRect, jfIdWidth);
    ret.size.height = JNIUtilGetIntField(jniEnv, jobjRect, jfIdHeight);
}

ZF_NAMESPACE_GLOBAL_END

#endif

