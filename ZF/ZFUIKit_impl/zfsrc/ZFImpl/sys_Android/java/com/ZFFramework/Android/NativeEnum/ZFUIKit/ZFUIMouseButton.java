/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIMouseButton {
    public static final int e_MouseButtonLeft = ZFUIMouseButton.native_e_MouseButtonLeft();
    private static native int native_e_MouseButtonLeft();
    public static final int e_MouseButtonRight = ZFUIMouseButton.native_e_MouseButtonRight();
    private static native int native_e_MouseButtonRight();
    public static final int e_MouseButtonCenter = ZFUIMouseButton.native_e_MouseButtonCenter();
    private static native int native_e_MouseButtonCenter();

    public static final int EnumDefault = ZFUIMouseButton.native_EnumDefault();
    private native static int native_EnumDefault();
}

