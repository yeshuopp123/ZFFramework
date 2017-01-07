/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFAlgorithm;

public class ZFJsonType {
    public static final int e_JsonNull = ZFJsonType.native_e_JsonNull();
    private native static int native_e_JsonNull();
    public static final int e_JsonValue = ZFJsonType.native_e_JsonValue();
    private native static int native_e_JsonValue();
    public static final int e_JsonObject = ZFJsonType.native_e_JsonObject();
    private native static int native_e_JsonObject();
    public static final int e_JsonArray = ZFJsonType.native_e_JsonArray();
    private native static int native_e_JsonArray();

    public static final int EnumDefault = ZFJsonType.native_EnumDefault();
    private native static int native_EnumDefault();
}

