/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIMouseAction {
    public static final int e_MouseDown = ZFUIMouseAction.native_e_MouseDown();
    private static native int native_e_MouseDown();
    public static final int e_MouseMove = ZFUIMouseAction.native_e_MouseMove();
    private static native int native_e_MouseMove();
    public static final int e_MouseUp = ZFUIMouseAction.native_e_MouseUp();
    private static native int native_e_MouseUp();
    public static final int e_MouseCancel = ZFUIMouseAction.native_e_MouseCancel();
    private static native int native_e_MouseCancel();
    public static final int e_MouseHoverEnter = ZFUIMouseAction.native_e_MouseHoverEnter();
    private static native int native_e_MouseHoverEnter();
    public static final int e_MouseHover = ZFUIMouseAction.native_e_MouseHover();
    private static native int native_e_MouseHover();
    public static final int e_MouseHoverExit = ZFUIMouseAction.native_e_MouseHoverExit();
    private static native int native_e_MouseHoverExit();

    public static final int EnumDefault = ZFUIMouseAction.native_EnumDefault();
    private native static int native_EnumDefault();
}

