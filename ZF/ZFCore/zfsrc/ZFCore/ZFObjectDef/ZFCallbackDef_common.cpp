/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCallbackDef_common.h"
#include "ZFSerializableDataDef.h"
#include "ZFCallbackSerializableDef.h"
#include "ZFObjectDef.h"
#include "ZFObjectUtilDef.h"
#include "ZFObjectSmartPointerDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN


zfindex ZFIOCallbackCalcFSeek(ZF_IN zfindex offset,
                              ZF_IN zfindex length,
                              ZF_IN zfindex curPos,
                              ZF_IN zfindex seekByteSize,
                              ZF_IN ZFSeekPos seekPos)
{
    switch(seekPos)
    {
        case ZFSeekPosBegin:
            return ((offset + seekByteSize > length) ? length : (offset + seekByteSize));
        case ZFSeekPosCur:
            return ((curPos + seekByteSize > length) ? length : curPos + seekByteSize);
        case ZFSeekPosCurReversely:
            return ((offset + seekByteSize > curPos) ? offset : (curPos - seekByteSize));
        case ZFSeekPosEnd:
            return ((offset + seekByteSize > length) ? offset : (length - seekByteSize));
        default:
            zfCoreCriticalShouldNotGoHere();
            return zfindexMax;
    }
}

// ============================================================
zfindex ZFIOCallback::ioSeek(ZF_IN zfindex byteSize,
                             ZF_IN_OPT ZFSeekPos pos /* = ZFSeekPosBegin */) const
{
    ZFObject *owner = this->callbackTagGet(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull)
    {
        return zfindexMax;
    }
    const ZFMethod *method = owner->classData()->methodForName(zfText("ioSeek"));
    if(method == zfnull)
    {
        return zfindexMax;
    }
    return method->execute<zfindex, zfindex, ZFSeekPos>(owner, byteSize, pos);
}
zfindex ZFIOCallback::ioTell(void) const
{
    ZFObject *owner = this->callbackTagGet(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull)
    {
        return zfindexMax;
    }
    const ZFMethod *method = owner->classData()->methodForName(zfText("ioTell"));
    if(method == zfnull)
    {
        return zfindexMax;
    }
    return method->execute<zfindex>(owner);
}
zfindex ZFIOCallback::ioSize(void) const
{
    ZFObject *owner = this->callbackTagGet(ZFCallbackTagKeyword_ioOwner);
    if(owner == zfnull)
    {
        return zfindexMax;
    }
    const ZFMethod *method = owner->classData()->methodForName(zfText("ioSize"));
    if(method == zfnull)
    {
        return zfindexMax;
    }
    return method->execute<zfindex>(owner);
}

// ============================================================
// ZFOutputCallbackForString
zfclass _ZFP_ZFOutputCallbackForStringOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFOutputCallbackForStringOwner, ZFObject)

public:
    zfstring *pString;
    zfindex savedLength;
    ZFMETHOD_DECLARE_2(zfindex, onOutput, const void *, s, zfindex, count)
    {
        pString->append((const zfchar *)s, count);
        return count;
    }
    ZFMETHOD_DECLARE_2(zfindex, ioSeek, zfindex, byteSize, ZFSeekPos, pos)
    {
        zfindex tmp = ZFIOCallbackCalcFSeek(this->savedLength, this->pString->length(), this->pString->length(), byteSize, pos);
        this->pString->remove(tmp);
        return tmp - this->savedLength;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    {
        return ((this->pString->length() >= this->savedLength) ? (this->pString->length() - this->savedLength) : zfindexMax);
    }
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
    {
        return this->ioTell();
    }
};
ZFOutputCallback _ZFP_ZFOutputCallbackForString(ZF_IN const zfcharA *ownerFilePath,
                                                ZF_IN const zfcharA *ownerFunctionName,
                                                ZF_IN zfindex ownerFileLine,
                                                ZF_IN zfstring &s)
{
    _ZFP_ZFOutputCallbackForStringOwner *owner = zfAlloc(_ZFP_ZFOutputCallbackForStringOwner);
    owner->pString = &s;
    owner->savedLength = owner->pString->length();
    ZFOutputCallback ret = ZFCallbackForMemberMethodDetail(
        owner, ZFMethodAccess(_ZFP_ZFOutputCallbackForStringOwner, onOutput),
        ownerFilePath, ownerFunctionName, ownerFileLine);
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

// ============================================================
// ZFOutputCallbackForBuffer
zfclass _ZFP_ZFOutputCallbackForBufferOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFOutputCallbackForBufferOwner, ZFObject)

