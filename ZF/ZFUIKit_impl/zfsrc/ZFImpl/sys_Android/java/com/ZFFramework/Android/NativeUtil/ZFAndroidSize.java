/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeUtil;

import android.annotation.SuppressLint;

public final class ZFAndroidSize {
    public static final ZFAndroidSize Zero = new ZFAndroidSize();

    public int width = 0;
    public int height = 0;

    @SuppressLint("DefaultLocale")
    @Override
    public String toString() {
        return String.format("(%d, %d)", this.width, this.height);
    }

    public ZFAndroidSize() {
    }
    public ZFAndroidSize(int width, int height) {
        this.width = width;
        this.height = height;
    }
    public ZFAndroidSize(ZFAndroidSize size) {
        if(size != null) {
            this.width = size.width;
            this.height = size.height;
        }
    }

    public void set(int width, int height) {
        this.width = width;
        this.height = height;
    }
    public void set(ZFAndroidSize size) {
        if(size == null) {
            this.width = 0;
            this.height = 0;
        }
        else {
            this.width = size.width;
            this.height = size.height;
        }
    }
}
