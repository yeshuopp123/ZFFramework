/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFProtocolLevel {
    public static final int e_Default = ZFProtocolLevel.native_e_Default();
    private native static int native_e_Default();
    public static final int e_SystemLow = ZFProtocolLevel.native_e_SystemLow();
    private native static int native_e_SystemLow();
    public static final int e_SystemNormal = ZFProtocolLevel.native_e_SystemNormal();
    private native static int native_e_SystemNormal();
    public static final int e_SystemHigh = ZFProtocolLevel.native_e_SystemHigh();
    private native static int native_e_SystemHigh();
    public static final int e_AppLow = ZFProtocolLevel.native_e_AppLow();
    private native static int native_e_AppLow();
    public static final int e_AppNormal = ZFProtocolLevel.native_e_AppNormal();
    private native static int native_e_AppNormal();
    public static final int e_AppHigh = ZFProtocolLevel.native_e_AppHigh();
    private native static int native_e_AppHigh();

    public static final int EnumDefault = ZFProtocolLevel.native_EnumDefault();
    private native static int native_EnumDefault();
}

