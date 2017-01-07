/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSerializableDataStringConverterDef.h"
#include "ZFSerializableUtilDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// escape char map for serializable data
// default is all number and lower/upper case letters only
static const zfchar _ZFP_ZFSerializableEscapeCharMap[256] = {
    // 0x00 ~ 0x0F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x10 ~ 0x1F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x20 ~ 0x2F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x30 ~ 0x3F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    // 0x40 ~ 0x4F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x50 ~ 0x5F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x60 ~ 0x6F
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    // 0x70 ~ 0x7F
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    // 0x80 ~ 0x8F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0x90 ~ 0x9F
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xA0 ~ 0xAF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xB0 ~ 0xBF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xC0 ~ 0xCF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xD0 ~ 0xDF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xE0 ~ 0xEF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // 0xF0 ~ 0xFF
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    //
};

// ============================================================
/*
 * // '_' if null for encoded data
 *
 * {
 *     ClassNameEncoded
 *     (
 *         refTypeEncoded : refDataEncoded
 *     )
 *     (
 *         AttributeNameEncoded = AttributeValueEncoded;
 *         AttributeNameEncoded = AttributeValueEncoded;
 *     )
 *     [
 *         {ReferenceData},
 *         {ChildElement0},
 *         {ChildElement1}
 *     ]
 * }
 */
