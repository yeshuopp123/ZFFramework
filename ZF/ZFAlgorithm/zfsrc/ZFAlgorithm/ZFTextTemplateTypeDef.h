/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFTextTemplateTypeDef.h
 * @brief text template util
 */

#ifndef _ZFI_ZFTextTemplateTypeDef_h_
#define _ZFI_ZFTextTemplateTypeDef_h_

#include "ZFAlgorithmDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief index output flags for #ZFTextTemplateIndexData
 */
ZFENUM_BEGIN(ZFTextTemplateIndexFlag)
    ZFENUM_VALUE(LeadingZero)
    ZFENUM_VALUE(LeadingSpace)
    ZFENUM_VALUE(TailSpace)
ZFENUM_SEPARATOR(ZFTextTemplateIndexFlag)
    ZFENUM_VALUE_REGISTER(LeadingZero)
    ZFENUM_VALUE_REGISTER(LeadingSpace)
    ZFENUM_VALUE_REGISTER(TailSpace)
ZFENUM_END_WITH_DEFAULT(ZFTextTemplateIndexFlag, LeadingZero)

// ============================================================
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateIndexData
{
public:
    /**
     * @brief start index, 0 by default
     */
    zfindex indexStart;
    /**
     * @brief increase or decrease the index, 1 by default,
     *   must not be 0
     */
    zfint indexOffset;
    /**
     * @brief radix, 10 by default
     */
    zfindex indexRadix;
    /**
     * @brief upper case or not, zftrue by default
     */
    zfbool indexUpperCase;
    /**
     * @brief specify width, 0 to disable, 0 by default
     */
    zfindex indexWidth;
    /**
     * @brief flag to use when output index,
     *   valid only if #indexWidth greater than 0,
     *   #ZFTextTemplateIndexFlag::EnumDefault by default
     */
    ZFTextTemplateIndexFlagEnum indexFlag;

public:
    ZFTextTemplateIndexData(void)
    : indexStart(0)
    , indexOffset(1)
    , indexRadix(10)
    , indexUpperCase(zftrue)
    , indexWidth(0)
    , indexFlag(ZFTextTemplateIndexFlag::EnumDefault())
    {
    }

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateIndexData &ref)
    {
        this->indexStart = ref.indexStart;
        this->indexOffset = ref.indexOffset;
        this->indexRadix = ref.indexRadix;
        this->indexUpperCase = ref.indexUpperCase;
        this->indexWidth = ref.indexWidth;
        this->indexFlag = ref.indexFlag;
    }
    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFTextTemplateIndexData &ref) const
    {
        return (zftrue
                && this->indexStart == ref.indexStart
                && this->indexOffset == ref.indexOffset
                && this->indexRadix == ref.indexRadix
                && this->indexUpperCase == ref.indexUpperCase
                && this->indexWidth == ref.indexWidth
                && this->indexFlag == ref.indexFlag
            );
    }
    zfbool operator != (ZF_IN const ZFTextTemplateIndexData &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */

public:
    /** @brief see #ZFObject::objectInfoT */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #ZFObject::objectInfo */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }
};
ZFCOMPARER_DEFAULT_DECLARE(ZFTextTemplateIndexData, ZFTextTemplateIndexData, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateIndexData
 *       indexStart="0" // optional, 0 by default
 *       indexOffset="1" // optional, 1 by default
 *       indexRadix="10" // optional, 10 by default
 *       indexUpperCase="true" // optional, true by default
 *       indexWidth="0" // optional, 0 by default
 *       indexFlag="LeadingZero" // optional, ZFTextTemplateIndexFlag::EnumDefault() by default
 *   />
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFTextTemplateIndexData, ZFTextTemplateIndexData)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexStart zfText("indexStart")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexOffset zfText("indexOffset")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexRadix zfText("indexRadix")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexUpperCase zfText("indexUpperCase")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexWidth zfText("indexWidth")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateIndexData_indexFlag zfText("indexFlag")

ZFOUTPUT_TYPE(ZFTextTemplateIndexData, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFTextTemplateIndexData *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTextTemplateIndexData *, {output << (const ZFTextTemplateIndexData *)v;})

