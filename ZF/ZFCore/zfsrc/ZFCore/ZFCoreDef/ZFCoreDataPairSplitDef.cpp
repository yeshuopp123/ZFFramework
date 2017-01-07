/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreDataPairSplitDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

const zfchar *zfCoreDataPairSplitString(ZF_OUT ZFCoreArray<zfindexRange> &outData,
                                        ZF_IN zfindex desiredCountOrIndexMax,
                                        ZF_IN const zfchar *src,
                                        ZF_IN_OPT zfindex srcLen /* = zfindexMax */,
                                        ZF_IN_OPT zfchar leftToken /* = '(' */,
                                        ZF_IN_OPT zfchar rightToken /* = ')' */,
                                        ZF_IN_OPT const zfstring &separatorTokens /* = zfText(",") */,
                                        ZF_IN_OPT zfbool allowEmptyItem /* = zffalse */)
{
    if(src == zfnull) {return zfText("");}
    const zfchar *p = src;
    const zfchar *srcEnd = ((srcLen == zfindexMax) ? (src + zfslen(src)) : (src + srcLen));

    // leftToken
    zfcharSkipSpace(p, srcEnd);
    if(leftToken != '\0')
    {
        if(*p != leftToken) {return p;}
        ++p;
    }

    // each item
    zfindex count = 0;
    do
    {
        zfindex itemTokenCount = 0;

        // skip item header space
        zfcharSkipSpace(p, srcEnd);

        // find item end pos
        const zfchar *tmpPosL = p;
        const zfchar *tmpPosR = p;
        for( ; tmpPosR < srcEnd; ++tmpPosR)
        {
            if(zfstringFindFirstOf(separatorTokens, *tmpPosR) != zfindexMax)
            {
                if(itemTokenCount == 0)
                {
                    break;
                }
            }
            else
            {
                if(!zfcharIsSpace(*tmpPosR) && *tmpPosR != rightToken)
                {
                    tmpPosL = tmpPosR;
                }

                if(*tmpPosR == leftToken)
                {
                    ++itemTokenCount;
                }
                else if(*tmpPosR == rightToken)
                {
                    if(itemTokenCount > 0)
                    {
                        tmpPosL = tmpPosR;
                        --itemTokenCount;
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
        if(itemTokenCount != 0)
        {
            return tmpPosR;
        }
        if(tmpPosL != tmpPosR)
        {
            ++tmpPosL;
        }

        if(!allowEmptyItem && tmpPosL <= p) {return tmpPosL;}

        // save item
        outData.add(zfindexRangeMake(p - src, tmpPosL - p));
        ++count;

        // end item
        if(*tmpPosR == rightToken || tmpPosR >= srcEnd) {p = tmpPosR; break;}
        p = tmpPosR + 1;
    } while(count < desiredCountOrIndexMax);

    // count check
    if(desiredCountOrIndexMax != zfindexMax && count != desiredCountOrIndexMax)
    {
        return p;
    }

    // rightToken
    if(rightToken != '\0')
    {
        if(p == srcEnd || *p != rightToken)
        {
            return p;
        }
        ++p;
    }

    // tail check
    zfcharSkipSpace(p, srcEnd);
    if(p < srcEnd)
    {
        return p;
    }

    // success
    return zfnull;
}

ZF_NAMESPACE_GLOBAL_END

