/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUITextEditKeyboardType {
    public static final int e_Normal = ZFUITextEditKeyboardType.native_e_Normal();
    private native static int native_e_Normal();
    public static final int e_CharBased = ZFUITextEditKeyboardType.native_e_CharBased();
    private native static int native_e_CharBased();
    public static final int e_PhonePad = ZFUITextEditKeyboardType.native_e_PhonePad();
    private native static int native_e_PhonePad();
    public static final int e_NumberPad = ZFUITextEditKeyboardType.native_e_NumberPad();
    private native static int native_e_NumberPad();

    public static final int EnumDefault = ZFUITextEditKeyboardType.native_EnumDefault();
    private native static int native_EnumDefault();
}

