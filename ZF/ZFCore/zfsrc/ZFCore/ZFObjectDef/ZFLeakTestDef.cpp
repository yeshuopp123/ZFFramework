/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFLeakTestDef.h"

#include "ZFObjectDef.h"
#include "ZFEnumDef.h"
#include "ZFObjectSmartPointerDef.h"
#include "ZFPropertyTypeDef.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_deque.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

static zfbool _ZFP_ZFLeakTestEnable = zftrue;
zfbool ZFLeakTestEnable(void)
{
    return _ZFP_ZFLeakTestEnable;
}
void ZFLeakTestEnableSet(ZF_IN zfbool enable)
{
    zfCoreAssertWithMessageTrim(!_ZFP_ZFLeakTestEnableCache,
        zfText("[ZFLeakTest] you must not change ZFLeakTestEnable while ZFLeakTest already started"));
    _ZFP_ZFLeakTestEnable = enable;
}

// ============================================================
static zfbool _ZFP_ZFLeakTestInitFlag = zffalse;
typedef zfstlmap<zfstlstringZ, zfindex> _ZFP_ZFLeakTestGlobalExcludeNameType;
typedef zfstlmap<ZFObject *, zfindex> _ZFP_ZFLeakTestGlobalExcludeInstanceType;
ZF_STATIC_INITIALIZER_INIT(ZFLeakTestGlobalExcludeData)
{
}
ZF_STATIC_INITIALIZER_DESTROY(ZFLeakTestGlobalExcludeData)
{
}
_ZFP_ZFLeakTestGlobalExcludeNameType excludeNames;
_ZFP_ZFLeakTestGlobalExcludeInstanceType excludeInstances;
ZF_STATIC_INITIALIZER_END(ZFLeakTestGlobalExcludeData)
#define _ZFP_ZFLeakTestGlobalExcludeNames \
    (ZF_STATIC_INITIALIZER_INSTANCE(ZFLeakTestGlobalExcludeData)->excludeNames)
#define _ZFP_ZFLeakTestGlobalExcludeInstances \
    (ZF_STATIC_INITIALIZER_INSTANCE(ZFLeakTestGlobalExcludeData)->excludeInstances)