zfbool _ZFP_ZFSerializableDataFromString(ZF_OUT ZFSerializableData &serializableData,
                                         ZF_IN_OUT const zfchar *&encodedData,
                                         ZF_IN zfindex encodedDataLen,
                                         ZF_OUT zfstring *outErrorHintToAppend,
                                         ZF_IN_OPT zfbool validateTail = zffalse)
{
    if(encodedData == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("invalid param"));
        return zffalse;
    }
    const zfchar *srcEnd = (encodedData + ((encodedDataLen == zfindexMax) ? zfslen(encodedData) : encodedDataLen));
    zfbool ret = zffalse;
    do
    {
        const zfchar *pLeft = zfnull;
        const zfchar *pRight = zfnull;
        zfstring decodedTmp;

        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd || *encodedData != '{') {break;}
        ++encodedData;

        // class name
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        pLeft = pRight = encodedData;
        while(*encodedData != '(' && encodedData < srcEnd)
        {
            if(!zfcharIsSpace(*encodedData))
            {
                zfcharMoveNext(encodedData);
                pRight = encodedData;
            }
            else
            {
                zfcharMoveNext(encodedData);
            }
        }
        if(encodedData >= srcEnd || *encodedData != '(') {break;}
        ++encodedData;

        if(pRight == pLeft + 1 && *pLeft == '_')
        {
            serializableData.itemClassSet(zfnull);
        }
        else
        {
            ZFCoreDataDecode(decodedTmp, zfstring(pLeft, pRight - pLeft).cString());
            serializableData.itemClassSet(decodedTmp.cString());
            decodedTmp.removeAll();
        }

        // refType
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        pLeft = pRight = encodedData;
        while(*encodedData != ':' && encodedData < srcEnd)
        {
            if(!zfcharIsSpace(*encodedData))
            {
                zfcharMoveNext(encodedData);
                pRight = encodedData;
            }
            else
            {
                zfcharMoveNext(encodedData);
            }
        }
        if(encodedData >= srcEnd || *encodedData != ':') {break;}
        ++encodedData;

        if(pRight == pLeft + 1 && *pLeft == '_')
        {
            serializableData.referenceRefTypeSet(zfnull);
        }
        else
        {
            ZFCoreDataDecode(decodedTmp, zfstring(pLeft, pRight - pLeft).cString());
            serializableData.referenceRefTypeSet(decodedTmp.cString());
            decodedTmp.removeAll();
        }

        // refData
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        pLeft = pRight = encodedData;
        while(*encodedData != ')' && encodedData < srcEnd)
        {
            if(!zfcharIsSpace(*encodedData))
            {
                zfcharMoveNext(encodedData);
                pRight = encodedData;
            }
            else
            {
                zfcharMoveNext(encodedData);
            }
        }
        if(encodedData >= srcEnd || *encodedData != ')') {break;}
        ++encodedData;

        if(pRight == pLeft + 1 && *pLeft == '_')
        {
            serializableData.referenceRefDataSet(zfnull);
        }
        else
        {
            ZFCoreDataDecode(decodedTmp, zfstring(pLeft, pRight - pLeft).cString());
            serializableData.referenceRefDataSet(decodedTmp.cString());
            decodedTmp.removeAll();
        }

        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        while(*encodedData != '(' && encodedData < srcEnd) {zfcharMoveNext(encodedData);}
        if(encodedData >= srcEnd || *encodedData != '(') {break;}
        ++encodedData;

        // attributes
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        while(encodedData < srcEnd)
        {
            if(*encodedData == ')')
            {
                ++encodedData;
                ret = zftrue;
                break;
            }

            // name
            pLeft = pRight = encodedData;
            while(*encodedData != '=' && encodedData < srcEnd)
            {
                if(!zfcharIsSpace(*encodedData))
                {
                    zfcharMoveNext(encodedData);
                    pRight = encodedData;
                }
                else
                {
                    zfcharMoveNext(encodedData);
                }
            }
            if(encodedData >= srcEnd || *encodedData != '=') {break;}
            ++encodedData;

            zfstring attributeName;
            ZFCoreDataDecode(attributeName, zfstring(pLeft, pRight - pLeft).cString());

            // value
            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            pLeft = pRight = encodedData;
            while(*encodedData != ';' && encodedData < srcEnd)
            {
                if(!zfcharIsSpace(*encodedData))
                {
                    zfcharMoveNext(encodedData);
                    pRight = encodedData;
                }
                else
                {
                    zfcharMoveNext(encodedData);
                }
            }
            if(encodedData >= srcEnd || *encodedData != ';') {break;}
            ++encodedData;

            ZFCoreDataDecode(decodedTmp, zfstring(pLeft, pRight - pLeft).cString());

            // save
            if(!attributeName.isEmpty() && !decodedTmp.isEmpty())
            {
                serializableData.attributeSet(attributeName.cString(), decodedTmp.cString());
            }
            decodedTmp.removeAll();

            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        }
        if(!ret) {break;}
        ret = zffalse;

        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd || *encodedData != '[') {break;}
        ++encodedData;

        // elements
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        while(encodedData < srcEnd)
        {
            if(*encodedData == ']')
            {
                ++encodedData;
                ret = zftrue;
                break;
            }

            {
                ZFSerializableData element;
                if(!_ZFP_ZFSerializableDataFromString(element, encodedData, srcEnd - encodedData, outErrorHintToAppend))
                {
                    return zffalse;
                }
                serializableData.elementAdd(element);
            }

            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            if(encodedData >= srcEnd || (*encodedData != ',' && *encodedData != ']')) {break;}
            if(*encodedData == ',')
            {
                ++encodedData;
                zfcharSkipSpaceAndEndl(encodedData, srcEnd);
                if(encodedData >= srcEnd || *encodedData == ']') {break;}
            }
        }
        if(!ret) {break;}
        ret = zffalse;

        // tail
        zfcharSkipSpaceAndEndl(encodedData, srcEnd);
        if(encodedData >= srcEnd || *encodedData != '}') {break;}
        ++encodedData;
        if(validateTail)
        {
            zfcharSkipSpaceAndEndl(encodedData, srcEnd);
            if(encodedData < srcEnd) {break;}
        }

        ret = zftrue;
    } while(zffalse);
    if(!ret)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
            zfText("wrong serializable string format at position: \"%s\""),
            zfstring(encodedData, srcEnd - encodedData).cString());
    }
    return ret;
}
zfbool ZFSerializableDataFromString(ZF_OUT ZFSerializableData &serializableData,
                                    ZF_IN const zfchar *encodedData,
                                    ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    return _ZFP_ZFSerializableDataFromString(serializableData, encodedData, encodedDataLen, outErrorHintToAppend, zfHint("validateTail")zftrue);
}
ZFSerializableData ZFSerializableDataFromString(ZF_IN const zfchar *encodedData,
                                                ZF_IN_OPT zfindex encodedDataLen /* = zfindexMax */,
                                                ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromString(ret, encodedData, encodedDataLen, outErrorHintToAppend))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

