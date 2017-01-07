/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZF2048;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import com.ZFFramework.Android.ZF.ZFMainEntry;

public class Loader extends Activity {
    static {
        System.loadLibrary("stlport_shared");

        System.loadLibrary("ZFCore");
        System.loadLibrary("ZFAlgorithm");
        System.loadLibrary("ZFUtility");
        System.loadLibrary("ZFUIKit");
        System.loadLibrary("ZFAddition");
        System.loadLibrary("ZFUIWidget");

        System.loadLibrary("ZF_impl");
        System.loadLibrary("ZF_loader_impl");
        System.loadLibrary("ZFCore_impl");
        System.loadLibrary("ZFAlgorithm_impl");
        System.loadLibrary("ZFUtility_impl");
        System.loadLibrary("ZFUIKit_impl");
        System.loadLibrary("ZFAddition_impl");
        System.loadLibrary("ZFUIWidget_impl");
        System.loadLibrary("ZF2048");
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = new Intent(Loader.this, ZFMainEntry.class);
        startActivity(intent);
        this.finish();
    }
}

