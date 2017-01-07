/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFResultType {
    public static final int e_Success = ZFResultType.native_e_Success();
    private native static int native_e_Success();
    public static final int e_Fail = ZFResultType.native_e_Fail();
    private native static int native_e_Fail();
    public static final int e_Cancel = ZFResultType.native_e_Cancel();
    private native static int native_e_Cancel();

    public static final int EnumDefault = ZFResultType.native_EnumDefault();
    private native static int native_EnumDefault();
}

