/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
#include "ZFImpl_default_ZF.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFString.h"
#include "ZFCore/ZFArray.h"

#if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS && !ZF_ENV_sys_Qt

#if !ZF_ENV_sys_WindowsCE
int main(int argc, zfcharA **argv)
#else // #if ZF_ENV_sys_WindowsCE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
#endif // #if ZF_ENV_sys_WindowsCE #else
{

#if !ZF_ENV_sys_WindowsCE
    return ZFMainCommonEntry(argc, argv);
#else
    ZFFrameworkInit();
    ZFArrayEditable *params = zfAllocWithoutLeakTest(ZFArrayEditable);

    ZFCoreArray<zfstring> tmpList;
    ZFCoreArgSplit(tmpList, ZFStringW2Z(lpCmdLine));
    for(zfindex i = 0; i < tmpList.count(); ++i)
    {
        ZFString *s = zfAllocWithoutLeakTest(ZFString, tmpList.get(i).cString());
        params->add(s);
        zfReleaseWithoutLeakTest(s);
    }

    zfint result = ZFMainExecute(params);
    zfReleaseWithoutLeakTest(params);
    ZFFrameworkCleanup();
    return result;
#endif // #if ZF_ENV_sys_WindowsCE #else
}

#endif // #if !ZF_ENV_sys_Android && !ZF_ENV_sys_iOS