public:
    zfbool autoAppendNullToken;
    zfbyte *pStart;
    zfbyte *pEnd; // autoAppendNullToken ? (buf + maxCount - 1) : (buf + maxCount)
    zfbyte *p;

public:
    ZFMETHOD_DECLARE_2(zfindex, onOutput, const void *, s, zfindex, count)
    {
        const zfbyte *pSrc = (const zfbyte *)s;
        if(count == zfindexMax)
        {
            while(*pSrc && p < pEnd)
            {
                *p = *pSrc;
                ++p;
                ++pSrc;
            }
            if(autoAppendNullToken)
            {
                *p = '\0';
            }
            return p - (const zfbyte *)s;
        }
        else
        {
            const zfbyte *pSrcEnd = (const zfbyte *)s + count;
            while(*pSrc && pSrc < pSrcEnd && p < pEnd)
            {
                *p = *pSrc;
                ++p;
                ++pSrc;
            }
            if(autoAppendNullToken)
            {
                *p = '\0';
            }
            return p - (const zfbyte *)s;
        }
    }
    ZFMETHOD_DECLARE_2(zfindex, ioSeek, zfindex, byteSize, ZFSeekPos, pos)
    {
        switch(pos)
        {
            case ZFSeekPosBegin:
            {
                if(pEnd > pStart && pStart + byteSize > pEnd)
                {
                    p = pEnd;
                }
                else
                {
                    p = pStart + byteSize;
                }
            }
                break;
            case ZFSeekPosCur:
            {
                if(pEnd > pStart && p + byteSize > pEnd)
                {
                    p = pEnd;
                }
                else
                {
                    p += byteSize;
                }
            }
                break;
            case ZFSeekPosCurReversely:
            {
                if(p - byteSize < pStart)
                {
                    p = pStart;
                }
                else
                {
                    p -= byteSize;
                }
            }
                break;
            case ZFSeekPosEnd:
            {
                if(pEnd <= pStart)
                {
                    return zffalse;
                }
                if(pEnd - byteSize < pStart)
                {
                    p = pStart;
                }
                else
                {
                    p = pEnd - byteSize;
                }
            }
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zfindexMax;
        }
        return p - pStart;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    {
        return p - pStart;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
    {
        return ((pEnd > pStart) ? (pEnd - pStart) : zfindexMax);
    }
};
ZFOutputCallback _ZFP_ZFOutputCallbackForBuffer(ZF_IN const zfcharA *ownerFilePath,
                                                ZF_IN const zfcharA *ownerFunctionName,
                                                ZF_IN zfindex ownerFileLine,
                                                ZF_IN void *buf,
                                                ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                                ZF_IN_OPT zfbool autoAppendNullToken /* = zftrue */)
{
    if(buf == zfnull || maxCount == 0 || (maxCount == 1 && autoAppendNullToken))
    {
        return ZFCallbackNullDetail(ownerFilePath, ownerFunctionName, ownerFileLine);
    }
    _ZFP_ZFOutputCallbackForBufferOwner *owner = zfAlloc(_ZFP_ZFOutputCallbackForBufferOwner);
    owner->autoAppendNullToken = autoAppendNullToken;
    owner->pStart = (zfbyte *)buf;
    if(maxCount == zfindexMax)
    {
        owner->pEnd = zfnull;
        --(owner->pEnd);
    }
    else
    {
        owner->pEnd = owner->pStart + maxCount;
        if(autoAppendNullToken)
        {
            --(owner->pEnd);
        }
    }
    owner->p = owner->pStart;
    ZFOutputCallback ret = ZFCallbackForMemberMethodDetail(
        owner, ZFMethodAccess(_ZFP_ZFOutputCallbackForBufferOwner, onOutput),
        ownerFilePath, ownerFunctionName, ownerFileLine);
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);
    return ret;
}

