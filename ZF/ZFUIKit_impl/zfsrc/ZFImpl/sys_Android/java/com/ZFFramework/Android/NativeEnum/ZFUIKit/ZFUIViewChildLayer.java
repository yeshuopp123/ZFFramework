/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIViewChildLayer {
    public static final int e_Normal = ZFUIViewChildLayer.native_e_Normal();
    private native static int native_e_Normal();
    public static final int e_Impl = ZFUIViewChildLayer.native_e_Impl();
    private native static int native_e_Impl();
    public static final int e_Background = ZFUIViewChildLayer.native_e_Background();
    private native static int native_e_Background();
    public static final int e_Foreground = ZFUIViewChildLayer.native_e_Foreground();
    private native static int native_e_Foreground();

    public static final int EnumDefault = ZFUIViewChildLayer.native_EnumDefault();
    private native static int native_EnumDefault();
}

