/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUITextAppearance {
    public static final int e_Normal = ZFUITextAppearance.native_e_Normal();
    private native static int native_e_Normal();
    public static final int e_Bold = ZFUITextAppearance.native_e_Bold();
    private native static int native_e_Bold();
    public static final int e_Italic = ZFUITextAppearance.native_e_Italic();
    private native static int native_e_Italic();
    public static final int e_BoldItalic = ZFUITextAppearance.native_e_BoldItalic();
    private native static int native_e_BoldItalic();

    public static final int EnumDefault = ZFUITextAppearance.native_EnumDefault();
    private native static int native_EnumDefault();
}

