/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_sys_Qt_ZFCore.h"
#include "ZFCore/protocol/ZFProtocolZFOutput.h"
#include "ZFCore/ZFString.h"

#if ZF_ENV_sys_Qt
#include <QDebug>
#include <QTextCodec>
#include <QMutex>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTRINGENCODING_ASSERT(ZFStringEncoding::e_UTF8)
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFOutputImpl_sys_Qt, ZFOutput, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT(zfText("Qt:qDebug"))
public:
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        QMutexLocker _logMutexLocker(&_logMutex);
        if(this->savedString.length() > 0)
        {
            qDebug() << this->savedString.cString();
        }
        zfsuper::protocolOnDealloc();
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
        QMutexLocker _logMutexLocker(&_logMutex);
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
    zfstringA savedString;
    void checkOutput(ZF_IN_OUT zfstringA &s)
    {
        if(s.length() > 0)
        {
            if(s[s.length() - 1] == '\n')
            {
                s.remove(s.length() - 1);
                qDebug() << s.cString();
                s.removeAll();
            }
            else
            {
                zfindex index = zfstringFindReversely(s, '\n');
                if(index != zfindexMax)
                {
                    s[index] = '\0';
                    qDebug() << s.cString();
                    s.remove(0, index + 1);
                }
            }
        }
    }
private:
    QMutex _logMutex;
ZFPROTOCOL_IMPLEMENTATION_END(ZFOutputImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFOutputImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif

