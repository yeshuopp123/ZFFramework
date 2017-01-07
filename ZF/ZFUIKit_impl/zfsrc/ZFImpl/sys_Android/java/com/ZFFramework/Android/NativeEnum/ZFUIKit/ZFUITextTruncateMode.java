/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFUITextTruncateMode {
    public static final int e_Disable = ZFUITextTruncateMode.native_e_Disable();
    private static native int native_e_Disable();
    public static final int e_Head = ZFUITextTruncateMode.native_e_Head();
    private static native int native_e_Head();
    public static final int e_Middle = ZFUITextTruncateMode.native_e_Middle();
    private static native int native_e_Middle();
    public static final int e_Tail = ZFUITextTruncateMode.native_e_Tail();
    private static native int native_e_Tail();

    public static final int EnumDefault = ZFUITextTruncateMode.native_EnumDefault();
    private native static int native_EnumDefault();
}