// ============================================================
// ZFInputCallback
ZFBuffer ZFInputCallbackReadToBuffer(ZF_IN_OUT const ZFInputCallback &input)
{
    if(!input.callbackIsValid())
    {
        return ZFBuffer();
    }

    ZFBuffer ret;
    zfindex totalSize = input.ioSize();
    if(totalSize != zfindexMax)
    {
        ret.bufferMalloc(totalSize + sizeof(zfcharW));
        if(input.execute(ret.buffer(), totalSize) != totalSize)
        {
            ret.bufferFree();
        }
        else
        {
            ret.bufferSizeSet(totalSize);
        }
    }
    else
    {
        #define _ZFP_ZFInputCallbackReadToBuffer_blockSize 256
        zfindex readCount = 0;
        zfindex size = 0;
        do
        {
            ret.bufferRealloc(ret.bufferSize() + _ZFP_ZFInputCallbackReadToBuffer_blockSize);
            readCount = input.execute(ret.bufferT<zfbyte *>() + size, _ZFP_ZFInputCallbackReadToBuffer_blockSize - sizeof(zfcharW));
            size += readCount;
            if(readCount < _ZFP_ZFInputCallbackReadToBuffer_blockSize - sizeof(zfcharW))
            {
                ret.bufferRealloc(size + sizeof(zfcharW));
                ret.bufferSizeSet(size);
                break;
            }
        } while(zftrue);
        #undef _ZFP_ZFInputCallbackReadToBuffer_blockSize
    }
    return ret;
}
zfindex ZFInputCallbackReadToOutput(ZF_IN_OUT const ZFInputCallback &input,
                                    ZF_IN_OUT const ZFOutputCallback &output)
{
    zfindex size = 0;
    if(input.callbackIsValid() && output.callbackIsValid())
    {
        #define _ZFP_ZFInputCallbackReadToOutput_blockSize 256
        zfchar buf[_ZFP_ZFInputCallbackReadToOutput_blockSize] = {0};
        zfindex readCount = 0;
        zfindex writeCount = 0;
        do
        {
            readCount = input.execute(buf, _ZFP_ZFInputCallbackReadToOutput_blockSize);
            writeCount = output.execute(buf, readCount);
            size += writeCount;
            if(readCount < _ZFP_ZFInputCallbackReadToOutput_blockSize || writeCount < readCount)
            {
                break;
            }
        } while(zftrue);
        #undef _ZFP_ZFInputCallbackReadToOutput_blockSize
    }
    return size;
}

