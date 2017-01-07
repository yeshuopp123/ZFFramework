/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFFileCallbackDef_ZFIOBridgeCallbackUsingTmpFile.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclass _ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE_ALLOW_CUSTOM_CONSTRUCTOR(_ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate, ZFObject)

private:
    ZFFileToken token;
    zfstring tmpFileName;
    zfindex outputIndex;
    zfindex inputIndex;

public:
    _ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate(void)
    : token(zfnull)
    , tmpFileName()
    , outputIndex(0)
    , inputIndex(0)
    {
        zfstringAppend(this->tmpFileName, zfText("%s%cZFIOBridgeCallbackUsingTmpFile_%s"),
            ZFFile::cachePath(),
            ZFFile::fileSeparator,
            zfsFromInt(zfidentityCalcPointer(this)).cString());

        this->token = ZFFile::fileOpen(this->tmpFileName.cString(),
            ZFFileOpenOption::e_Create | ZFFileOpenOption::e_Read | ZFFileOpenOption::e_Write);
    }
    ~_ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate(void)
    {
        if(this->token != ZFFileTokenInvalid)
        {
            ZFFile::fileClose(this->token);
            this->token = ZFFileTokenInvalid;
        }
        ZFFile::fileRemove(this->tmpFileName.cString(), zfHint("recursive")zffalse, zfHint("force")zftrue);
    }

public:
    zfbool callbackIsValid(void)
    {
        return (this->token != ZFFileTokenInvalid);
    }

    void resetInput(void)
    {
        this->inputIndex = 0;
    }
    void resetOutput(void)
    {
        this->inputIndex = 0;
        this->outputIndex = 0;
    }

public:
    ZFMETHOD_DECLARE_2(zfindex, onInput, void *, buf, zfindex, count)
    {
        if(this->token == ZFFileTokenInvalid)
        {
            return 0;
        }
        if(buf == zfnull)
        {
            return this->outputIndex - this->inputIndex;
        }
        else
        {
            ZFFile::fileSeek(this->token, this->inputIndex);
            zfindex read = ZFFile::fileRead(this->token, buf, zfmMin(count, this->outputIndex - this->inputIndex));
            this->inputIndex += read;
            return read;
        }
    }
    ZFMETHOD_DECLARE_2(zfindex, onOutput, const void *, buf, zfindex, count)
    {
        if(this->token == ZFFileTokenInvalid)
        {
            return 0;
        }
        if(count == 0)
        {
            return count;
        }
        ZFFile::fileSeek(this->token, this->outputIndex);
        zfindex written = ZFFile::fileWrite(this->token, buf, count);
        this->outputIndex += written;
        return written;
    }
};

/** @cond ZFPrivateDoc */
ZFIOBridgeCallbackUsingTmpFile::ZFIOBridgeCallbackUsingTmpFile(void)
: ZFIOBridgeCallbackAbs()
{
    d = zfAllocWithoutLeakTest(_ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate);
}
ZFIOBridgeCallbackUsingTmpFile::ZFIOBridgeCallbackUsingTmpFile(ZF_IN const ZFIOBridgeCallbackUsingTmpFile &ref)
: ZFIOBridgeCallbackAbs(ref)
{
    d = zfRetainWithoutLeakTest(ref.d);
}
ZFIOBridgeCallbackUsingTmpFile &ZFIOBridgeCallbackUsingTmpFile::operator =(ZF_IN const ZFIOBridgeCallbackUsingTmpFile &ref)
{
    zfRetainWithoutLeakTest(ref.d);
    zfReleaseWithoutLeakTest(d);
    d = ref.d;
    return *this;
}
/** @endcond */
ZFIOBridgeCallbackUsingTmpFile::~ZFIOBridgeCallbackUsingTmpFile(void)
{
    zfReleaseWithoutLeakTest(d);
    d = zfnull;
}

ZFInputCallback ZFIOBridgeCallbackUsingTmpFile::inputCallback(void)
{
    return (d->callbackIsValid()
        ? ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate, onInput))
        : ZFCallbackNull());
}
ZFOutputCallback ZFIOBridgeCallbackUsingTmpFile::outputCallback(void)
{
    return (d->callbackIsValid()
        ? ZFCallbackForMemberMethod(d, ZFMethodAccess(_ZFP_ZFIOBridgeCallbackUsingTmpFilePrivate, onOutput))
        : ZFCallbackNull());
}

void ZFIOBridgeCallbackUsingTmpFile::resetInput(void)
{
    d->resetInput();
}
void ZFIOBridgeCallbackUsingTmpFile::resetOutput(void)
{
    d->resetOutput();
}

ZF_NAMESPACE_GLOBAL_END