zfbool ZFSerializableDataFromInput(ZF_OUT ZFSerializableData &serializableData,
                                   ZF_IN const ZFInputCallback &input,
                                   ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!input.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("invalid input callback"));
        return zffalse;
    }
    ZFBuffer buf = ZFInputCallbackReadToBuffer(input);
    if(buf.buffer() == zfnull)
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("unable to load data from input"));
        return zffalse;
    }
    return ZFSerializableDataFromString(serializableData, buf.bufferAsString(), buf.bufferAsStringLength(), outErrorHintToAppend);
}
ZFSerializableData ZFSerializableDataFromInput(ZF_IN const ZFInputCallback &input,
                                               ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    ZFSerializableData ret;
    if(ZFSerializableDataFromInput(ret, input, outErrorHintToAppend))
    {
        return ret;
    }
    else
    {
        return ZFSerializableData();
    }
}

// ============================================================
zfbool ZFSerializableDataToString(ZF_OUT zfstring &result,
                                  ZF_IN const ZFSerializableData &serializableData,
                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    result += '{';

    // serializable class
    if(serializableData.itemClass() == zfnull)
    {
        result += '_';
    }
    else
    {
        ZFCoreDataEncode(result, serializableData.itemClass(), zfindexMax, _ZFP_ZFSerializableEscapeCharMap);
    }

    // refType and refData
    result += '(';
    if(serializableData.referenceRefType() == zfnull)
    {
        result += '_';
    }
    else
    {
        ZFCoreDataEncode(result, serializableData.referenceRefType(), zfindexMax, _ZFP_ZFSerializableEscapeCharMap);
    }
    result += ':';
    if(serializableData.referenceRefData() == zfnull)
    {
        result += '_';
    }
    else
    {
        ZFCoreDataEncode(result, serializableData.referenceRefData(), zfindexMax, _ZFP_ZFSerializableEscapeCharMap);
    }
    result += ')';

    // attributes
    result += '(';
    for(zfiterator it = serializableData.attributeIterator();
        serializableData.attributeIteratorIsValid(it);
        serializableData.attributeIteratorNext(it))
    {
        ZFCoreDataEncode(result, serializableData.attributeIteratorGetKey(it), zfindexMax, _ZFP_ZFSerializableEscapeCharMap);
        result += '=';
        ZFCoreDataEncode(result, serializableData.attributeIteratorGet(it), zfindexMax, _ZFP_ZFSerializableEscapeCharMap);
        result += ';';
    }
    result += ')';

    // elements
    result += '[';
    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        if(i != 0)
        {
            result += ',';
        }
        if(!ZFSerializableDataToString(result, serializableData.elementAtIndex(i), outErrorHintToAppend))
        {
            return zffalse;
        }
    }
    result += ']';

    result += '}';
    return zftrue;
}
zfstring ZFSerializableDataToString(ZF_IN const ZFSerializableData &serializableData,
                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    zfstring tmp;
    ZFSerializableDataToString(tmp, serializableData, outErrorHintToAppend);
    return tmp;
}
zfbool ZFSerializableDataToOutput(ZF_IN_OUT const ZFOutputCallback &output,
                                  ZF_IN const ZFSerializableData &serializableData,
                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!output.callbackIsValid())
    {
        ZFSerializableUtil::errorOccurred(outErrorHintToAppend, zfText("invalid output callback"));
        return zffalse;
    }
    zfstring tmp;
    if(!ZFSerializableDataToString(tmp, serializableData, outErrorHintToAppend))
    {
        return zffalse;
    }
    output.execute(tmp.cString(), tmp.length());
    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

