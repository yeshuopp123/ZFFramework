/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFTextTemplate.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassLikePOD ZF_ENV_EXPORT _ZFP_ZFTextTemplateIndexDataState
{
public:
    const ZFTextTemplateIndexData *indexData;
    zfindex indexCur;

public:
    _ZFP_ZFTextTemplateIndexDataState(void)
    : indexData(zfnull)
    , indexCur(0)
    {
    }
};

static const zfchar *_ZFP_ZFTextTemplate_header = zfText("{ZFTT_");
static const zfindex _ZFP_ZFTextTemplate_headerSize = zfslen(_ZFP_ZFTextTemplate_header);
static void _ZFP_ZFTextTemplateApply_replaceData(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN_OUT ZFCoreMap &stateMap,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const zfchar *pEnd,
                                                 ZF_IN_OUT const zfchar *&data,
                                                 ZF_IN_OUT const zfchar *&p,
                                                 ZF_IN_OUT zfindex &size);
static void _ZFP_ZFTextTemplateApply_enableData(ZF_IN const ZFTextTemplateParam &param,
                                                ZF_IN_OUT ZFCoreMap &stateMap,
                                                ZF_IN const ZFOutputCallback &output,
                                                ZF_IN const zfchar *pEnd,
                                                ZF_IN_OUT const zfchar *&data,
                                                ZF_IN_OUT const zfchar *&p,
                                                ZF_IN_OUT zfindex &size,
                                                ZF_IN_OUT zfindex &condCount);
static void _ZFP_ZFTextTemplateApply_indexData(ZF_IN const ZFTextTemplateParam &param,
                                               ZF_IN_OUT ZFCoreMap &stateMap,
                                               ZF_IN const ZFOutputCallback &output,
                                               ZF_IN const zfchar *pEnd,
                                               ZF_IN_OUT const zfchar *&data,
                                               ZF_IN_OUT const zfchar *&p,
                                               ZF_IN_OUT zfindex &size);

// ============================================================
zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                            ZF_IN const ZFOutputCallback &output,
                            ZF_IN const ZFInputCallback &input)
{
    ZFBuffer buffer = ZFInputCallbackReadToBuffer(input);
    if(buffer.buffer() == zfnull)
    {
        return zfindexMax;
    }
    return ZFTextTemplateApply(param, output, buffer.bufferAsString(), buffer.bufferAsStringLength());
}

zfindex ZFTextTemplateApply(ZF_IN const ZFTextTemplateParam &param,
                            ZF_IN const ZFOutputCallback &output,
                            ZF_IN const zfchar *data,
                            ZF_IN_OPT zfindex dataSize /* = zfindexMax */)
{
    if(data == zfnull)
    {
        return zfindexMax;
    }

    zfindex condCount = 0;
    zfindex size = 0;
    const zfchar *p = data;
    const zfchar *pEnd = (data + ((dataSize == zfindexMax) ? zfslen(data) : dataSize));
    ZFCoreMap stateMap;
    do
    {
        if(p >= pEnd)
        {
            if(p > data)
            {
                size += p - data;
                if(output.callbackIsValid())
                {
                    output.execute(data, p - data);
                }
            }
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_headerSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_header, _ZFP_ZFTextTemplate_headerSize) == 0
            ))
        {
            ++p;
            continue;
        }
        if(p > data)
        {
            size += p - data;
            if(output.callbackIsValid())
            {
                output.execute(data, p - data);
            }
            data = p;
        }

        switch(p[_ZFP_ZFTextTemplate_headerSize])
        {
            case 'R':
                p += _ZFP_ZFTextTemplate_headerSize + 1;
                _ZFP_ZFTextTemplateApply_replaceData(param, stateMap, output, pEnd, data, p, size);
                break;
            case 'C':
                p += _ZFP_ZFTextTemplate_headerSize + 1;
                _ZFP_ZFTextTemplateApply_enableData(param, stateMap, output, pEnd, data, p, size, condCount);
                break;
            case 'I':
                p += _ZFP_ZFTextTemplate_headerSize + 1;
                _ZFP_ZFTextTemplateApply_indexData(param, stateMap, output, pEnd, data, p, size);
                break;
            default:
                ++p;
                break;
        }
    } while(zftrue);
    return size;
}

