/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import com.ZFFramework.Android.NativeUtil.ZFAndroidMargin;
import com.ZFFramework.Android.NativeUtil.ZFAndroidNinePatch;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import android.graphics.Bitmap;
import android.graphics.Bitmap.Config;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;

public final class ZFUIImageIO {
    @SuppressWarnings("deprecation")
    public static Object native_imageApplyScale(float imageScale,
                                                Object nativeImage,
                                                Object newSize,
                                                Object ninePatch) {
        ZFAndroidSize newSizeTmp = (ZFAndroidSize)newSize;
        ZFAndroidMargin ninePatchTmp = (ZFAndroidMargin)ninePatch;
        Bitmap bmp = Bitmap.createBitmap(newSizeTmp.width, newSizeTmp.height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        ZFAndroidNinePatch.drawNinePatch(
            ((BitmapDrawable)nativeImage).getBitmap(),
            1, // src and dst always have same scale
            ninePatchTmp.left, ninePatchTmp.top, ninePatchTmp.right, ninePatchTmp.bottom,
            canvas,
            newSizeTmp.width, newSizeTmp.height);
        return new BitmapDrawable(bmp);
    }
    @SuppressWarnings("deprecation")
    public static Object native_imageLoadFromColor(int color,
                                                   Object size) {
        ZFAndroidSize sizeTmp = (ZFAndroidSize)size;
        Bitmap bmp = Bitmap.createBitmap(sizeTmp.width, sizeTmp.height, Config.ARGB_8888);
        Canvas canvas = new Canvas(bmp);
        canvas.drawColor(color);
        return new BitmapDrawable(bmp);
    }
}
