/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFAlgorithm;

public class ZFRegExpOption {
    public static final int e_None = ZFRegExpOption.native_e_None();
    private native static int native_e_None();
    public static final int e_IgnoreCase = ZFRegExpOption.native_e_IgnoreCase();
    private native static int native_e_IgnoreCase();
    public static final int e_SingleLine = ZFRegExpOption.native_e_SingleLine();
    private native static int native_e_SingleLine();
    public static final int e_MultiLine = ZFRegExpOption.native_e_MultiLine();
    private native static int native_e_MultiLine();

    public static final int EnumDefault = ZFRegExpOption.native_EnumDefault();
    private native static int native_EnumDefault();
}

