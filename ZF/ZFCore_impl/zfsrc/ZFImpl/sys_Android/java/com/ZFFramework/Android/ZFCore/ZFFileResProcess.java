/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import com.ZFFramework.Android.ZF.ZFMainEntry;
import android.content.res.AssetManager;

public final class ZFFileResProcess {
    private static final String _resPrefix = "zfres/";
    private static final String _resPostfix = ".mp2";

    public static String[] native_listAssets(String resPath) {
        try {
            if(resPath.length() > 0 && resPath.charAt(resPath.length() - 1) == '/') {
                resPath = resPath.substring(0, resPath.length() - 1);
            }
            return ZFMainEntry.assetManager().list(resPath);
        }
        catch(IOException e) {
            return null;
        }
    }

    public static String native_resCp(String resPath,
                                      String dstPath,
                                      boolean isRecursive,
                                      boolean isForce) {
        resPath = String.format("%s%s", _resPrefix, resPath);
        AssetManager assetManager = ZFMainEntry.assetManager();
        InputStream dirCheck = native_resCp_checkFileOrDir(assetManager, resPath);
        if(dirCheck == null) {
            String tmp = native_resCp_dir(assetManager, resPath, dstPath, isRecursive, isForce);
            if(tmp != null) {
                return tmp.substring(_resPrefix.length());
            }
            else {
                return null;
            }
        }
        else {
            try {
                dirCheck.close();
            }
            catch(IOException e) {
                e.printStackTrace();
            }
            if(native_resCp_file(assetManager, resPath, dstPath, isRecursive, isForce)) {
                return null;
            }
            else {
                String errPos = resPath.substring(_resPrefix.length());
                if(errPos.indexOf(_resPostfix) == errPos.length() - _resPostfix.length()) {
                    errPos = errPos.substring(0, errPos.length() - _resPostfix.length());
                }
                return errPos;
            }
        }
    }

    private static String native_resCp_dir(AssetManager assetManager,
                                           String resPath,
                                           String dstPath,
                                           boolean isRecursive,
                                           boolean isForce) {
        if(!isRecursive) {
            return resPath;
        }
        File dstFd = new File(dstPath);
        if(isForce) {
            try {
                native_resCp_rm(dstFd);
            }
            catch(Exception e) {
                e.printStackTrace();
            }
        }
        String[] files = null;
        try {
            files = assetManager.list(resPath);
            dstFd.mkdirs();
        }
        catch(Exception e) {
            e.printStackTrace();
            return resPath;
        }
        for(String file : files) {
            String fromAbsPath = String.format("%s/%s", resPath, file);
            InputStream dirCheck = native_resCp_checkFileOrDir(assetManager, fromAbsPath);
            String toAbsPath = String.format("%s/%s", dstPath, file);
            if(dirCheck == null) {
                String tmp = native_resCp_dir(assetManager, fromAbsPath, toAbsPath, isRecursive, isForce);
                if(tmp != null) {
                    return tmp;
                }
            }
            else {
                try {
                    dirCheck.close();
                }
                catch(IOException e) {
                    e.printStackTrace();
                }
                if(toAbsPath.indexOf(_resPostfix) == toAbsPath.length() - _resPostfix.length()) {
                    toAbsPath = toAbsPath.substring(0, toAbsPath.length() - _resPostfix.length());
                }
                if(!native_resCp_file(assetManager, fromAbsPath, toAbsPath, isRecursive, isForce)) {
                    return fromAbsPath;
                }
            }
        }
        return null;
    }

    private static boolean native_resCp_file(AssetManager assetManager,
                                             String resPath,
                                             String dstPath,
                                             boolean isRecursive,
                                             boolean isForce) {
        File dstFd = new File(dstPath);
        if(isForce) {
            try {
                native_resCp_rm(dstFd);
            }
            catch(Exception e) {
                e.printStackTrace();
            }
        }
        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(resPath);
            dstFd.createNewFile();
            out = new FileOutputStream(dstPath);
            byte[] buffer = new byte[1024];
            int read;
            while((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
            return true;
        }
        catch(Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    private static void native_resCp_rm(File fileOrDirectory) {
        if(fileOrDirectory.isDirectory()) {
            for(File child : fileOrDirectory.listFiles()) {
                native_resCp_rm(child);
            }
        }
        fileOrDirectory.delete();
    }
    private static InputStream native_resCp_checkFileOrDir(AssetManager assetManager,
                                                           String resPath) {
        InputStream ret = null;
        try {
            ret = assetManager.open(resPath);
            if(ret == null) {
                ret = assetManager.open(String.format("%s%s", resPath, _resPostfix));
            }
        }
        catch(Exception e) {
            // nothing to do
        }
        return ret;
    }
}
