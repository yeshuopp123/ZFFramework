/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZFAlgorithm.h"
#include "ZFAlgorithm/protocol/ZFProtocolZFXmlEscapeChar.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFXmlEscapeCharImpl_default, ZFXmlEscapeChar, ZFProtocolLevel::e_Default)
public:
    virtual void xmlEscapeCharEncode(ZF_OUT const ZFOutputCallback &dst,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex count = zfindexMax)
    {
        const zfchar *p = src;
        const zfchar *pLeft = p;
        const zfchar *pEnd = ((count == zfindexMax) ? (p + zfslen(src)) : (p + count));
        while(p < pEnd)
        {
            if(*p == '<')
            {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&lt;")); ++p; pLeft = p;}
            else if(*p == '>')
            {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&gt;")); ++p; pLeft = p;}
            else if(*p == '&')
            {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&amp;")); ++p; pLeft = p;}
            else if(*p == '\'')
            {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&apos;")); ++p; pLeft = p;}
            else if(*p == '\"')
            {if(pLeft != p) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&quot;")); ++p; pLeft = p;}
            else
            {
                zfcharMoveNext(p);
            }
        }
        if(pLeft < pEnd)
        {
            dst.execute(pLeft, pEnd - pLeft);
        }
    }
    virtual void xmlEscapeCharDecode(ZF_OUT const ZFOutputCallback &dst,
                                     ZF_IN const zfchar *src,
                                     ZF_IN_OPT zfindex count = zfindexMax)
    {
        const zfchar *p = src;
        const zfchar *pLeft = p;
        const zfchar *pEnd = ((count == zfindexMax) ? (p + zfslen(src)) : (p + count));
        zfindex encodedCharLen = 0;
        while(p < pEnd)
        {
            if(*p == '&')
            {
                // &lt;
                if(p+3 < pEnd && *(p+1) == 'l' && *(p+2) == 't' && *(p+3) == ';')
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("<")); p += 4; pLeft = p;}

                // &gt;
                else if(p+3 < pEnd && *(p+1) == 'g' && *(p+2) == 't' && *(p+3) == ';')
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText(">")); p += 4; pLeft = p;}

                // &amp;
                else if(p+4 < pEnd && *(p+1) == 'a' && *(p+2) == 'm' && *(p+3) == 'p' && *(p+4) == ';')
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("&")); p += 5; pLeft = p;}

                // &apos;
                else if(p+5 < pEnd && *(p+1) == 'a' && *(p+2) == 'p' && *(p+3) == 'o' && *(p+4) == 's' && *(p+5) == ';')
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("\'")); p += 6; pLeft = p;}

                // &quot;
                else if(p+5 < pEnd && *(p+1) == 'q' && *(p+2) == 'u' && *(p+3) == 'o' && *(p+4) == 't' && *(p+5) == ';')
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} dst.execute(zfText("\"")); p += 6; pLeft = p;}

                // &#0; ~ &#65536;
                else if(p+3 < pEnd && *(p+1) == '#' && (encodedCharLen = this->decimalCharCheck(p+2, pEnd)) != 0)
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} p += 2; this->decimalCharEscape(dst, p, encodedCharLen); pLeft = p;}

                // &#x0; ~ &#xFFFF;
                else if(p+4 < pEnd && *(p+1) == '#' && *(p+2) == 'x' && (encodedCharLen = this->heximalCharCheck(p+3, pEnd)) != 0)
                {if(p != pLeft) {dst.execute(pLeft, p - pLeft);} p += 3; this->heximalCharEscape(dst, p, encodedCharLen); pLeft = p;}

                else {++p;}
            }
            else
            {
                zfcharMoveNext(p);
            }
        }
        if(pLeft < pEnd)
        {
            dst.execute(pLeft, pEnd - pLeft);
        }
    }
private:
    zfindex decimalCharCheck(ZF_IN const zfchar *head,
                             ZF_IN const zfchar *end)
    { // 0; ~ 65536;
        const zfchar *p = head;
        if(head + 6 < end)
        {
            end = head + 6;
        }
        if(zfcharIsNumber(*p))
        {
            do
            {
                ++p;
                if(zfcharIsNumber(*p))
                {
                    continue;
                }
                else if(*p == ';')
                {
                    return p - head;
                }
                else
                {
                    break;
                }
            } while (p < end);
        }
        return 0;
    }
    void decimalCharEscape(ZF_IN_OUT const ZFOutputCallback &dst,
                           ZF_IN_OUT const zfchar *&p,
                           ZF_IN zfindex encodedCharLen)
    {
        zfcharW s[4] = {0};
        if(zfsToInt(*(zft_zfuint16 *)s, p, encodedCharLen, 10) == zfnull)
        {
            dst.execute(ZFString::toZFChar(s, ZFStringEncoding::e_UTF16).cString());
        }
        else
        {
            dst.execute(zfText("&#"));
            dst.execute(zfstring(p, encodedCharLen).cString());
            dst.execute(zfText(";"));
        }
        p += encodedCharLen + 1;
    }
    zfindex heximalCharCheck(ZF_IN const zfchar *head,
                             ZF_IN const zfchar *end)
    { // 0; ~ FFFF;
        const zfchar *p = head;
        if(head + 5 < end)
        {
            end = head + 5;
        }
        if(zfcharIsHexNumber(*p))
        {
            do
            {
                ++p;
                if(zfcharIsHexNumber(*p))
                {
                    continue;
                }
                else if(*p == ';')
                {
                    return p - head;
                }
                else
                {
                    break;
                }
            } while (p < end);
        }
        return 0;
    }
    void heximalCharEscape(ZF_IN_OUT const ZFOutputCallback &dst,
                           ZF_IN_OUT const zfchar *&p,
                           ZF_IN zfindex encodedCharLen)
    {
        zfcharW s[4] = {0};
        if(zfsToInt(*(zft_zfuint16 *)s, p, encodedCharLen, 16) == zfnull)
        {
            dst.execute(ZFString::toZFChar(s, ZFStringEncoding::e_UTF16).cString());
        }
        else
        {
            dst.execute(zfText("&#x"));
            dst.execute(zfstring(p, encodedCharLen).cString());
            dst.execute(zfText(";"));
        }
        p += encodedCharLen + 1;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFXmlEscapeCharImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFXmlEscapeCharImpl_default)

ZF_NAMESPACE_GLOBAL_END

