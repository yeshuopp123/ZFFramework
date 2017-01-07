/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIKeyAction {
    public static final int e_KeyDown = ZFUIKeyAction.native_e_KeyDown();
    private static native int native_e_KeyDown();
    public static final int e_KeyRepeat = ZFUIKeyAction.native_e_KeyRepeat();
    private static native int native_e_KeyRepeat();
    public static final int e_KeyUp = ZFUIKeyAction.native_e_KeyUp();
    private static native int native_e_KeyUp();
    public static final int e_KeyCancel = ZFUIKeyAction.native_e_KeyCancel();
    private static native int native_e_KeyCancel();

    public static final int EnumDefault = ZFUIKeyAction.native_EnumDefault();
    private native static int native_EnumDefault();
}