void ZFLeakTestExcludeNameAdd(ZF_IN const zfchar *name)
{
    if(!_ZFP_ZFLeakTestInitFlag || name == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    _ZFP_ZFLeakTestGlobalExcludeNameType::iterator it = _ZFP_ZFLeakTestGlobalExcludeNames.find(name);
    if(it != _ZFP_ZFLeakTestGlobalExcludeNames.end())
    {
        ++(it->second);
    }
    else
    {
        _ZFP_ZFLeakTestGlobalExcludeNames[name] = 1;
    }
}
void ZFLeakTestExcludeNameRemove(ZF_IN const zfchar *name)
{
    if(!_ZFP_ZFLeakTestInitFlag || name == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    _ZFP_ZFLeakTestGlobalExcludeNameType::iterator it = _ZFP_ZFLeakTestGlobalExcludeNames.find(name);
    if(it != _ZFP_ZFLeakTestGlobalExcludeNames.end())
    {
        --(it->second);
        if(it->second == 0)
        {
            _ZFP_ZFLeakTestGlobalExcludeNames.erase(it);
        }
    }
}
void ZFLeakTestExcludeInstanceAdd(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag || obj == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    _ZFP_ZFLeakTestGlobalExcludeInstanceType::iterator it = _ZFP_ZFLeakTestGlobalExcludeInstances.find(obj);
    if(it != _ZFP_ZFLeakTestGlobalExcludeInstances.end())
    {
        ++(it->second);
    }
    else
    {
        _ZFP_ZFLeakTestGlobalExcludeInstances[obj] = 1;
    }
}
void ZFLeakTestExcludeInstanceRemove(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag || obj == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    _ZFP_ZFLeakTestGlobalExcludeInstanceType::iterator it = _ZFP_ZFLeakTestGlobalExcludeInstances.find(obj);
    if(it != _ZFP_ZFLeakTestGlobalExcludeInstances.end())
    {
        --(it->second);
        if(it->second == 0)
        {
            _ZFP_ZFLeakTestGlobalExcludeInstances.erase(it);
        }
    }
}

// ============================================================
ZFLeakTestObjectFilter _ZFP_ZFLeakTestObjectFilterNull;

// ============================================================
// global type
ZFENUM_BEGIN(_ZFP_ZFLeakTestAction)
    ZFENUM_VALUE(Alloc)
    ZFENUM_VALUE(Retain)
    ZFENUM_VALUE(Release)
    ZFENUM_VALUE(AutoRelease)
    ZFENUM_VALUE(OnAutoRelease)
ZFENUM_SEPARATOR(_ZFP_ZFLeakTestAction)
    ZFENUM_VALUE_REGISTER_WITH_NAME(Alloc,            zfText("  Alloc"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(Retain,           zfText(" Retain"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(Release,          zfText("Release"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(AutoRelease,      zfText("  AutoR"))
    ZFENUM_VALUE_REGISTER_WITH_NAME(OnAutoRelease,    zfText("OnAutoR"))
ZFENUM_END(_ZFP_ZFLeakTestAction)
ZFENUM_DEFINE(_ZFP_ZFLeakTestAction)
zfclassNotPOD _ZFP_ZFLeakTestActionData
{
public:
    _ZFP_ZFLeakTestActionData(ZF_IN _ZFP_ZFLeakTestActionEnum action,
                              ZF_IN zfindex curRetainCount,
                              ZF_IN const zfcharA *callerFile,
                              ZF_IN const zfcharA *callerFunction,
                              ZF_IN zfindex callerLine)
    : action(action)
    , curRetainCount(curRetainCount)
    , callerFile((callerFile != zfnull) ? callerFile : "")
    , callerFunction((callerFunction != zfnull) ? callerFunction : "")
    , callerLine(callerLine)
    {
    }
    ~_ZFP_ZFLeakTestActionData(void)
    {
    }
public:
    _ZFP_ZFLeakTestActionEnum action;
    zfindex curRetainCount;
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
};

typedef zfstldeque<ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> > _ZFP_ZFLeakTestActionDataListType;
zfclassNotPOD _ZFP_ZFLeakTestObjectData
{
public:
    _ZFP_ZFLeakTestObjectData(void)
    : objectRetainCountSaved(0)
    , objectCacheCountFix(0)
    , actionList()
    {
    }

public:
    zfindex objectRetainCountSaved;
    zfint objectCacheCountFix;
    _ZFP_ZFLeakTestActionDataListType actionList;
};
typedef zfstlmap<ZFObject *, ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> > _ZFP_ZFLeakTestObjectDataMapType;
zfclassNotPOD _ZFP_ZFLeakTestSectionData
{
public:
    _ZFP_ZFLeakTestSectionData(void)
    : callerFile(zfnull)
    , callerFunction(zfnull)
    , callerLine(0)
    , param()
    , data()
    {
    }

public:
    const zfcharA *callerFile;
    const zfcharA *callerFunction;
    zfindex callerLine;
    ZFLeakTestBeginParam param;
    _ZFP_ZFLeakTestObjectDataMapType data;

public:
    void output(ZF_IN const zfchar *s)
    {
        if(this->param.outputCallback().callbackIsValid())
        {
            this->param.outputCallback().execute(s);
        }
    }
};
typedef zfstldeque<ZFCorePointerForObject<_ZFP_ZFLeakTestSectionData *> > _ZFP_ZFLeakTestGlobalDataType;

// ============================================================
// global data
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFLeakTestDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFLeakTestInitFlag = zftrue;
    (void)ZF_STATIC_INITIALIZER_INSTANCE(ZFLeakTestGlobalExcludeData);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFLeakTestDataHolder)
{
    while(!this->globalData.empty())
    {
        ZFLeakTestEnd();
    }
    _ZFP_ZFLeakTestInitFlag = zffalse;
}
public:
    _ZFP_ZFLeakTestGlobalDataType globalData;
ZF_GLOBAL_INITIALIZER_END(ZFLeakTestDataHolder)
#define _ZFP_ZFLeakTestGlobalData (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFLeakTestDataHolder)->globalData)
static _ZFP_ZFLeakTestSectionData *_ZFP_ZFLeakTestActivatingSection = zfnull;
zfbool _ZFP_ZFLeakTestEnableCache = zffalse;

// ============================================================
// internal function declare
static void _ZFP_ZFLeakTestPrintLocationInfo(ZF_IN _ZFP_ZFLeakTestSectionData *ownerSection,
                                             ZF_IN_OPT const zfcharA *callerFile = zfnull,
                                             ZF_IN_OPT const zfcharA *callerFunction = zfnull,
                                             ZF_IN_OPT zfindex callerLine = 0);
static void _ZFP_ZFLeakTestPrintObjectStatus(ZF_IN _ZFP_ZFLeakTestSectionData *ownerSection,
                                             ZF_IN ZFObject *obj,
                                             ZF_IN _ZFP_ZFLeakTestObjectData *objectData);

// ============================================================
// onAlloc
void _ZFP_ZFLeakTestLogAfterAlloc(ZF_IN ZFObject *obj,
                                  ZF_IN const zfcharA *callerFile,
                                  ZF_IN const zfcharA *callerFunction,
                                  ZF_IN zfindex callerLine)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull || obj == zfnull)
    {
        return ;
    }

    zfindex objectRetainCount = obj->objectRetainCount();

    ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> data(zfnew(_ZFP_ZFLeakTestActionData,
        _ZFP_ZFLeakTestAction::e_Alloc,
        objectRetainCount,
        callerFile,
        callerFunction,
        callerLine));

    ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
    objectData.pointerValueGet()->objectRetainCountSaved = objectRetainCount - 1;
    objectData.pointerValueGet()->actionList.push_back(data);
    _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;

    if(_ZFP_ZFLeakTestActivatingSection->param.verboseMode())
    {
        zfstring s = zfstringWithFormat(zfText("[ZFLeakTest] %s(%2zi) %s %s, info: %s\n"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(_ZFP_ZFLeakTestAction::e_Alloc),
            objectRetainCount,
            obj->objectInfoOfInstance().cString(),
            zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine)),
            obj->objectInfo().cString());
        _ZFP_ZFLeakTestActivatingSection->output(s.cString());
    }
}

// ============================================================
// objectOnRetain
void _ZFP_ZFLeakTestLogAfterRetain(ZF_IN ZFObject *obj,
                                   ZF_IN const zfcharA *callerFile,
                                   ZF_IN const zfcharA *callerFunction,
                                   ZF_IN zfindex callerLine)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull || obj == zfnull)
    {
        return ;
    }

    zfindex objectRetainCount = obj->objectRetainCount();

    ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> data(zfnew(_ZFP_ZFLeakTestActionData,
        _ZFP_ZFLeakTestAction::e_Retain,
        objectRetainCount,
        callerFile,
        callerFunction,
        callerLine));

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        it->second.pointerValueGet()->actionList.push_back(data);
    }
    else
    {
        ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
        objectData.pointerValueGet()->objectRetainCountSaved = objectRetainCount - 1;
        objectData.pointerValueGet()->actionList.push_back(data);
        _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;
    }

    if(_ZFP_ZFLeakTestActivatingSection->param.verboseMode())
    {
        zfstring s = zfstringWithFormat(zfText("[ZFLeakTest] %s(%2zi) %s %s, info: %s\n"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(_ZFP_ZFLeakTestAction::e_Retain),
            objectRetainCount,
            obj->objectInfoOfInstance().cString(),
            zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine)),
            obj->objectInfo().cString());
        _ZFP_ZFLeakTestActivatingSection->output(s.cString());
    }
}

