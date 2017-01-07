/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFEnvDeviceUIType {
    public static final int e_Desktop = ZFEnvDeviceUIType.native_e_Desktop();
    private native static int native_e_Desktop();
    public static final int e_Handheld = ZFEnvDeviceUIType.native_e_Handheld();
    private native static int native_e_Handheld();
    public static final int e_Wearable = ZFEnvDeviceUIType.native_e_Wearable();
    private native static int native_e_Wearable();
    public static final int e_TV = ZFEnvDeviceUIType.native_e_TV();
    private native static int native_e_TV();

    public static final int EnumDefault = ZFEnvDeviceUIType.native_EnumDefault();
    private native static int native_EnumDefault();
}

