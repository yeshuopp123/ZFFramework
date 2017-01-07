/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Android_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFOutput.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Android
ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFImpl_sys_Android_ZFOutput_output(fmt, ...) \
    AndroidLogDetail(AndroidLogLevelA, AndroidLogTagDefault, fmt, ##__VA_ARGS__)

zfclass _ZFP_I_ZFOutputImpl_sys_Android_SyncObject : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFOutputImpl_sys_Android_SyncObject, ZFObject)
};

ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFOutputImpl_sys_Android, ZFOutput, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Android:Logcat"))
public:
    zfoverride
    virtual void protocolOnInitFinish(void)
    {
        zfsuper::protocolOnInitFinish();
        this->syncObj = zfAllocWithoutLeakTest(_ZFP_I_ZFOutputImpl_sys_Android_SyncObject);
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        if(this->savedString.length() > 0)
        {
            _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), this->savedString.cString());
        }
        zfReleaseWithoutLeakTest(this->syncObj);
        zfsuper::protocolOnDeallocPrepare();
    }

public:
    virtual zfindex outputCoreLog(ZF_IN const zfcharA *s)
    {
        zfstringA tmp = s;
        this->checkOutput(tmp);
        return zfindexMax;
    }
    virtual zfindex outputLog(ZF_IN const zfchar *s, ZF_IN_OPT zfindex count = zfindexMax)
    {
        zfsynchronizedObject(this->syncObj);
        if(count == zfindexMax)
        {
            this->savedString += s;
        }
        else
        {
            this->savedString.append(s, count);
        }
        this->checkOutput(this->savedString);
        return zfindexMax;
    }
private:
    ZFObject *syncObj;
    zfstringA savedString;
    void checkOutput(ZF_IN_OUT zfstringA &s)
    {
        if(s.length() > 0)
        {
            if(s[s.length() - 1] == '\n')
            {
                if(s.length() == 1)
                {
                    _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA(" "));
                }
                else
                {
                    s.remove(s.length() - 1);
                    _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), s.cString());
                }
                s.removeAll();
            }
            else
            {
                zfindex index = zfstringFindReversely(s, '\n');
                if(index != zfindexMax)
                {
                    if(index == 0)
                    {
                        _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA(" "));
                    }
                    else
                    {
                        s[index] = '\0';
                        _ZFP_ZFImpl_sys_Android_ZFOutput_output(zfTextA("%s"), s.cString());
                    }
                    s.remove(0, index + 1);
                }
            }
        }
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFOutputImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFOutputImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

#endif