zfchar ZFInputCallbackSkipChars(ZF_IN_OUT const ZFInputCallback &input,
                                ZF_IN_OPT const zfchar *charSet /* = zfText(" \t\r\n") */)
{
    zfchar ret = '\0';
    if(input.callbackIsValid())
    {
        zfindex charSetCount = zfslen(charSet);
        zfbool matched = zffalse;
        do
        {
            if(input.execute(&ret, 1) < 1)
            {
                break;
            }
            matched = zffalse;
            for(zfindex i = 0; i < charSetCount; ++i)
            {
                if(charSet[i] == ret)
                {
                    matched = zftrue;
                    break;
                }
            }
        } while(matched);
    }
    return ret;
}
zfindex ZFInputCallbackReadUntil(ZF_IN_OUT zfstring &ret,
                                 ZF_IN_OUT const ZFInputCallback &input,
                                 ZF_IN_OPT const zfchar *charSet /* = zfText(" \t\r\n") */,
                                 ZF_IN_OPT zfindex maxCount /* = zfindexMax */,
                                 ZF_OUT_OPT zfchar *firstCharMatchedCharSet /* = zfnull */)
{
    zfindex readCount = 0;
    if(firstCharMatchedCharSet != zfnull)
    {
        *firstCharMatchedCharSet = '\0';
    }
    if(input.callbackIsValid())
    {
        zfchar c = 0;
        zfindex charSetCount = zfslen(charSet);
        zfbool matched = zffalse;
        while(readCount < maxCount && input.execute(&c, 1) == 1)
        {
            matched = zffalse;
            for(zfindex i = 0; i < charSetCount; ++i)
            {
                if(charSet[i] == c)
                {
                    matched = zftrue;
                    break;
                }
            }
            if(matched)
            {
                if(firstCharMatchedCharSet != zfnull)
                {
                    *firstCharMatchedCharSet = c;
                }
                break;
            }
            ret += c;
            ++readCount;
        }
    }
    return readCount;
}
zfindex ZFInputCallbackCheckMatch(ZF_IN const zfchar **tokens,
                                  ZF_IN zfindex tokenCount,
                                  ZF_IN_OUT const ZFInputCallback &input)
{
    zfindex ret = zfindexMax;
    if(input.callbackIsValid())
    {
        zfindex saved = input.ioTell();
        zfindex maxLen = 0;
        for(zfindex i = 0; i < tokenCount; ++i)
        {
            maxLen = zfmMax(maxLen, zfslen(tokens[i]));
        }
        zfchar *buf = (zfchar *)zfmalloc(sizeof(zfchar) * maxLen);
        zfblockedFree(buf);

        buf[0] = ZFInputCallbackSkipChars(input);
        input.execute(buf + 1, maxLen - 1);
        zfbool matched = zffalse;
        zfindex matchedLen = 0;
        for(zfindex i = 0; i < tokenCount; ++i)
        {
            if(zfsncmp(tokens[i], buf, zfslen(tokens[i])) == 0)
            {
                matched = zftrue;
                matchedLen = zfslen(tokens[i]);
                ret = i;
                break;
            }
        }
        if(matched)
        {
            input.ioSeek(maxLen - matchedLen, ZFSeekPosCurReversely);
        }
        else
        {
            input.ioSeek(saved, ZFSeekPosBegin);
        }
    }
    return ret;
}
zfbool ZFInputCallbackReadDataPair(ZF_OUT ZFCoreArray<zfstring> &outData,
                                   ZF_IN_OUT const ZFInputCallback &input,
                                   ZF_IN zfindex desiredCount /* = zfindexMax */,
                                   ZF_IN_OPT zfchar leftToken /* = '(' */,
                                   ZF_IN_OPT zfchar rightToken /* = ')' */,
                                   ZF_IN_OPT const zfstring &separatorTokens /* = zfText(",") */)
{
    if(!input.callbackIsValid()) {return zffalse;}
    zfbool success = zffalse;
    do
    {
        zfchar p = ZFInputCallbackSkipChars(input);
        if(p != leftToken) {break;}

        zfstring element;
        p = ZFInputCallbackSkipChars(input);
        zfbool hasSeparatorToken = zffalse;
        zfindex elementCount = 0;
        zfindex tokenCount = 0;
        do
        {
            if(p == '\0')
            {
                break;
            }
            else if(zfstringFindFirstOf(separatorTokens, p) != zfindexMax)
            {
                hasSeparatorToken = zftrue;
                if(element.isEmpty())
                {
                    break;
                }
                else
                {
                    outData.add(element);
                    ++elementCount;
                    element.removeAll();
                    p = ZFInputCallbackSkipChars(input);
                }
            }
            else if(p == leftToken)
            {
                ++tokenCount;
                element += p;
                if(input.execute(&p, 1) < 1)
                {
                    break;
                }
            }
            else if(p == rightToken)
            {
                if(tokenCount > 0)
                {
                    --tokenCount;
                    element += p;
                    if(input.execute(&p, 1) < 1)
                    {
                        break;
                    }
                }
                else
                {
                    if(hasSeparatorToken && element.isEmpty())
                    {
                        p = '\0';
                    }
                    else if(!element.isEmpty())
                    {
                        outData.add(element);
                        ++elementCount;
                    }
                    break;
                }
            }
            else
            {
                element += p;
                if(input.execute(&p, 1) < 1)
                {
                    break;
                }
            }
        } while(zftrue);
        if(p != rightToken) {break;}
        if(desiredCount != zfindexMax && elementCount != desiredCount) {break;}

        success = zftrue;
        break;
    } while(zffalse);
    if(!success)
    {
        input.ioSeek(1, ZFSeekPosCurReversely);
    }
    return success;
}

// ============================================================
// ZFInputCallbackForInputInRange
zfclass _ZFP_ZFInputCallbackForInputInRangeOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFInputCallbackForInputInRangeOwner, ZFObject)
public:
    ZFInputCallback src;
    zfindex srcStart;
    zfindex srcCount;
    zfbool autoRestorePos;
    zfindex savedPos;
    zfindex curPos; // ensured init with start

public:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        if(autoRestorePos)
        {
            src.ioSeek(savedPos);
        }
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput, void *, buf, zfindex, count)
    {
        if(buf == zfnull)
        {
            return 0;
        }
        if(curPos - srcStart + count > srcCount)
        {
            count = srcCount - curPos + srcStart;
        }
        count = src.execute(buf, count);
        curPos += count;
        return count;
    }
    ZFMETHOD_DECLARE_2(zfindex, ioSeek, zfindex, byteSize, ZFSeekPos, pos)
    {
        curPos = ZFIOCallbackCalcFSeek(srcStart, srcCount, curPos, byteSize, pos);
        return curPos;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    {
        return curPos - srcStart;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
    {
        return srcStart + srcCount - curPos;
    }
};
ZFInputCallback _ZFP_ZFInputCallbackForInputInRange(ZF_IN const zfcharA *ownerFilePath,
                                                    ZF_IN const zfcharA *ownerFunctionName,
                                                    ZF_IN zfindex ownerFileLine,
                                                    ZF_IN const ZFInputCallback &inputCallback,
                                                    ZF_IN_OPT zfindex start /* = 0 */,
                                                    ZF_IN_OPT zfindex count /* = zfindexMax */,
                                                    ZF_IN_OPT zfbool autoRestorePos /* = zftrue */)
{
    zfbool valid = zffalse;
    zfindex savedPos = zfindexMax;
    zfindex countFixed = count;
    do
    {
        if(!inputCallback.callbackIsValid()) {break;}

        savedPos = inputCallback.ioTell();
        if(savedPos == zfindexMax) {break;}

        if(inputCallback.ioSeek(start, ZFSeekPosBegin) != start) {break;}

        zfindex srcCount = inputCallback.ioSize();
        if(srcCount == zfindexMax) {break;}
        if(start + countFixed > srcCount)
        {
            countFixed = srcCount - start;
        }

        valid = zftrue;
    } while(zffalse);
    if(!valid)
    {
        if(savedPos != zfindexMax)
        {
            inputCallback.ioSeek(savedPos, ZFSeekPosBegin);
        }
        return ZFCallbackNullDetail(ownerFilePath, ownerFunctionName, ownerFileLine);
    }

    _ZFP_ZFInputCallbackForInputInRangeOwner *owner = zfAlloc(_ZFP_ZFInputCallbackForInputInRangeOwner);
    owner->src = inputCallback;
    owner->srcStart = start;
    owner->srcCount = countFixed;
    owner->autoRestorePos = autoRestorePos;
    owner->savedPos = savedPos;
    owner->curPos = start;
    ZFInputCallback ret = ZFCallbackForMemberMethodDetail(
        owner, ZFMethodAccess(_ZFP_ZFInputCallbackForInputInRangeOwner, onInput),
        ownerFilePath, ownerFunctionName, ownerFileLine);
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    zfRelease(owner);

    if(inputCallback.callbackId() != zfnull)
    {
        ret.callbackIdSet(zfstringWithFormat(zfText("ZFInputCallbackForInputInRange[%zi, %zi]:%@"), start, count, inputCallback.callbackId()));
    }

    if(inputCallback.callbackSerializeCustomType() != zfnull)
    {
        ZFSerializableData inputData;
        if(ZFCallbackToSerializableData(inputData, inputCallback))
        {
            ZFSerializableData customData;
            customData.itemClassSet(ZFSerializableKeyword_node);

            zfbool success = zffalse;
            do {
                inputData.categorySet(ZFSerializableKeyword_ZFInputCallbackForInputInRange_input);
                customData.elementAdd(inputData);

                if(start != 0)
                {
                    ZFSerializableData startData;
                    if(!zfindexToSerializableData(startData, start))
                    {
                        break;
                    }
                    startData.categorySet(ZFSerializableKeyword_ZFInputCallbackForInputInRange_start);
                    customData.elementAdd(startData);
                }

                if(count != zfindexMax)
                {
                    ZFSerializableData countData;
                    if(!zfindexToSerializableData(countData, count))
                    {
                        break;
                    }
                    countData.categorySet(ZFSerializableKeyword_ZFInputCallbackForInputInRange_count);
                    customData.elementAdd(countData);
                }

                if(!autoRestorePos)
                {
                    ZFSerializableData autoRestorePosData;
                    if(!zfboolToSerializableData(autoRestorePosData, autoRestorePos))
                    {
                        break;
                    }
                    autoRestorePosData.categorySet(ZFSerializableKeyword_ZFInputCallbackForInputInRange_autoRestorePos);
                    customData.elementAdd(autoRestorePosData);
                }

                success = zftrue;
            } while(zffalse);

            if(success)
            {
                ret.callbackSerializeCustomTypeSet(ZFCallbackSerializeCustomTypeName_ZFInputCallbackForInputInRange);
                ret.callbackSerializeCustomDataSet(customData);
            }
        }
    }

    return ret;
}
ZFCALLBACK_SERIALIZE_CUSTOM_TYPE_DEFINE(ZFCallbackSerializeCustomType_ZFInputCallbackForInputInRange)
{
    const ZFSerializableData *inputData = ZFSerializableUtil::requireElementByCategory(
        serializableData, ZFSerializableKeyword_ZFInputCallbackForInputInRange_input, outErrorHint, outErrorPos);
    if(inputData == zfnull)
    {
        return zffalse;
    }
    ZFCallback input;
    if(!ZFCallbackFromSerializableData(input, *inputData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }

    zfindex start = 0;
    {
        const ZFSerializableData *startData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputCallbackForInputInRange_start);
        if(startData != zfnull && !zfindexFromSerializableData(start, *startData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    zfindex count = zfindexMax;
    {
        const ZFSerializableData *countData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputCallbackForInputInRange_count);
        if(countData != zfnull && !zfindexFromSerializableData(count, *countData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    zfbool autoRestorePos = zftrue;
    {
        const ZFSerializableData *autoRestorePosData = ZFSerializableUtil::checkElementByCategory(serializableData, ZFSerializableKeyword_ZFInputCallbackForInputInRange_autoRestorePos);
        if(autoRestorePosData != zfnull && !zfboolFromSerializableData(autoRestorePos, *autoRestorePosData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
    }
    serializableData.resolveMark();

    result = ZFInputCallbackForInputInRange(input, start, count, autoRestorePos);
    return zftrue;
}

// ============================================================
// ZFInputCallbackForBuffer
zfclass _ZFP_ZFInputCallbackForBufferOwner : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFInputCallbackForBufferOwner, ZFObject)

public:
    zfbool isCopy;
    const zfbyte *pStart;
    const zfbyte *pEnd; // ensured valid
    const zfbyte *p;
public:
    ZFMETHOD_DECLARE_2(zfindex, onInput, void *, buf, zfindex, count)
    {
        if(buf == zfnull)
        {
            return 0;
        }
        if(p + count > pEnd)
        {
            count = pEnd - p;
        }
        zfmemcpy(buf, p, count);
        p += count;
        return count;
    }
    ZFMETHOD_DECLARE_2(zfindex, ioSeek, zfindex, byteSize, ZFSeekPos, pos)
    {
        const zfbyte *nullAddr = zfnull;
        p = nullAddr + ZFIOCallbackCalcFSeek(pStart - nullAddr, pEnd - nullAddr, p - nullAddr, byteSize, pos);
        return p - pStart;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioTell)
    {
        return p - pStart;
    }
    ZFMETHOD_DECLARE_0(zfindex, ioSize)
    {
        return pEnd - p;
    }
};
ZFInputCallback _ZFP_ZFInputCallbackForBuffer(ZF_IN const zfcharA *ownerFilePath,
                                              ZF_IN const zfcharA *ownerFunctionName,
                                              ZF_IN zfindex ownerFileLine,
                                              ZF_IN zfbool copy,
                                              ZF_IN const void *src,
                                              ZF_IN_OPT zfindex count /* = zfindexMax */)
{
    if(src == zfnull)
    {
        return ZFCallbackNullDetail(ownerFilePath, ownerFunctionName, ownerFileLine);
    }
    zfindex len = count;
    void *srcTmp = zfnull;
    if(len == zfindexMax)
    {
        len = zfslen((const zfchar *)src);
    }
    if(copy)
    {
        srcTmp = zfmalloc(sizeof(zfchar) * len);
        zfmemcpy(srcTmp, src, sizeof(zfchar) * len);
        src = srcTmp;
    }
    if(src == zfnull)
    {
        return ZFCallbackNullDetail(ownerFilePath, ownerFunctionName, ownerFileLine);
    }

    _ZFP_ZFInputCallbackForBufferOwner *owner = zfAlloc(_ZFP_ZFInputCallbackForBufferOwner);
    owner->isCopy = copy;
    owner->pStart = (const zfbyte *)src;
    owner->pEnd = owner->pStart + len;
    owner->p = owner->pStart;
    ZFInputCallback ret = ZFCallbackForMemberMethodDetail(
        owner, ZFMethodAccess(_ZFP_ZFInputCallbackForBufferOwner, onInput),
        ownerFilePath, ownerFunctionName, ownerFileLine);
    ret.callbackTagSet(ZFCallbackTagKeyword_ioOwner, owner);
    if(copy)
    {
        ret.callbackTagSet(
            zfText("ZFInputCallbackForBufferCopiedBuffer"),
            zflineAlloc(ZFTypeHolder, srcTmp, ZFTypeHolder::DeletePOD));

    }
    zfRelease(owner);
    return ret;
}

// ============================================================
// ZFIOBridgeCallbackAbs

// ============================================================
// ZFIOBridgeCallbackUsingBuffer
zfclass _ZFP_ZFIOBridgeCallbackUsingBufferPrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(_ZFP_ZFIOBridgeCallbackUsingBufferPrivate, ZFObject)

public:
    ZFCoreArrayPOD<zfbyte> ioBuf;
    zfindex inputIndex;

public:
    _ZFP_ZFIOBridgeCallbackUsingBufferPrivate(void)
    : ioBuf()
    , inputIndex(0)
    {
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput, void *, buf, zfindex, count)
    {
        if(buf == zfnull)
        {
            return this->ioBuf.count();
        }
        else
        {
            count = zfmMin(count, this->ioBuf.count() - this->inputIndex);
            zfmemcpy(buf, this->ioBuf.arrayBuf() + this->inputIndex, count);
            this->inputIndex = zfmMin(this->inputIndex + count, this->ioBuf.count());
            return count;
        }
    }
    ZFMETHOD_DECLARE_2(zfindex, onOutput, const void *, buf, zfindex, count)
    {
        if(count == 0)
        {
            return count;
        }
        if(count == zfindexMax)
        {
            count = zfslen((const zfchar *)buf) * sizeof(zfchar);
        }
        this->ioBuf.addFrom(ZFCastStatic(const zfbyte *, buf), count);
        return count;
    }
};

/** @cond ZFPrivateDoc */
ZFIOBridgeCallbackUsingBuffer::ZFIOBridgeCallbackUsingBuffer(void)
: ZFIOBridgeCallbackAbs()
{
    d = zfAllocWithoutLeakTest(_ZFP_ZFIOBridgeCallbackUsingBufferPrivate);
}
ZFIOBridgeCallbackUsingBuffer::ZFIOBridgeCallbackUsingBuffer(ZF_IN const ZFIOBridgeCallbackUsingBuffer &ref)
: ZFIOBridgeCallbackAbs(ref)
{
    d = zfnull;
    this->operator=(ref);
}
ZFIOBridgeCallbackUsingBuffer &ZFIOBridgeCallbackUsingBuffer::operator =(ZF_IN const ZFIOBridgeCallbackUsingBuffer &ref)
{
    zfRetainWithoutLeakTest(ref.d);
    zfReleaseWithoutLeakTest(d);
    d = ref.d;
    return *this;
}
/** @endcond */
ZFIOBridgeCallbackUsingBuffer::~ZFIOBridgeCallbackUsingBuffer(void)
{
    zfReleaseWithoutLeakTest(d);
    d = zfnull;
}

ZFInputCallback ZFIOBridgeCallbackUsingBuffer::inputCallback(void)
{
    return ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBridgeCallbackUsingBufferPrivate, onInput));
}
ZFOutputCallback ZFIOBridgeCallbackUsingBuffer::outputCallback(void)
{
    return ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBridgeCallbackUsingBufferPrivate, onOutput));
}

void ZFIOBridgeCallbackUsingBuffer::resetInput(void)
{
    d->inputIndex = 0;
}
void ZFIOBridgeCallbackUsingBuffer::resetOutput(void)
{
    d->inputIndex = 0;
    d->ioBuf.removeAll();
}

ZF_NAMESPACE_GLOBAL_END

