/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import com.ZFFramework.Android.ZF.ZFMainEntry;
import android.os.Environment;

public final class ZFFilePath {
    public static Object native_moduleFilePath() {
        try {
            return ZFMainEntry.appContext().getPackageManager().getApplicationInfo(
                ZFMainEntry.appContext().getPackageName(),
                0).publicSourceDir;
        }
        catch(Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    public static Object native_settingPath() {
        return ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfsetting";
    }
    public static Object native_storagePath() {
        return ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfstorage";
    }
    public static Object native_storageSharedPath() {
        String ret = null;
        if(Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())) {
            ret = Environment.getExternalStorageDirectory().getAbsolutePath();
        }
        else {
            ret = ZFMainEntry.appContext().getFilesDir().getAbsolutePath() + "/zfstorageshared";
        }
        return ret;
    }
    public static Object native_cachePath() {
        return ZFMainEntry.appContext().getCacheDir().getAbsolutePath() + "/zfcache";
    }
}
