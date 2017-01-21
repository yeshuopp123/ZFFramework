/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFValueType {
    public static final int e_bool = ZFValueType.native_e_bool();
    private native static int native_e_bool();
    public static final int e_char = ZFValueType.native_e_char();
    private native static int native_e_char();
    public static final int e_int = ZFValueType.native_e_int();
    private native static int native_e_int();
    public static final int e_unsignedInt = ZFValueType.native_e_unsignedInt();
    private native static int native_e_unsignedInt();
    public static final int e_index = ZFValueType.native_e_index();
    private native static int native_e_index();
    public static final int e_float = ZFValueType.native_e_float();
    private native static int native_e_float();
    public static final int e_double = ZFValueType.native_e_double();
    private native static int native_e_double();
    public static final int e_longDouble = ZFValueType.native_e_longDouble();
    private native static int native_e_longDouble();
    public static final int e_time = ZFValueType.native_e_time();
    private native static int native_e_time();
    public static final int e_flags = ZFValueType.native_e_flags();
    private native static int native_e_flags();
    public static final int e_identity = ZFValueType.native_e_identity();
    private native static int native_e_identity();
    public static final int e_serializableData = ZFValueType.native_e_serializableData();
    private native static int native_e_serializableData();

    public static final int EnumDefault = ZFValueType.native_EnumDefault();
    private native static int native_EnumDefault();
}

