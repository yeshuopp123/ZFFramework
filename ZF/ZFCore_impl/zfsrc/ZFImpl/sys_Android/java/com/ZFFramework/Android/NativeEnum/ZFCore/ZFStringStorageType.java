/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFStringStorageType {
    public static final int e_NativeString = ZFStringStorageType.native_e_NativeString();
    private native static int native_e_NativeString();
    public static final int e_Copy = ZFStringStorageType.native_e_Copy();
    private native static int native_e_Copy();
    public static final int e_CharBufferRef = ZFStringStorageType.native_e_CharBufferRef();
    private native static int native_e_CharBufferRef();

    public static final int EnumDefault = ZFStringStorageType.native_EnumDefault();
    private native static int native_EnumDefault();
}

