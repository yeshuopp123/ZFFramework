/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeUtil;

import android.content.Context;
import android.util.DisplayMetrics;
import android.view.WindowManager;

public class ZFAndroidUI {
    private static DisplayMetrics _displayMetricsCache = new DisplayMetrics();
    public static ZFAndroidSize screenSize(Context context) {
        ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(_displayMetricsCache);
        return new ZFAndroidSize(_displayMetricsCache.widthPixels, _displayMetricsCache.heightPixels);
    }
    public static float screenDensity(Context context) {
        ((WindowManager)context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getMetrics(_displayMetricsCache);
        return _displayMetricsCache.density;
    }
}