// ============================================================
static zfindex _ZFP_ZFTextTemplateApply_keyLength(ZF_IN const zfchar *pEnd,
                                                  ZF_IN const zfchar *p)
{
    if(*p == ' ' || *p == '\t' || *p == '}')
    {
        return zfindexMax;
    }
    const zfchar *t = p;
    while(t < pEnd && *t != '}') {++t;}
    if(t >= pEnd)
    {
        return zfindexMax;
    }
    return (t - p);
}
// ============================================================
static void _ZFP_ZFTextTemplateApply_replaceData(ZF_IN const ZFTextTemplateParam &param,
                                                 ZF_IN_OUT ZFCoreMap &stateMap,
                                                 ZF_IN const ZFOutputCallback &output,
                                                 ZF_IN const zfchar *pEnd,
                                                 ZF_IN_OUT const zfchar *&data,
                                                 ZF_IN_OUT const zfchar *&p,
                                                 ZF_IN_OUT zfindex &size)
{ // {ZFTT_R_myKey}
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    const zfchar *value = param.replaceData(key);
    if(value == zfnull)
    {
        return ;
    }
    zfindex valueLen = zfslen(value);
    if(output.callbackIsValid())
    {
        output.execute(value, valueLen);
    }
    size += valueLen;
    data = p;
}
static void _ZFP_ZFTextTemplateApply_enableData(ZF_IN const ZFTextTemplateParam &param,
                                                ZF_IN_OUT ZFCoreMap &stateMap,
                                                ZF_IN const ZFOutputCallback &output,
                                                ZF_IN const zfchar *pEnd,
                                                ZF_IN_OUT const zfchar *&data,
                                                ZF_IN_OUT const zfchar *&p,
                                                ZF_IN_OUT zfindex &size,
                                                ZF_IN_OUT zfindex &condCount)
{ // {ZFTT_C_myCond}...{ZFTT_CE}
    if(pEnd - p >= 2 && p[0] == 'E' && p[1] == '}')
    {
        p += 2;
        if(condCount > 0)
        {
            --condCount;
            data = p;
        }
        return ;
    }
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    data = p;
    const zfbool *valueTmp = param.enableData(key);
    zfbool value = (valueTmp ? *valueTmp : param.enableDataDefault());
    if(value)
    {
        ++condCount;
        return ;
    }

    zfindex condCountOffset = 0;
    do
    {
        if(p >= pEnd)
        {
            data = p;
            break;
        }
        if(!(
                (zfindex)(pEnd - p) > _ZFP_ZFTextTemplate_headerSize
                && zfsncmp(p, _ZFP_ZFTextTemplate_header, _ZFP_ZFTextTemplate_headerSize) == 0
            ))
        {
            ++p;
            continue;
        }

        p += _ZFP_ZFTextTemplate_headerSize;
        if(*p != 'C')
        {
            continue;
        }
        ++p;
        if(pEnd - p >= 2 && p[0] == 'E' && p[1] == '}')
        {
            p += 2;
            if(condCountOffset == 0)
            {
                data = p;
                break;
            }
            else
            {
                --condCountOffset;
                continue;
            }
        }
        if(*p != '_')
        {
            continue;
        }
        ++p;
        zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
        if(keySize == zfindexMax)
        {
            continue;
        }
        ++condCountOffset;
        p += keySize + 1;
    } while(zftrue);
}
// ============================================================
static void _ZFP_ZFTextTemplateApply_indexData_reset(ZF_IN const ZFTextTemplateParam &param,
                                                     ZF_IN_OUT ZFCoreMap &stateMap,
                                                     ZF_IN const ZFOutputCallback &output,
                                                     ZF_IN const zfchar *pEnd,
                                                     ZF_IN_OUT const zfchar *&data,
                                                     ZF_IN_OUT const zfchar *&p,
                                                     ZF_IN_OUT zfindex &size);
