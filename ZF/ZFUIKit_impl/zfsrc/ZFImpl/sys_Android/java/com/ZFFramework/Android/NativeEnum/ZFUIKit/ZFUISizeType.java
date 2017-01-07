/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUISizeType {
    public static final int e_Wrap = ZFUISizeType.native_e_Wrap();
    private static native int native_e_Wrap();
    public static final int e_Fill = ZFUISizeType.native_e_Fill();
    private static native int native_e_Fill();

    public static final int EnumDefault = ZFUISizeType.native_EnumDefault();
    private native static int native_EnumDefault();
}

