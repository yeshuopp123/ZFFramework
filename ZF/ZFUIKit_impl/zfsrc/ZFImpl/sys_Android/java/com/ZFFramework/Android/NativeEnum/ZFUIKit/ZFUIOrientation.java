/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIOrientation {
    public static final int e_Left = ZFUIOrientation.native_e_Left();
    private native static int native_e_Left();
    public static final int e_Top = ZFUIOrientation.native_e_Top();
    private native static int native_e_Top();
    public static final int e_Right = ZFUIOrientation.native_e_Right();
    private native static int native_e_Right();
    public static final int e_Bottom = ZFUIOrientation.native_e_Bottom();
    private native static int native_e_Bottom();

    public static final int EnumDefault = ZFUIOrientation.native_EnumDefault();
    private native static int native_EnumDefault();
}