// ============================================================
// objectOnRelease
void _ZFP_ZFLeakTestLogBeforeRelease(ZF_IN ZFObject *obj,
                                     ZF_IN const zfcharA *callerFile,
                                     ZF_IN const zfcharA *callerFunction,
                                     ZF_IN zfindex callerLine)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull || obj == zfnull)
    {
        return ;
    }

    zfindex objectRetainCount = obj->objectRetainCount() - 1;

    ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> data(zfnew(_ZFP_ZFLeakTestActionData,
        _ZFP_ZFLeakTestAction::e_Release,
        objectRetainCount,
        callerFile,
        callerFunction,
        callerLine));

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        it->second.pointerValueGet()->actionList.push_back(data);
    }
    else
    {
        ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
        objectData.pointerValueGet()->objectRetainCountSaved = objectRetainCount + 1;
        objectData.pointerValueGet()->actionList.push_back(data);
        _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;

        it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    }

    if(_ZFP_ZFLeakTestActivatingSection->param.verboseMode())
    {
        zfstring s = zfstringWithFormat(zfText("[ZFLeakTest] %s(%2zi) %s %s, info: %s\n"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(_ZFP_ZFLeakTestAction::e_Release),
            objectRetainCount,
            obj->objectInfoOfInstance().cString(),
            zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine)),
            obj->objectInfo().cString());
        _ZFP_ZFLeakTestActivatingSection->output(s.cString());
    }
}

