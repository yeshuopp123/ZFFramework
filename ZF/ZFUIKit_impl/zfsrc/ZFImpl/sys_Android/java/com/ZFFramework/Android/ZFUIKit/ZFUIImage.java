/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import com.ZFFramework.Android.NativeUtil.ZFAndroidBuffer;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;


public final class ZFUIImage {
    @SuppressWarnings("deprecation")
    public static Object native_nativeImageFromInput(Object buf) {
        Bitmap bitmap = BitmapFactory.decodeStream(((ZFAndroidBuffer)buf).toInputStream());
        return new BitmapDrawable(bitmap);
    }
    public static Object native_nativeImageToOutput(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        ZFAndroidBuffer buffer = new ZFAndroidBuffer(1024);
        src.getBitmap().compress(CompressFormat.PNG, 1, buffer.toOutputStream());
        buffer.bufferReadyToRead();
        return buffer;
    }
    @SuppressWarnings("deprecation")
    public static Object native_nativeImageCopy(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        Bitmap dst = src.getBitmap().copy(Bitmap.Config.ARGB_8888, true);
        return new BitmapDrawable(dst);
    }
    public static Object native_nativeImageSize(Object nativeImage) {
        BitmapDrawable src = (BitmapDrawable)nativeImage;
        return new ZFAndroidSize(src.getBitmap().getWidth(), src.getBitmap().getHeight());
    }
}