static void _ZFP_ZFTextTemplateApply_indexData(ZF_IN const ZFTextTemplateParam &param,
                                               ZF_IN_OUT ZFCoreMap &stateMap,
                                               ZF_IN const ZFOutputCallback &output,
                                               ZF_IN const zfchar *pEnd,
                                               ZF_IN_OUT const zfchar *&data,
                                               ZF_IN_OUT const zfchar *&p,
                                               ZF_IN_OUT zfindex &size)
{ // {ZFTT_I_myKey}
    if(*p == 'R')
    {
        ++p;
        _ZFP_ZFTextTemplateApply_indexData_reset(param, stateMap, output, pEnd, data, p, size);
        return ;
    }
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;

    zfstring value;
    {
        zfstring indexDataStateKey = zfstringWithFormat(zfText("indexData:%s"), key.cString());
        _ZFP_ZFTextTemplateIndexDataState *indexDataState = stateMap.get<_ZFP_ZFTextTemplateIndexDataState *>(indexDataStateKey);
        if(indexDataState == zfnull)
        {
            indexDataState = zfnew(_ZFP_ZFTextTemplateIndexDataState);
            indexDataState->indexData = param.indexData(key);
            if(indexDataState->indexData == zfnull)
            {
                indexDataState->indexData = &(param.indexDataDefault());
            }

            indexDataState->indexCur = indexDataState->indexData->indexStart;

            stateMap.set(indexDataStateKey, ZFCorePointerForObject<_ZFP_ZFTextTemplateIndexDataState *>(indexDataState));
        }

        zfstring fmt;
        if(indexDataState->indexData->indexWidth == 0)
        {
            fmt = zfText("%s");
        }
        else
        {
            switch(indexDataState->indexData->indexFlag)
            {
                case ZFTextTemplateIndexFlag::e_LeadingZero:
                    zfstringAppend(fmt, zfText("%%0%zis"), indexDataState->indexData->indexWidth);
                    break;
                case ZFTextTemplateIndexFlag::e_LeadingSpace:
                    zfstringAppend(fmt, zfText("%%%zis"), indexDataState->indexData->indexWidth);
                    break;
                case ZFTextTemplateIndexFlag::e_TailSpace:
                    zfstringAppend(fmt, zfText("%%-%zis"), indexDataState->indexData->indexWidth);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return ;
            }
        }

        zfstringAppend(value, fmt.cString(), zfsFromInt(
                indexDataState->indexCur,
                indexDataState->indexData->indexRadix,
                indexDataState->indexData->indexUpperCase
            ).cString());

        zfCoreAssertWithMessage(indexDataState->indexData->indexOffset, zfText("indexOffset must not be 0"));
        indexDataState->indexCur += indexDataState->indexData->indexOffset;
    }

    if(output.callbackIsValid())
    {
        output.execute(value.cString(), value.length());
    }
    size += value.length();
    data = p;
}
static void _ZFP_ZFTextTemplateApply_indexData_reset(ZF_IN const ZFTextTemplateParam &param,
                                                     ZF_IN_OUT ZFCoreMap &stateMap,
                                                     ZF_IN const ZFOutputCallback &output,
                                                     ZF_IN const zfchar *pEnd,
                                                     ZF_IN_OUT const zfchar *&data,
                                                     ZF_IN_OUT const zfchar *&p,
                                                     ZF_IN_OUT zfindex &size)
{ // {ZFTT_IR_myKey}
    if(*p != '_')
    {
        return ;
    }
    ++p;
    zfindex keySize = _ZFP_ZFTextTemplateApply_keyLength(pEnd, p);
    if(keySize == zfindexMax)
    {
        return ;
    }
    zfstring key(p, keySize);
    p += keySize + 1;
    data = p;

    zfstring indexDataStateKey = zfstringWithFormat(zfText("indexData:%s"), key.cString());
    stateMap.remove(indexDataStateKey);
}

ZF_NAMESPACE_GLOBAL_END

