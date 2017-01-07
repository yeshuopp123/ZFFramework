/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import com.ZFFramework.Android.NativeUtil.ZFAndroidRect;
import android.view.View;

public class ZFUIViewPositionOnScreen {
    private static int[] _viewPositionOnScreenCache = new int[2];
    public static Object native_viewPositionOnScreen(Object nativeView) {
        View nativeViewTmp = (View)nativeView;
        nativeViewTmp.getLocationOnScreen(_viewPositionOnScreenCache);
        return new ZFAndroidRect(_viewPositionOnScreenCache[0], _viewPositionOnScreenCache[1], nativeViewTmp.getWidth(), nativeViewTmp.getHeight());
    }
}