// ============================================================
// onAutoRelease
void _ZFP_ZFLeakTestLogAutoReleaseAdd(ZF_IN ZFObject *obj,
                                      ZF_IN const zfcharA *callerFile,
                                      ZF_IN const zfcharA *callerFunction,
                                      ZF_IN zfindex callerLine)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull || obj == zfnull)
    {
        return ;
    }

    zfindex objectRetainCount = obj->objectRetainCount();

    ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> data(zfnew(_ZFP_ZFLeakTestActionData,
        _ZFP_ZFLeakTestAction::e_AutoRelease,
        objectRetainCount,
        callerFile,
        callerFunction,
        callerLine));

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        it->second.pointerValueGet()->actionList.push_back(data);
    }
    else
    {
        ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
        objectData.pointerValueGet()->objectRetainCountSaved = objectRetainCount;
        objectData.pointerValueGet()->actionList.push_back(data);
        _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;
    }

    if(_ZFP_ZFLeakTestActivatingSection->param.verboseMode())
    {
        zfstring s = zfstringWithFormat(zfText("[ZFLeakTest] %s(%2zi) %s %s, info: %s\n"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(_ZFP_ZFLeakTestAction::e_AutoRelease),
            objectRetainCount,
            obj->objectInfoOfInstance().cString(),
            zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine)),
            obj->objectInfo().cString());
        _ZFP_ZFLeakTestActivatingSection->output(s.cString());
    }
}

// ============================================================
// onAutoReleaseBeforeRelease
void _ZFP_ZFLeakTestLogAutoReleaseBeforeRelease(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull || obj == zfnull)
    {
        return ;
    }

    zfindex objectRetainCount = obj->objectRetainCount() - 1;

    ZFCorePointerForObject<_ZFP_ZFLeakTestActionData *> data(zfnew(_ZFP_ZFLeakTestActionData,
        _ZFP_ZFLeakTestAction::e_OnAutoRelease,
        objectRetainCount,
        zfnull,
        zfnull,
        0));

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        it->second.pointerValueGet()->actionList.push_back(data);
    }
    else
    {
        ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
        objectData.pointerValueGet()->objectRetainCountSaved = objectRetainCount + 1;
        objectData.pointerValueGet()->actionList.push_back(data);
        _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;

        it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    }

    if(_ZFP_ZFLeakTestActivatingSection->param.verboseMode())
    {
        zfstring s = zfstringWithFormat(zfText("[ZFLeakTest] %s(%2zi) %s, info: %s\n"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(_ZFP_ZFLeakTestAction::e_OnAutoRelease),
            objectRetainCount,
            obj->objectInfoOfInstance().cString(),
            obj->objectInfo().cString());
        _ZFP_ZFLeakTestActivatingSection->output(s.cString());
    }
}