// ============================================================
zfclassFwd _ZFP_ZFTextTemplateParamPrivate;
/** @brief see #ZFTextTemplateApply */
zfclassLikePOD ZF_ENV_EXPORT ZFTextTemplateParam
{
public:
    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void replaceDataAdd(ZF_IN const zfchar *key, ZF_IN const zfchar *value);
    /** @brief see #ZFTextTemplateApply */
    const zfchar *replaceData(ZF_IN const zfchar *key) const;
    /** @brief see #ZFTextTemplateApply */
    zfindex replaceDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfchar *replaceDataNameAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    const zfchar *replaceDataAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemove(ZF_IN const zfchar *key);
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemoveAtIndex(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void replaceDataRemoveAll(void);

    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void enableDataDefaultSet(ZF_IN zfbool enableDataDefault);
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataDefault(void) const;

    /** @brief see #ZFTextTemplateApply */
    void enableDataAdd(ZF_IN const zfchar *key, ZF_IN zfbool value);
    /** @brief see #ZFTextTemplateApply */
    const zfbool *enableData(ZF_IN const zfchar *key) const;
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataValue(ZF_IN const zfchar *key) const;
    /** @brief see #ZFTextTemplateApply */
    zfindex enableDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfchar *enableDataNameAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    zfbool enableDataAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemove(ZF_IN const zfchar *key);
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemoveAtIndex(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void enableDataRemoveAll(void);

    // ============================================================
    /** @brief see #ZFTextTemplateApply */
    void indexDataDefaultSet(ZF_IN const ZFTextTemplateIndexData &indexDataDefault);
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData &indexDataDefault(void) const;

    /** @brief see #ZFTextTemplateApply */
    void indexDataAdd(ZF_IN const zfchar *key, ZF_IN const ZFTextTemplateIndexData &value);
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData *indexData(ZF_IN const zfchar *key) const;
    /** @brief see #ZFTextTemplateApply */
    ZFTextTemplateIndexData *indexData(ZF_IN const zfchar *key);
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData *indexDataValue(ZF_IN const zfchar *key) const;
    /** @brief see #ZFTextTemplateApply */
    zfindex indexDataCount(void) const;
    /** @brief see #ZFTextTemplateApply */
    const zfchar *indexDataNameAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    const ZFTextTemplateIndexData *indexDataAtIndex(ZF_IN zfindex index) const;
    /** @brief see #ZFTextTemplateApply */
    ZFTextTemplateIndexData *indexDataAtIndex(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemove(ZF_IN const zfchar *key);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemoveAtIndex(ZF_IN zfindex index);
    /** @brief see #ZFTextTemplateApply */
    void indexDataRemoveAll(void);

public:
    /** @brief copy from another */
    void copyFrom(ZF_IN const ZFTextTemplateParam &ref);

    /** @cond ZFPrivateDoc */
    zfbool operator == (ZF_IN const ZFTextTemplateParam &ref) const;
    zfbool operator != (ZF_IN const ZFTextTemplateParam &ref) const
    {
        return !this->operator == (ref);
    }
    /** @endcond */

public:
    /** @brief see #ZFObject::objectInfoT */
    void objectInfoT(ZF_IN_OUT zfstring &ret) const;
    /** @brief see #ZFObject::objectInfo */
    inline zfstring objectInfo(void) const
    {
        zfstring ret;
        this->objectInfoT(ret);
        return ret;
    }

public:
    ZFTextTemplateParam(void);
    ~ZFTextTemplateParam(void);
private:
    _ZFP_ZFTextTemplateParamPrivate *d;
};
ZFCOMPARER_DEFAULT_DECLARE(ZFTextTemplateParam, ZFTextTemplateParam, {
        return ((v0 == v1) ? ZFCompareTheSame : ZFCompareUncomparable);
    })

/**
 * @brief see #ZFPROPERTY_TYPE_DECLARE
 *
 * serializable data:
 * @code
 *   <ZFTextTemplateParam>
 *       <node category="replace"> // optional
 *           <zfstring name="key0" value="value0" />
 *           <zfstring name="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <node category="enable"> // optional
 *           <zfbool name="key0" value="value0" />
 *           <zfbool name="key1" value="value1" />
 *           ...
 *       </node>
 *
 *       <zfbool category="enableDataDefault" ... /> // optional, false by default
 *
 *       <node category="index"> // optional
 *           <ZFTextTemplateIndexData name="key0" ... />
 *           <ZFTextTemplateIndexData name="key1" ... />
 *           ...
 *       </node>
 *
 *       <ZFTextTemplateIndexData category="indexDataDefault" ... /> // optional
 *   </ZFTextTemplateParam>
 * @endcode
 */
ZFPROPERTY_TYPE_DECLARE(ZFTextTemplateParam, ZFTextTemplateParam)

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_replace zfText("replace")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enable zfText("enable")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_enableDataDefault zfText("enableDataDefault")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_index zfText("index")
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFTextTemplateParam_indexDataDefault zfText("indexDataDefault")

ZFOUTPUT_TYPE(ZFTextTemplateParam, {output << v.objectInfo();})
ZFOUTPUT_TYPE(const ZFTextTemplateParam *, {if(v) {output << *v;} else {output.execute(ZFTOKEN_zfnull);}})
ZFOUTPUT_TYPE(ZFTextTemplateParam *, {output << (const ZFTextTemplateParam *)v;})

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFTextTemplateTypeDef_h_

