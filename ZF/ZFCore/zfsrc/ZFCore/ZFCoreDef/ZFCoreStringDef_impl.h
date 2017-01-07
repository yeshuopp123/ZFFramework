/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
/**
 * @file ZFCoreStringDef_impl.h
 * @brief tricks to wrap std::string
 */

#ifndef _ZFI_ZFCoreStringDef_impl_h_
#define _ZFI_ZFCoreStringDef_impl_h_

#include "ZFCoreTypeDef_ClassType.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFCoreString_builtin_capacityDefault 16

// ============================================================
template<typename T_Char>
zfclassNotPOD _ZFP_ZFCoreString_implPrivate
{
public:
    zfbool builtinBuf;
    zfindex length;
    union {
        struct {
            zfindex capacity;
            T_Char *s;
        } s;
        T_Char buf[_ZFP_ZFCoreString_builtin_capacityDefault];
    } d;

public:
    _ZFP_ZFCoreString_implPrivate(void)
    : builtinBuf(zftrue)
    , length(0)
    , d()
    {
    }

public:
    inline T_Char *buf(void)
    {
        if(this->builtinBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
    inline const T_Char *buf(void) const
    {
        if(this->builtinBuf)
        {
            return d.buf;
        }
        else
        {
            return d.s.s;
        }
    }
};
/**
 * @brief low level string container
 */
template<typename T_Char>
zfclassLikePOD ZF_ENV_EXPORT ZFCoreString_impl
{
public:
    /** @brief construct an empty string */
    ZFCoreString_impl(void)
    : d()
    {
    }
    /** @brief construct a string with specified capacity */
    ZFCoreString_impl(ZF_IN zfindex capacity)
    : d()
    {
        _capacityRequire(capacity)[0] = '\0';
    }
    /** @brief copy content from another string */
    ZFCoreString_impl(ZF_IN const ZFCoreString_impl &s)
    : d()
    {
        T_Char *buf = _capacityRequire(s.length());
        d.length = s.length();
        zfmemcpy(buf, s.cString(), d.length * sizeof(T_Char));
        buf[d.length] = '\0';
    }
    /** @brief copy content from another string */
    ZFCoreString_impl(ZF_IN const ZFCoreString_impl &s, zfindex pos)
    : d()
    {
        if(pos < s.length())
        {
            T_Char *buf = _capacityRequire(s.length() - pos);
            d.length = s.length() - pos;
            zfmemcpy(buf, s.cString() + pos, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    ZFCoreString_impl(ZF_IN const ZFCoreString_impl &s, zfindex pos, zfindex len)
    : d()
    {
        if(pos < s.length())
        {
            if(len > s.length() - pos)
            {
                len = s.length() - pos;
            }
            T_Char *buf = _capacityRequire(len);
            d.length = len;
            zfmemcpy(buf, s.cString() + pos, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    ZFCoreString_impl(ZF_IN const T_Char *s)
    : d()
    {
        if(s)
        {
            zfindex len = zfslenT(s);
            T_Char *buf = _capacityRequire(len);
            d.length = len;
            zfmemcpy(buf, s, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    /** @brief copy content from another string */
    ZFCoreString_impl(ZF_IN const T_Char *s, zfindex len)
    : d()
    {
        if(s)
        {
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(len);
            d.length = len;
            zfmemcpy(buf, s, d.length * sizeof(T_Char));
            buf[d.length] = '\0';
        }
    }
    ~ZFCoreString_impl(void)
    {
        if(!d.builtinBuf)
        {
            zffree(d.d.s.s);
        }
    }

public:
    /** @cond ZFPrivateDoc */
    inline operator const T_Char *(void) const {return this->cString();}
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline ZFCoreString_impl &operator = (ZF_IN const ZFCoreString_impl &s)
    {
        if(&s == this)
        {
            return *this;
        }
        else
        {
            return this->assign(s);
        }
    }
    inline ZFCoreString_impl &operator = (ZF_IN const T_Char *s) {return this->assign(s);}
    ZFCoreString_impl &operator = (ZF_IN T_Char c)
    {
        T_Char *buf = d.buf();
        buf[0] = c;
        buf[1] = '\0';
        d.length = 1;
        return *this;
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline ZFCoreString_impl &operator += (ZF_IN const ZFCoreString_impl &s) {return this->append(s);}
    inline ZFCoreString_impl &operator += (ZF_IN const T_Char *s) {return this->append(s);}
    ZFCoreString_impl &operator += (ZF_IN T_Char c)
    {
        T_Char *buf = _capacityRequire(d.length + 1);
        buf[d.length] = c;
        ++d.length;
        buf[d.length] = '\0';
        return *this;
    }
    /** @endcond */

public:
    /** @cond ZFPrivateDoc */
    inline T_Char &operator[] (ZF_IN zfindex pos)
    {
        return d.buf()[pos];
    }
    inline const T_Char &operator[] (ZF_IN zfindex pos) const
    {
        return d.buf()[pos];
    }
    /** @endcond */

public:
    /** @brief append string */
    inline ZFCoreString_impl &append(ZF_IN const ZFCoreString_impl &s) {this->append(s.cString(), s.length()); return *this;}
    /** @brief append string */
    inline ZFCoreString_impl &append(ZF_IN const ZFCoreString_impl &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax)
    {
        return this->append(s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief append string */
    inline ZFCoreString_impl &append(ZF_IN const T_Char *s) {return this->append(s, zfindexMax);}
    /** @brief append string */
    ZFCoreString_impl &append(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len);
            _safeCheck(buf, s);
            zfmemcpy(buf + d.length, s, len * sizeof(T_Char));
            d.length += len;
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief replace all content of the string */
    inline ZFCoreString_impl &assign(ZF_IN const ZFCoreString_impl &s) {return this->assign(s.cString(), s.length());}
    /** @brief replace all content of the string */
    inline ZFCoreString_impl &assign(ZF_IN const ZFCoreString_impl &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax)
    {
        return this->assign(s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief replace all content of the string */
    inline ZFCoreString_impl &assign(ZF_IN const T_Char *s) {return this->assign(s, zfindexMax);}
    /** @brief replace all content of the string */
    ZFCoreString_impl &assign(ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(s)
        {
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(len);;
            _safeCheck(buf, s);
            zfmemcpy(buf, s, len * sizeof(T_Char));
            d.length = len;
            buf[d.length] = '\0';
        }
        else
        {
            d.length = 0;
            d.buf()[0] = '\0';
        }
        return *this;
    }

public:
    /** @brief insert string */
    inline ZFCoreString_impl &insert(ZF_IN zfindex insertAt, ZF_IN const ZFCoreString_impl &s) {return this->insert(insertAt, s.cString(), s.length());}
    /** @brief insert string */
    inline ZFCoreString_impl &insert(ZF_IN zfindex insertAt, ZF_IN const ZFCoreString_impl &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax)
    {
        return this->insert(insertAt, s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief insert string */
    inline ZFCoreString_impl &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s) {return this->insert(insertAt, s, zfindexMax);}
    /** @brief insert string */
    ZFCoreString_impl &insert(ZF_IN zfindex insertAt, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(insertAt >= d.length)
        {
            this->append(s, len);
        }
        else if(s)
        {
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len);
            _safeCheck(buf, s);
            zfmemmove(buf + insertAt + len, buf + insertAt, (d.length - insertAt) * sizeof(T_Char));
            zfmemcpy(buf + insertAt, s, len * sizeof(T_Char));
            d.length += len;
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief replace string in range */
    inline ZFCoreString_impl &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const ZFCoreString_impl &s) {return this->replace(replacePos, replaceLen, s.cString(), s.length());}
    /** @brief replace string in range */
    ZFCoreString_impl &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const ZFCoreString_impl &s, ZF_IN zfindex pos, ZF_IN zfindex len = zfindexMax)
    {
        return this->replace(replacePos, replaceLen, s.cString() + pos, (pos < s.length()) ? ((len > s.length() - pos) ? (s.length() - pos) : len) : 0);
    }
    /** @brief replace string in range */
    inline ZFCoreString_impl &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s) {return this->replace(replacePos, replaceLen, s, zfindexMax);}
    /** @brief replace string in range */
    ZFCoreString_impl &replace(ZF_IN zfindex replacePos, ZF_IN zfindex replaceLen, ZF_IN const T_Char *s, ZF_IN zfindex len)
    {
        if(replacePos >= d.length)
        {
            this->append(s, len);
        }
        else if(s)
        {
            if(replaceLen > d.length - replacePos)
            {
                replaceLen = d.length - replacePos;
            }
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            T_Char *buf = _capacityRequire(d.length + len - replaceLen);
            _safeCheck(buf, s);
            zfmemmove(buf + replacePos + len, buf + replacePos + replaceLen, (d.length - replacePos - replaceLen) * sizeof(T_Char));
            zfmemcpy(buf + replacePos, s, len * sizeof(T_Char));
            d.length = d.length + len - replaceLen;
            buf[d.length] = '\0';
        }
        return *this;
    }

public:
    /** @brief access string value */
    inline const T_Char *cString(void) const
    {
        return d.buf();
    }
    /** @brief length of the string */
    inline zfindex length(void) const {return d.length;}
    /** @brief true if empty */
    inline zfbool isEmpty(void) const {return (d.length == 0);}

public:
    /** @brief ensure the string's capacity */
    inline void capacitySet(ZF_IN zfindex capacity) {_capacityRequire(capacity);}
    /** @brief capacity of the string */
    zfindex capacity(void)
    {
        if(d.builtinBuf)
        {
            return _ZFP_ZFCoreString_builtin_capacityDefault;
        }
        else
        {
            return d.d.s.capacity;
        }
    }
    /** @brief remove part of the string */
    void remove(ZF_IN zfindex pos = 0, ZF_IN zfindex len = zfindexMax)
    {
        if(pos < d.length)
        {
            if(len > d.length - pos)
            {
                len = d.length - pos;
            }
            if(len > 0)
            {
                T_Char *buf = d.buf();
                zfmemmove(buf + pos, buf + pos + len, (d.length - pos - len) * sizeof(T_Char));
                d.length -= len;
                buf[d.length] = '\0';
            }
        }
    }
    /** @brief remove all content of the string */
    inline void removeAll(void)
    {
        d.length = 0;
        d.buf()[0] = '\0';
    }

public:
    /** @brief compare with another string */
    inline zfint compare(const ZF_IN ZFCoreString_impl &s) const {return zfscmpT(this->cString(), s.cString());}
    /** @brief compare with another string */
    zfint compare(ZF_IN const T_Char *s, ZF_IN zfindex len = zfindexMax) const
    {
        const T_Char *buf = d.buf();
        if(s)
        {
            if(len == zfindexMax)
            {
                len = zfslenT(s);
            }
            if(len > d.length)
            {
                return zfsncmpT(buf, s, d.length + 1);
            }
            else
            {
                zfint t = zfsncmpT(buf, s, len);
                if(t)
                {
                    return t;
                }
                else
                {
                    return (zfint)buf[len];
                }
            }
        }
        else
        {
            return (zfint)buf[0];
        }
    }

private:
    _ZFP_ZFCoreString_implPrivate<T_Char> d;
private:
    T_Char *_capacityRequire(ZF_IN zfindex size)
    {
        if(d.builtinBuf)
        {
            if(size >= _ZFP_ZFCoreString_builtin_capacityDefault)
            {
                zfindex capacity = this->capacity();
                while(capacity <= size)
                {
                    capacity *= 2;
                }
                T_Char *buf = (T_Char *)zfmalloc(capacity * sizeof(T_Char));
                zfmemcpy(buf, d.d.buf, d.length * sizeof(T_Char));
                d.d.s.s = buf;
                d.d.s.capacity = capacity;
                d.builtinBuf = zffalse;
                return d.d.s.s;
            }
            return d.d.buf;
        }
        else
        {
            if(size >= d.d.s.capacity)
            {
                do
                {
                    d.d.s.capacity *= 2;
                } while(d.d.s.capacity <= size);
                d.d.s.s = (T_Char *)zfrealloc(d.d.s.s, d.d.s.capacity * sizeof(T_Char));
            }
            return d.d.s.s;
        }
    }
    inline void _safeCheck(ZF_IN const T_Char *buf, ZF_IN const T_Char *s) const
    {
        // no overlapped copy supported
        if(s >= buf && s < buf + d.length)
        {
            abort();
        }
    }
};

/** @brief see #ZFCoreString_impl */
typedef ZFCoreString_impl<zfcharA> ZFCoreStringA_impl;
/** @brief see #ZFCoreString_impl */
typedef ZFCoreString_impl<zfcharW> ZFCoreStringW_impl;

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreStringDef_implwrap_h_