// ============================================================
// objectOnDealloc
void _ZFP_ZFLeakTestLogBeforeDealloc(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    _ZFP_ZFLeakTestGlobalExcludeInstances.erase(obj);
    if(_ZFP_ZFLeakTestActivatingSection == zfnull)
    {
        return ;
    }

    ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectDataTmp;
    zfbool needCheckOverRelease = _ZFP_ZFLeakTestActivatingSection->param.checkOverRelease();
    for(zfindex i = 0; i < _ZFP_ZFLeakTestGlobalData.size(); ++i)
    {
        _ZFP_ZFLeakTestSectionData *section = _ZFP_ZFLeakTestGlobalData.at((zfstlsize)i).pointerValueGet();
        _ZFP_ZFLeakTestObjectDataMapType::iterator it = section->data.find(obj);
        if(it != section->data.end())
        {
            if(needCheckOverRelease)
            {
                if(objectDataTmp.pointerValueGet() == zfnull)
                {
                    objectDataTmp = it->second;
                    if(objectDataTmp.pointerValueGet()->objectRetainCountSaved + objectDataTmp.pointerValueGet()->objectCacheCountFix
                        && _ZFP_ZFLeakTestGlobalData.size() == 1)
                    {
                        needCheckOverRelease = zffalse;
                    }
                }
                else
                {
                    const _ZFP_ZFLeakTestActionDataListType &actionListTmp = it->second.pointerValueGet()->actionList;
                    for(_ZFP_ZFLeakTestActionDataListType::const_iterator itAction = actionListTmp.begin();
                        itAction != actionListTmp.end();
                        ++itAction)
                    {
                        objectDataTmp.pointerValueGet()->actionList.push_back(*itAction);
                    }
                }
            }
            section->data.erase(it);
        }
    }
    if(needCheckOverRelease && objectDataTmp.pointerValueGet() != zfnull
        && objectDataTmp.pointerValueGet()->objectRetainCountSaved + objectDataTmp.pointerValueGet()->objectCacheCountFix != 0
        && _ZFP_ZFLeakTestGlobalExcludeNames.find(obj->classData()->className()) == _ZFP_ZFLeakTestGlobalExcludeNames.end()
        && _ZFP_ZFLeakTestGlobalExcludeInstances.find(obj) == _ZFP_ZFLeakTestGlobalExcludeInstances.end()
        && !obj->objectIsInternal())
    {
        _ZFP_ZFLeakTestActivatingSection->output(zfText("================== ZFLeakTest message begin =================\n"));
        _ZFP_ZFLeakTestActivatingSection->output(zfText("| possible over-released object\n"));
        _ZFP_ZFLeakTestPrintLocationInfo(_ZFP_ZFLeakTestActivatingSection);
        _ZFP_ZFLeakTestActivatingSection->output(zfText("|-----------------------------------------------------------\n"));
        _ZFP_ZFLeakTestPrintObjectStatus(_ZFP_ZFLeakTestActivatingSection, obj, objectDataTmp);
        _ZFP_ZFLeakTestActivatingSection->output(zfText("================== ZFLeakTest message  end  =================\n"));
    }
}

// ============================================================
void _ZFP_ZFLeakTestLogCacheAdd(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull)
    {
        return ;
    }

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        ++(it->second.pointerValueGet()->objectCacheCountFix);
    }
    else
    {
        ZFCorePointerForObject<_ZFP_ZFLeakTestObjectData *> objectData(zfnew(_ZFP_ZFLeakTestObjectData));
        objectData.pointerValueGet()->objectRetainCountSaved = obj->objectRetainCount();
        objectData.pointerValueGet()->objectCacheCountFix = 1;
        _ZFP_ZFLeakTestActivatingSection->data[obj] = objectData;
    }
}
void _ZFP_ZFLeakTestLogCacheRemove(ZF_IN ZFObject *obj)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull)
    {
        return ;
    }

    _ZFP_ZFLeakTestObjectDataMapType::iterator it = _ZFP_ZFLeakTestActivatingSection->data.find(obj);
    if(it != _ZFP_ZFLeakTestActivatingSection->data.end())
    {
        if(it->second.pointerValueGet()->objectCacheCountFix > 0)
        {
            --(it->second.pointerValueGet()->objectCacheCountFix);
        }
    }
}

// ============================================================
// other
void _ZFP_ZFLeakTestBegin(ZF_IN const zfcharA *callerFile,
                          ZF_IN const zfcharA *callerFunction,
                          ZF_IN zfindex callerLine,
                          ZF_IN_OPT const ZFLeakTestBeginParam &param /* = ZFLeakTestBeginParam() */)
{
    ZFCoreMutexLocker();
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(!_ZFP_ZFLeakTestEnable)
    {
        return ;
    }

    ZFCorePointerForObject<_ZFP_ZFLeakTestSectionData *> _sectionData(zfnew(_ZFP_ZFLeakTestSectionData));
    _ZFP_ZFLeakTestSectionData *sectionData = _sectionData.pointerValueGet();
    sectionData->callerFile = callerFile;
    sectionData->callerFunction = callerFunction;
    sectionData->callerLine = callerLine;
    sectionData->param = param;
    _ZFP_ZFLeakTestGlobalData.push_back(_sectionData);
    _ZFP_ZFLeakTestActivatingSection = _ZFP_ZFLeakTestGlobalData.back().pointerValueGet();
    _ZFP_ZFLeakTestEnableCache = (_ZFP_ZFLeakTestActivatingSection != zfnull);
}

