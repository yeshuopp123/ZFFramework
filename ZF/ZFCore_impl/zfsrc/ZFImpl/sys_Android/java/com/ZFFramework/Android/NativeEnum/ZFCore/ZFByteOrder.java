/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFByteOrder {
    public static final int e_LittleEndian = ZFByteOrder.native_e_LittleEndian();
    private native static int native_e_LittleEndian();
    public static final int e_BigEndian = ZFByteOrder.native_e_BigEndian();
    private native static int native_e_BigEndian();

    public static final int EnumDefault = ZFByteOrder.native_EnumDefault();
    private native static int native_EnumDefault();
}

