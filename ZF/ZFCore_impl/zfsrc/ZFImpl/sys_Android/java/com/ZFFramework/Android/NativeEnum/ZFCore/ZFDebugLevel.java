/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFDebugLevel {
    public static final int e_Verbose = ZFDebugLevel.native_e_Verbose();
    private native static int native_e_Verbose();
    public static final int e_Debug = ZFDebugLevel.native_e_Debug();
    private native static int native_e_Debug();
    public static final int e_Info = ZFDebugLevel.native_e_Info();
    private native static int native_e_Info();
    public static final int e_Warning = ZFDebugLevel.native_e_Warning();
    private native static int native_e_Warning();
    public static final int e_Error = ZFDebugLevel.native_e_Error();
    private native static int native_e_Error();
    public static final int e_Assert = ZFDebugLevel.native_e_Assert();
    private native static int native_e_Assert();

    public static final int EnumDefault = ZFDebugLevel.native_EnumDefault();
    private native static int native_EnumDefault();
}