void _ZFP_ZFLeakTestEnd(void)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    ZFCoreMutexLocker();
    if(_ZFP_ZFLeakTestGlobalData.size() == 1)
    {
        _ZFP_ZFLeakTestGlobalData.clear();
        _ZFP_ZFLeakTestActivatingSection = zfnull;
        _ZFP_ZFLeakTestEnableCache = (_ZFP_ZFLeakTestActivatingSection != zfnull);
    }
    else if(_ZFP_ZFLeakTestGlobalData.size() > 1)
    {
        _ZFP_ZFLeakTestSectionData *sectionCur = _ZFP_ZFLeakTestGlobalData.at(_ZFP_ZFLeakTestGlobalData.size() - 1).pointerValueGet();
        _ZFP_ZFLeakTestSectionData *sectionOld = _ZFP_ZFLeakTestGlobalData.at(_ZFP_ZFLeakTestGlobalData.size() - 2).pointerValueGet();

        for(_ZFP_ZFLeakTestObjectDataMapType::const_iterator itObjectDataMapCur = sectionCur->data.begin();
            itObjectDataMapCur != sectionCur->data.end();
            ++itObjectDataMapCur)
        {
            _ZFP_ZFLeakTestObjectDataMapType::iterator itObjectDataMapOld = sectionOld->data.find(itObjectDataMapCur->first);
            if(itObjectDataMapOld == sectionOld->data.end())
            {
                sectionOld->data[itObjectDataMapCur->first] = itObjectDataMapCur->second;
                continue;
            }

            _ZFP_ZFLeakTestObjectData *objectDataCur = itObjectDataMapCur->second.pointerValueGet();
            _ZFP_ZFLeakTestObjectData *objectDataOld = itObjectDataMapOld->second.pointerValueGet();
            objectDataOld->objectCacheCountFix += objectDataCur->objectCacheCountFix;
            for(_ZFP_ZFLeakTestActionDataListType::const_iterator itTmp = objectDataCur->actionList.begin();
                itTmp != objectDataCur->actionList.end();
                ++itTmp)
            {
                objectDataOld->actionList.push_back(*itTmp);
            }
        }

        _ZFP_ZFLeakTestGlobalData.pop_back();
        _ZFP_ZFLeakTestActivatingSection = _ZFP_ZFLeakTestGlobalData.back().pointerValueGet();
        _ZFP_ZFLeakTestEnableCache = (_ZFP_ZFLeakTestActivatingSection != zfnull);
    }
}

