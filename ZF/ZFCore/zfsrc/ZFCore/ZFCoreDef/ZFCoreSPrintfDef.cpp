/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFCoreSPrintfDef.h"
#include "ZFCoreUtilDef.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_zfstringAppendPrecisionNone (zfindexMax - 1)
#define _ZFP_zfstringAppendFlag_alignLeft 1
#define _ZFP_zfstringAppendFlag_leadingZero 2
zfclassLikePOD ZF_ENV_EXPORT _ZFP_zfstringAppendFlag
{
public:
    zfflags flags;
    zfindex width; // zfindexMax to scan from vaList
    zfindex precision; // zfindexMax to scan from vaList,
                       // or _ZFP_zfstringAppendPrecisionNone if none
    zfcharA positiveToken; // "+" or "-" or " ", or '\0' if none
    zfbool success;

public:
    _ZFP_zfstringAppendFlag(void)
    : flags(0)
    , width(0)
    , precision(_ZFP_zfstringAppendPrecisionNone)
    , positiveToken('\0')
    , success(zftrue)
    {
    }

public:
    zfbool hasFlag(void)
    {
        return (zffalse
                || flags != 0
                || width != 0
                || precision != _ZFP_zfstringAppendPrecisionNone
                || positiveToken != '\0'
            );
    }
};
static inline void _ZFP_zfstringAppend_appendToken(ZF_IN_OUT ZFCoreStringA &s, ZF_IN const zfchar *token)
{
    s += zfsCoreZ2A(token);
}
static inline void _ZFP_zfstringAppend_appendToken(ZF_IN_OUT ZFCoreStringW &s, ZF_IN const zfchar *token)
{
    s += zfsCoreZ2W(token);
}
#define _ZFP_zfstringAppend_action(ret_, flags_, p_, vaList_) \
    do { \
        switch(*p_) \
        { \
            case 'b': \
                _ZFP_zfstringAppend_appendToken(ret_, \
                    (va_arg(vaList_, zfint) != 0) ? ZFTOKEN_zfbool_zftrue : ZFTOKEN_zfbool_zffalse); \
                break; \
            case 'z': \
                if(*(p_+1) == 'i') \
                { \
                    ++p_; \
                    zfsFromIntT(ret_, va_arg(vaList_, zfindex), 10); \
                } \
                else \
                { \
                    flags_.success = zffalse; \
                } \
                break; \
            case 'd': \
            case 'i': \
            { \
                zfint v = va_arg(vaList_, zfint); \
                if(v < 0 && flags_.hasFlag()) \
                { \
                    flags_.positiveToken = '-'; \
                    v = -v; \
                } \
                zfsFromIntT(ret_, v, 10); \
            } \
                break; \
            case 'u': \
                zfsFromIntT(ret_, va_arg(vaList_, zfuint), 10); \
                break; \
            case 'o': \
                zfsFromIntT(ret_, va_arg(vaList_, zfuint), 8); \
                break; \
            case 'x': \
                zfsFromIntT(ret_, va_arg(vaList_, zfuint), 16, zffalse); \
                break; \
            case 'X': \
                zfsFromIntT(ret_, va_arg(vaList_, zfuint), 16); \
                break; \
            case 'f': \
            { \
                zffloat v = (zffloat)va_arg(vaList_, double); \
                if(v < 0 && flags_.hasFlag()) \
                { \
                    flags_.positiveToken = '-'; \
                    v = -v; \
                } \
                zfsFromFloatT(ret_, v); \
            } \
                break; \
            case 'l': \
            case 'L': \
                if(*p_ == 'l' && *(p_+1) == 'f') \
                { \
                    ++p_; \
                    zfdouble v = va_arg(vaList_, double); \
                    if(v < 0 && flags_.hasFlag()) \
                    { \
                        flags_.positiveToken = '-'; \
                        v = -v; \
                    } \
                    zfsFromFloatT(ret_, v); \
                } \
                else if((*p_ == 'l' && *(p_+1) == 'l' && *(p_+2) == 'f') \
                    || (*p_ == 'L' && *(p_+1) == 'f')) \
                { \
                    if(*p_ == 'l') \
                    { \
                        p_ += 2; \
                    } \
                    else \
                    { \
                        ++p_; \
                    } \
                    zflongdouble v = va_arg(vaList_, long double); \
                    if(v < 0 && flags_.hasFlag()) \
                    { \
                        flags_.positiveToken = '-'; \
                        v = -v; \
                    } \
                    zfsFromFloatT(ret_, v); \
                } \
                else \
                { \
                    flags_.success = zffalse; \
                } \
                break; \
            case 'p': \
                zfsFromPointerT(ret_, va_arg(vaList_, const void *)); \
                break; \
            case 'c': \
            case 'C': \
                ret_ += (T_Char)va_arg(vaList_, int); \
                break; \
            case 's': \
            case 'S': \
            { \
                const T_Char *v = va_arg(vaList_, const T_Char *); \
                if(v != zfnull) \
                { \
                    if(flags_.precision != _ZFP_zfstringAppendPrecisionNone) \
                    { \
                        zfindex len = zfslenT(v); \
                        if(flags_.precision < len) \
                        { \
                            len = flags_.precision; \
                        } \
                        ret_.append(v, len); \
                    } \
                    else \
                    { \
                        ret_.append(v); \
                    } \
                } \
                else \
                { \
                    _ZFP_zfstringAppend_appendToken(ret_, ZFTOKEN_zfnull); \
                } \
            } \
                break; \
            default: \
                flags_.success = zffalse; \
                break; \
        } \
        ++p_; \
    } while(zffalse)
