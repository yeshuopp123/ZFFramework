/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFStringEncoding {
    public static final int e_UTF8 = ZFStringEncoding.native_e_UTF8();
    private native static int native_e_UTF8();
    public static final int e_UTF16LE = ZFStringEncoding.native_e_UTF16LE();
    private native static int native_e_UTF16LE();
    public static final int e_UTF16BE = ZFStringEncoding.native_e_UTF16BE();
    private native static int native_e_UTF16BE();
    public static final int e_UTF16 = ZFStringEncoding.native_e_UTF16();
    private native static int native_e_UTF16();

    public static final int EnumDefault = ZFStringEncoding.native_EnumDefault();
    private native static int native_EnumDefault();
}

