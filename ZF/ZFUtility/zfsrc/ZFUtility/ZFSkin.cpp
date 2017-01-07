/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFSkin.h"
#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_hashmap.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBSERVER_EVENT_REGISTER(ZFGlobalEvent, SkinOnChange)

// ============================================================
typedef zfstlhashmap<zfstlstringZ, zfautoObject> _ZFP_ZFSkinMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFSkinDataHolder, ZFLevelZFFrameworkHigh)
{
}
public:
    _ZFP_ZFSkinMapType skinMap;
ZF_GLOBAL_INITIALIZER_END(ZFSkinDataHolder)
#define _ZFP_ZFSkinMapGlobal ZF_GLOBAL_INITIALIZER_INSTANCE(ZFSkinDataHolder)->skinMap

// ============================================================
zfautoObject zfSkin(ZF_IN const zfchar *key,
                    ZF_IN_OPT ZFStyleable *valueDefault /* = zfnull */)
{
    ZFCoreMutexLocker();
    _ZFP_ZFSkinMapType &m = _ZFP_ZFSkinMapGlobal;
    _ZFP_ZFSkinMapType::iterator it = m.find(key);
    if(it != m.end())
    {
        return zfautoObjectCreate(it->second.toObject());
    }
    else
    {
        if(valueDefault != zfnull)
        {
            return zfautoObjectCreate(valueDefault->toObject());
        }
        else
        {
            return zfautoObjectNull;
        }
    }
}
void zfSkinNotifyChanged(void)
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventSkinOnChange());
}
void zfSkinSet(ZF_IN const zfchar *key, ZF_IN ZFStyleable *value)
{
    if(key != zfnull)
    {
        ZFCoreMutexLocker();
        if(value == zfnull)
        {
            _ZFP_ZFSkinMapGlobal.erase(key);
        }
        else
        {
            if(value->classData()->classIsTypeOf(ZFSerializable::ClassData()))
            {
                _ZFP_ZFSkinMapGlobal[key] = zfautoObjectCreateMarkCached(value->toObject());
            }
            else
            {
                zfCoreCriticalClassNotTypeOf(value->classData(), ZFSerializable::ClassData());
            }
        }
    }
}
void zfSkinSetDefault(ZF_IN const zfchar *key, ZF_IN ZFStyleable *value)
{
    if(key && value)
    {
        if(!value->classData()->classIsTypeOf(ZFSerializable::ClassData()))
        {
            zfCoreCriticalClassNotTypeOf(value->classData(), ZFSerializable::ClassData());
            return ;
        }

        ZFCoreMutexLocker();
        _ZFP_ZFSkinMapType &m = _ZFP_ZFSkinMapGlobal;
        _ZFP_ZFSkinMapType::iterator it = m.find(key);
        if(it == m.end())
        {
            m[key] = zfautoObjectCreateMarkCached(value->toObject());
        }
    }
}

void zfSkinUnload(void)
{
    ZFCoreMutexLocker();
    _ZFP_ZFSkinMapGlobal.clear();

    zfSkinNotifyChanged();
}

void zfSkinDebug(ZF_IN_OPT const ZFOutputCallback &output /* = ZFOutputCallbackDefault */)
{
    _ZFP_ZFSkinMapType &m = _ZFP_ZFSkinMapGlobal;
    for(_ZFP_ZFSkinMapType::iterator it = m.begin(); it != m.end(); ++it)
    {
        output << it->first.c_str() << zfText(" = ") << it->second.toObject()->objectInfoOfInstance() << zfText("\n");
    }
}

ZF_NAMESPACE_GLOBAL_END