template<typename T_Char, typename T_Str>
void _ZFP_zfstringAppendT(ZF_OUT T_Str &s,
                          ZF_IN const T_Char *fmt,
                          ZF_IN va_list vaList)
{
    if(fmt == zfnull) {return ;}

    const T_Char *p = fmt;
    while(*p != '\0')
    {
        if(*p != '%')
        {
            zfcharAppendAndMoveNext(s, p);
            continue;
        }

        ++p;
        if(*p == '%')
        {
            s += '%';
            ++p;
            continue;
        }

        const T_Char *savedPos = p;
        _ZFP_zfstringAppendFlag flags;
        do
        {
            while(*p == '-' || *p == '0' || *p == '+' || *p == ' ')
            {
                if(*p == '-')
                {
                    if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_alignLeft)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstringAppendFlag_alignLeft);}
                }
                else if(*p == '0')
                {
                    if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_leadingZero)) {flags.success = zffalse; break;}
                    else {ZFBitSet(flags.flags, _ZFP_zfstringAppendFlag_leadingZero);}
                }
                else if(*p == '+')
                {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = '+';}
                }
                else if(*p == ' ')
                {
                    if(flags.positiveToken != '\0') {flags.success = zffalse; break;}
                    else {flags.positiveToken = ' ';}
                }
                ++p;
            }
            if(!flags.success) {break;}

            if(*p == '*')
            {
                flags.width = va_arg(vaList, zfindex);
                ++p;
            }
            else
            {
                while(*p >= '0' && *p <= '9')
                {
                    flags.width = flags.width * 10 + (*p - '0');
                    ++p;
                }
            }
            if(*p == '.')
            {
                ++p;
                if(*p == '*')
                {
                    flags.precision = va_arg(vaList, zfindex);
                    ++p;
                }
                else
                {
                    flags.precision = 0;
                    while(*p >= '0' && *p <= '9')
                    {
                        flags.precision = flags.precision * 10 + (*p - '0');
                        ++p;
                    }
                }
            }
        } while(zffalse);
        if(!flags.success)
        {
            s += '%';
            p = savedPos;
            continue;
        }
        if(!flags.hasFlag())
        {
            _ZFP_zfstringAppend_action(s, flags, p, vaList);
            continue;
        }
        T_Str tmp;
        _ZFP_zfstringAppend_action(tmp, flags, p, vaList);

        if(flags.precision != _ZFP_zfstringAppendPrecisionNone)
        {
            zfindex pos = zfstringFind(tmp, '.');
            if(pos != zfindexMax)
            {
                zfindex decimalLen = tmp.length() - pos - 1;
                if(decimalLen > flags.precision)
                {
                    zfindex newLen = pos + flags.precision + 1;
                    if(tmp[newLen] >= '5')
                    {
                        tmp[newLen - 1] = tmp[newLen - 1] + 1;
                    }
                    tmp.remove(newLen);
                }
                else if(decimalLen < flags.precision)
                {
                    for(zfindex i = decimalLen; i < flags.precision; ++i)
                    {
                        tmp += '0';
                    }
                }
            }
        }

        if(flags.positiveToken != '\0')
        {
            ++flags.width;
        }

        if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_alignLeft))
        {
            zfindex writtenLen = 0;
            if(flags.positiveToken != '\0')
            {
                s += flags.positiveToken;
                ++writtenLen;
            }
            s += tmp;
            writtenLen += tmp.length();
            for(zfindex i = writtenLen; i < flags.width; ++i)
            {
                s += ' ';
            }
        }
        else
        {
            zfindex leftTokenLen = ((flags.positiveToken == '\0') ? 0 : 1);
            if(flags.width > tmp.length() + leftTokenLen)
            {
                zfindex spaceLen = flags.width - tmp.length() - leftTokenLen;
                if(ZFBitTest(flags.flags, _ZFP_zfstringAppendFlag_leadingZero))
                {
                    if(flags.positiveToken != '\0')
                    {
                        s += flags.positiveToken;
                    }
                    for(zfindex i = 0; i < spaceLen; ++i)
                    {
                        s += '0';
                    }
                }
                else
                {
                    for(zfindex i = 0; i < spaceLen; ++i)
                    {
                        s += ' ';
                    }
                    if(flags.positiveToken != '\0')
                    {
                        s += flags.positiveToken;
                    }
                }
                s += tmp;
            }
            else
            {
                if(flags.positiveToken != '\0')
                {
                    s += flags.positiveToken;
                }
                s += tmp;
            }
        }
    } // while(*p != '\0')
}

void _ZFP_zfstringAppendV(ZF_OUT ZFCoreStringA &s, ZF_IN const zfcharA *fmt, va_list vaList)
{
    _ZFP_zfstringAppendT(s, fmt, vaList);
}
void _ZFP_zfstringAppendV(ZF_OUT ZFCoreStringW &s, ZF_IN const zfcharW *fmt, va_list vaList)
{
    _ZFP_zfstringAppendT(s, fmt, vaList);
}

ZF_NAMESPACE_GLOBAL_END

