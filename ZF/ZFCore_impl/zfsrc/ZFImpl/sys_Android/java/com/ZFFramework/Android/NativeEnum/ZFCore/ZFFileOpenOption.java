/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFFileOpenOption {
    public static final int e_Create = ZFFileOpenOption.native_e_Create();
    private native static int native_e_Create();
    public static final int e_Read = ZFFileOpenOption.native_e_Read();
    private native static int native_e_Read();
    public static final int e_Write = ZFFileOpenOption.native_e_Write();
    private native static int native_e_Write();
    public static final int e_Append = ZFFileOpenOption.native_e_Append();
    private native static int native_e_Append();

    public static final int EnumDefault = ZFFileOpenOption.native_EnumDefault();
    private native static int native_EnumDefault();
}

