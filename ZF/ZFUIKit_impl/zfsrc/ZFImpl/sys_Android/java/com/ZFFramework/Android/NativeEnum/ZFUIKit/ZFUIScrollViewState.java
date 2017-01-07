/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUIScrollViewState {
    public static final int e_Idle = ZFUIScrollViewState.native_e_Idle();
    private static native int native_e_Idle();
    public static final int e_Dragging = ZFUIScrollViewState.native_e_Dragging();
    private static native int native_e_Dragging();
    public static final int e_Scrolling = ZFUIScrollViewState.native_e_Scrolling();
    private static native int native_e_Scrolling();

    public static final int EnumDefault = ZFUIScrollViewState.native_EnumDefault();
    private native static int native_EnumDefault();
}

