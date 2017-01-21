/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFUILayoutParam.h
 * @brief layout param types for UI
 */

#ifndef _ZFI_ZFUILayoutParam_h_
#define _ZFI_ZFUILayoutParam_h_

#include "ZFUIKitDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUISizeType
/**
 * @brief size params for parent to define how to measure children,
 *   see #ZFUIViewLayoutParam for more info
 */
ZFENUM_BEGIN(ZFUISizeType)
    /**
     * @brief wrap child to minimal
     *
     * associate size hint means the max size child can reach
     */
    ZFENUM_VALUE_WITH_INIT(Wrap, 0)
    /**
     * @brief fill child to parent
     *
     * associate size hint means the size child to fill
     */
    ZFENUM_VALUE_WITH_INIT(Fill, 1)
ZFENUM_SEPARATOR(ZFUISizeType)
    ZFENUM_VALUE_REGISTER(Wrap)
    ZFENUM_VALUE_REGISTER(Fill)
ZFENUM_END(ZFUISizeType)

// ============================================================
// ZFUISizeParam
/**
 * @brief 2D size
 */
zfclassPOD ZF_ENV_EXPORT ZFUISizeParam
{
public:
    ZFUISizeTypeEnum width;   /**< @brief width */
    ZFUISizeTypeEnum height;  /**< @brief height */
};

ZFCORETYPE_STRING_CONVERTER_DECLARE(ZFUISizeParam, ZFUISizeParam)

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFUISizeParam
 *       value="(Wrap, Wrap)" // optional, #ZFUISizeType::e_Wrap by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFUISizeParam, ZFUISizeParam)

ZFOUTPUT_TYPE(ZFUISizeParam, {output << ZFUISizeParamToString(v);})
ZFOUTPUT_TYPE(const ZFUISizeParam *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFUISizeParam *, {output << (const ZFUISizeParam *)v;})
ZFINPUT_TYPE_DECLARE(ZFUISizeParam, ZFUISizeParam)

ZFCORE_POD_COMPARER_DECLARE(ZFUISizeParam)
ZFCOMPARER_DEFAULT_DECLARE(ZFUISizeParam, ZFUISizeParam, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief make a ZFUISizeParam
 */
inline ZFUISizeParam ZFUISizeParamMake(ZF_IN ZFUISizeTypeEnum const &width,
                                       ZF_IN ZFUISizeTypeEnum const &height)
{
    ZFUISizeParam ret = {width, height};
    return ret;
}
/**
 * @brief make a ZFUISizeParam
 */
inline ZFUISizeParam ZFUISizeParamMake(ZF_IN ZFUISizeTypeEnum const &v)
{
    ZFUISizeParam ret = {v, v};
    return ret;
}

/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Wrap)
 */
extern ZF_ENV_EXPORT const ZFUISizeParam ZFUISizeParamWrapWidthWrapHeight;
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Wrap)
 */
extern ZF_ENV_EXPORT const ZFUISizeParam ZFUISizeParamFillWidthWrapHeight;
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Wrap, #ZFUISizeType::e_Fill)
 */
extern ZF_ENV_EXPORT const ZFUISizeParam ZFUISizeParamWrapWidthFillHeight;
/**
 * @brief #ZFUISizeParamMake(#ZFUISizeType::e_Fill, #ZFUISizeType::e_Fill)
 */
extern ZF_ENV_EXPORT const ZFUISizeParam ZFUISizeParamFillWidthFillHeight;

// ============================================================
// ZFUILayoutParam
/**
 * @brief base class of all layout param in ZFFramework
 *
 * @note all layout param must be declared as #ZFProperty,
 *   and must be serializable
 * @note by default, a layout param would be compared by comparing all property
 */
zfclass ZF_ENV_EXPORT ZFUILayoutParam : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUILayoutParam, ZFStyleableObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when layout param changed,
     * this event would be automatically fired if any of property value changed
     * (#ZFObject::EventObjectPropertyValueAfterChange),
     * you may also notify it manually to show the layout param changed
     */
    ZFOBSERVER_EVENT(LayoutParamOnChange)

public:
    zfoverride
    virtual zfidentity objectHash(void)
    {
        return zfidentityCalcString(this->classData()->className());
    }
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj)
    {
        if(anotherObj != zfnull && anotherObj->classData()->classIsSubclassOf(ZFUILayoutParam::ClassData())
            && ZFClassUtil::allPropertyIsEqual(this, anotherObj))
        {
            return ZFCompareTheSame;
        }
        return ZFCompareUncomparable;
    }

protected:
    zfoverride
    virtual void objectPropertyValueAfterChange(ZF_IN const ZFProperty *property)
    {
        zfsuper::objectPropertyValueAfterChange(property);
        this->layoutParamOnChange();
    }
protected:
    /** @brief see #EventLayoutParamOnChange */
    virtual inline void layoutParamOnChange(void)
    {
        this->observerNotify(ZFUILayoutParam::EventLayoutParamOnChange());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUILayoutParam_h_

