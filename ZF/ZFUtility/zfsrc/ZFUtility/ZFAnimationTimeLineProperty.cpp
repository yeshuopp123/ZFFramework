/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFAnimationTimeLineProperty.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationTimeLinePropertyPrivate
zfclassNotPOD _ZFP_ZFAnimationTimeLinePropertyPrivate
{
public:
    ZFCoreArrayPOD<ZFTimeLineProperty *> steps;
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationTimeLineProperty)

// ============================================================
// serialize
zfbool ZFAnimationTimeLineProperty::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                                    ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */,
                                                                    ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHintToAppend, outErrorPos)) {return zffalse;}

    this->stepRemoveAll();

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFAnimationTimeLineProperty_step))
        {
            zfautoObject step;
            if(!ZFObjectFromSerializableData(step, categoryData, outErrorHintToAppend, outErrorPos))
            {
                return zffalse;
            }
            if(step == zfautoObjectNull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHintToAppend, outErrorPos, categoryData,
                    zfText("invalid step: %s"), ZFObjectInfoOfInstance(step.toObject()).cString());
                return zffalse;
            }
            this->stepAdd(ZFCastZFObjectUnchecked(ZFTimeLineProperty *, step));

            categoryData.resolveMark();
        }
    }
    return zftrue;
}
zfbool ZFAnimationTimeLineProperty::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                                  ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                                  ZF_OUT_OPT zfstring *outErrorHintToAppend /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHintToAppend)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    if(ref == zfnull)
    {
        for(zfindex i = 0; i < d->steps.count(); ++i)
        {
            ZFSerializableData stepData;
            if(!ZFObjectToSerializableData(stepData, d->steps[i], outErrorHintToAppend))
            {
                return zffalse;
            }
            stepData.categorySet(ZFSerializableKeyword_ZFAnimationTimeLineProperty_step);
            serializableData.elementAdd(stepData);
        }
    }
    else
    {
        zfbool mismatch = zffalse;
        if(d->steps.count() != ref->d->steps.count())
        {
            mismatch = zftrue;
        }
        if(!mismatch)
        {
            for(zfindex i = 0; i < d->steps.count(); ++i)
            {
                if(d->steps[i]->objectCompare(ref->d->steps[i]) != ZFCompareTheSame)
                {
                    mismatch = zftrue;
                    break;
                }
            }
        }
        if(mismatch)
        {
            ZFSerializableUtil::errorOccurred(outErrorHintToAppend,
                zfText("steps mismatch: %s, and: %s"),
                    d->steps.objectInfoOfContent(ZFCoreElementInfoGetter<ZFTimeLineProperty *>::elementInfoGetter).cString(),
                    ref->d->steps.objectInfoOfContent(ZFCoreElementInfoGetter<ZFTimeLineProperty *>::elementInfoGetter).cString());
            return zffalse;
        }
    }

    return zftrue;
}

// ============================================================
// object
ZFObject *ZFAnimationTimeLineProperty::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFAnimationTimeLinePropertyPrivate);
    return this;
}
void ZFAnimationTimeLineProperty::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFAnimationTimeLineProperty::objectOnDeallocPrepare(void)
{
    this->aniStop();
    this->stepRemoveAll();
    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFAnimationTimeLineProperty::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , (zfidentity)d->steps.count()
        , (zfidentity)(d->steps.isEmpty() ? 0 : d->steps[0]->objectHash())
        );
}
ZFCompareResult ZFAnimationTimeLineProperty::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(d->steps.count() != another->d->steps.count())
    {
        return ZFCompareUncomparable;
    }
    for(zfindex i = 0; i < d->steps.count(); ++i)
    {
        if(d->steps[i]->objectCompare(another->d->steps[i]) != ZFCompareTheSame)
        {
            return ZFCompareUncomparable;
        }
    }
    return ZFCompareTheSame;
}

// ============================================================
// property change step
void ZFAnimationTimeLineProperty::stepAdd(ZF_IN ZFTimeLineProperty *step)
{
    zfCoreAssert(!this->aniRunning());
    zfCoreAssert(step != zfnull);
    d->steps.add(zfRetain(step));
}
void ZFAnimationTimeLineProperty::stepRemove(ZF_IN ZFTimeLineProperty *step)
{
    zfCoreAssert(!this->aniRunning());
    zfindex index = d->steps.find(step, ZFComparerCheckEqualOnly);
    if(index != zfindexMax)
    {
        d->steps.remove(index);
        zfRelease(step);
    }
}
void ZFAnimationTimeLineProperty::stepRemoveAtIndex(ZF_IN zfindex index)
{
    zfCoreAssert(!this->aniRunning());
    ZFTimeLineProperty *step = d->steps[index];
    d->steps.remove(index);
    zfRelease(step);
}
void ZFAnimationTimeLineProperty::stepRemoveAll(void)
{
    zfCoreAssert(!this->aniRunning());
    if(!d->steps.isEmpty())
    {
        ZFCoreArrayPOD<ZFTimeLineProperty *> tmp;
        tmp.copyFrom(d->steps);
        d->steps.removeAll();
        for(zfindex i = 0; i < tmp.count(); ++i)
        {
            zfRelease(tmp[i]);
        }
    }
}
zfindex ZFAnimationTimeLineProperty::stepCount(void)
{
    return d->steps.count();
}
ZFTimeLineProperty *ZFAnimationTimeLineProperty::stepAtIndex(ZF_IN zfindex index)
{
    return d->steps.get(index);
}

// ============================================================
// start stop
zfbool ZFAnimationTimeLineProperty::aniImplCheckValid(void)
{
    if(!zfsuper::aniImplCheckValid() || this->aniTarget() == zfnull)
    {
        return zffalse;
    }
    if(!d->steps.isEmpty())
    {
        for(zfindex i = 0; i < d->steps.count(); ++i)
        {
            const ZFProperty *property = d->steps[i]->ownerProperty();
            if(property == zfnull || !this->aniTarget()->classData()->classIsTypeOf(property->ownerClass()))
            {
                return zffalse;
            }
        }
    }
    return zftrue;
}

void ZFAnimationTimeLineProperty::aniOnStart(void)
{
    zfidentity aniId = this->aniId();
    zfsuper::aniOnStart();
    for(zfindex i = 0; i < d->steps.count() && aniId == this->aniId(); ++i)
    {
        d->steps[i]->stateSave(this->aniTarget());
    }
}
void ZFAnimationTimeLineProperty::aniOnStop(void)
{
    zfidentity aniId = this->aniId();
    for(zfindex i = 0; i < d->steps.count() && aniId == this->aniId(); ++i)
    {
        d->steps[i]->stateRestore(this->aniTarget());
    }
    zfsuper::aniOnStop();
}
void ZFAnimationTimeLineProperty::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    zfidentity aniId = this->aniId();
    zfsuper::aniTimeLineOnUpdate(progress);
    for(zfindex i = 0; i < d->steps.count() && aniId == this->aniId(); ++i)
    {
        d->steps[i]->progressUpdate(this->aniTarget(), progress);
    }
}

ZF_NAMESPACE_GLOBAL_END

