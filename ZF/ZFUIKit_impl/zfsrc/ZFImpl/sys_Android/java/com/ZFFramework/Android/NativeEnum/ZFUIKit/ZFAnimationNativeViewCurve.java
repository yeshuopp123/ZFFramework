/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFUIKit;

public class ZFAnimationNativeViewCurve {
    public static final int e_Linear = ZFAnimationNativeViewCurve.native_e_Linear();
    private native static int native_e_Linear();
    public static final int e_EaseIn = ZFAnimationNativeViewCurve.native_e_EaseIn();
    private native static int native_e_EaseIn();
    public static final int e_EaseOut = ZFAnimationNativeViewCurve.native_e_EaseOut();
    private native static int native_e_EaseOut();
    public static final int e_EaseInOut = ZFAnimationNativeViewCurve.native_e_EaseInOut();
    private native static int native_e_EaseInOut();

    public static final int EnumDefault = ZFAnimationNativeViewCurve.native_EnumDefault();
    private native static int native_EnumDefault();
}