// ============================================================
// ZFLeakTestPrintStatus
void _ZFP_ZFLeakTestPrintLocationInfo(ZF_IN _ZFP_ZFLeakTestSectionData *ownerSection,
                                      ZF_IN_OPT const zfcharA *callerFile /* = zfnull */,
                                      ZF_IN_OPT const zfcharA *callerFunction /* = zfnull */,
                                      ZF_IN_OPT zfindex callerLine /* = 0 */)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    zfstring s = zfstringWithFormat(zfText("|   leak test was started at %s\n"),
        zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(ownerSection->callerFile,
            ownerSection->callerFunction,
            ownerSection->callerLine)));
    if(callerFile != zfnull)
    {
        zfstringAppend(s, zfText("|   is printing at %s\n"),
            zfsCoreA2Z(ZF_CALLER_INFO_DETAIL(callerFile, callerFunction, callerLine)));
    }
    ownerSection->output(s.cString());
}
void _ZFP_ZFLeakTestPrintObjectStatus(ZF_IN _ZFP_ZFLeakTestSectionData *ownerSection,
                                      ZF_IN ZFObject *obj,
                                      ZF_IN _ZFP_ZFLeakTestObjectData *objectData)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    const _ZFP_ZFLeakTestActionDataListType &actionList = objectData->actionList;
    zfstring s = zfstringWithFormat(zfText("| %s retainCount from %zi to %zi%s, info:\n|   %s\n"),
        obj->objectInfoForDebug().cString(),
        objectData->objectRetainCountSaved,
        obj->objectRetainCount(),
        (objectData->objectCacheCountFix != 0)
            ? zfstringWithFormat(zfText(" (cached %d)"), objectData->objectCacheCountFix).cString()
            : zfText(""),
        obj->objectInfo().cString());

    ownerSection->output(s.cString());
    s.removeAll();

    for(_ZFP_ZFLeakTestActionDataListType::const_iterator itData = actionList.begin();
        itData != actionList.end();
        ++itData)
    {
        _ZFP_ZFLeakTestActionData *data = itData->pointerValueGet();
        zfstringAppend(s, zfText("|     %s %2zi"),
            _ZFP_ZFLeakTestAction::EnumNameForValue(data->action),
            data->curRetainCount);
        if(data->action != _ZFP_ZFLeakTestAction::e_OnAutoRelease)
        {
            s += zfText(" ");
            ZF_CALLER_INFO_DETAIL_REF(s, data->callerFile, data->callerFunction, data->callerLine);
        }
        s += zfText("\n");
        ownerSection->output(s.cString());
        s.removeAll();
    }
}
void _ZFP_ZFLeakTestPrintStatus(ZF_IN const zfcharA *callerFile,
                                ZF_IN const zfcharA *callerFunction,
                                ZF_IN zfindex callerLine,
                                ZF_IN_OPT const ZFLeakTestObjectFilter &filter /* = _ZFP_ZFLeakTestObjectFilterNull */)
{
    if(!_ZFP_ZFLeakTestInitFlag)
    {
        return ;
    }

    if(_ZFP_ZFLeakTestActivatingSection == zfnull)
    {
        return ;
    }

    ZFCoreMutexLocker();

    zfindex outputNum = 0;
    for(_ZFP_ZFLeakTestObjectDataMapType::const_iterator it = _ZFP_ZFLeakTestActivatingSection->data.begin();
        it != _ZFP_ZFLeakTestActivatingSection->data.end();
        ++it)
    {
        if(filter.filterCheckActive(it->first)
            && _ZFP_ZFLeakTestGlobalExcludeNames.find(it->first->classData()->className()) == _ZFP_ZFLeakTestGlobalExcludeNames.end()
            && _ZFP_ZFLeakTestGlobalExcludeInstances.find(it->first) == _ZFP_ZFLeakTestGlobalExcludeInstances.end()
            && !it->first->objectIsInternal())
        {
            ZFObject *obj = it->first;
            _ZFP_ZFLeakTestObjectData *objectData = it->second.pointerValueGet();
            if(obj->objectRetainCount() == objectData->objectRetainCountSaved + objectData->objectCacheCountFix
                || (obj->objectRetainCount() < objectData->objectRetainCountSaved + objectData->objectCacheCountFix && !_ZFP_ZFLeakTestActivatingSection->param.checkOverRelease()))
            {
                continue;
            }
            if(outputNum == 0)
            {
                _ZFP_ZFLeakTestActivatingSection->output(zfText("================== ZFLeakTest message begin =================\n"));
                _ZFP_ZFLeakTestActivatingSection->output(zfText("| possible leaked or over-released object\n"));
                _ZFP_ZFLeakTestPrintLocationInfo(_ZFP_ZFLeakTestActivatingSection,
                    callerFile, callerFunction, callerLine);
            }
            ++outputNum;

            _ZFP_ZFLeakTestActivatingSection->output(zfText("|-----------------------------------------------------------\n"));
            _ZFP_ZFLeakTestPrintObjectStatus(_ZFP_ZFLeakTestActivatingSection, obj, objectData);
        }
    }
    if(outputNum > 0)
    {
        _ZFP_ZFLeakTestActivatingSection->output(zfText("================== ZFLeakTest message  end  =================\n"));
    }
}

ZF_NAMESPACE_GLOBAL_END

