/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUITextEditKeyboardReturnAction {
    public static final int e_None = ZFUITextEditKeyboardReturnAction.native_e_None();
    private native static int native_e_None();
    public static final int e_FocusNext = ZFUITextEditKeyboardReturnAction.native_e_FocusNext();
    private native static int native_e_FocusNext();
    public static final int e_HideKeyboard = ZFUITextEditKeyboardReturnAction.native_e_HideKeyboard();
    private native static int native_e_HideKeyboard();

    public static final int EnumDefault = ZFUITextEditKeyboardReturnAction.native_EnumDefault();
    private native static int native_EnumDefault();
}

