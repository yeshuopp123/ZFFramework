/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFCompareResult {
    public static final int ZFCompareUncomparable = ZFCompareResult.native_ZFCompareUncomparable();
    private native static int native_ZFCompareUncomparable();
    public static final int ZFCompareSmaller = ZFCompareResult.native_ZFCompareSmaller();
    private native static int native_ZFCompareSmaller();
    public static final int ZFCompareTheSame = ZFCompareResult.native_ZFCompareTheSame();
    private native static int native_ZFCompareTheSame();
    public static final int ZFCompareGreater = ZFCompareResult.native_ZFCompareGreater();
    private native static int native_ZFCompareGreater();
}

