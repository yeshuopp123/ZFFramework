/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLang.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, LangOnChange)

// ============================================================
typedef zfstlhashmap<zfstlstringZ, zfstring> _ZFP_ZFLangMapType;

// ============================================================
static _ZFP_ZFLangMapType &_ZFP_ZFLangMapGlobal(void)
{
    static _ZFP_ZFLangMapType d;
    return d;
}
void zfLang(ZF_IN_OUT zfstring &ret,
            ZF_IN const zfchar *key,
            ZF_IN_OPT const zfchar *valueDefault /* = zfnull */)
{
    ZFCoreMutexLocker();
    _ZFP_ZFLangMapType &m = _ZFP_ZFLangMapGlobal();
    _ZFP_ZFLangMapType::iterator it = m.find(key);
    if(it != m.end())
    {
        ret += it->second;
    }
    else
    {
        if(valueDefault != zfnull)
        {
            ret += valueDefault;
        }
        else
        {
            ret += key;
        }
    }
}
void zfLangNotifyChanged(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventLangOnChange());
}
void zfLangSet(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(key != zfnull)
    {
        ZFCoreMutexLocker();
        if(value == zfnull)
        {
            _ZFP_ZFLangMapGlobal().erase(key);
        }
        else
        {
            _ZFP_ZFLangMapGlobal()[key] = value;
        }
    }
}
void zfLangSetDefault(ZF_IN const zfchar *key, ZF_IN const zfchar *value)
{
    if(key && value)
    {
        ZFCoreMutexLocker();
        _ZFP_ZFLangMapType &m = _ZFP_ZFLangMapGlobal();
        _ZFP_ZFLangMapType::iterator it = m.find(key);
        if(it == m.end())
        {
            m[key] = value;
        }
    }
}

void zfLangUnload(void)
{
    ZFCoreMutexLocker();
    _ZFP_ZFLangMapGlobal().clear();

    zfLangNotifyChanged();
}

void zfLangDebug(ZF_IN_OPT const ZFOutputCallback &output /* = ZFOutputCallbackDefault */)
{
    _ZFP_ZFLangMapType &m = _ZFP_ZFLangMapGlobal();
    for(_ZFP_ZFLangMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        output << it->first.c_str() << zfText(" = \"") << it->second << zfText("\"\n");
    }
}

ZF_NAMESPACE_GLOBAL_END

