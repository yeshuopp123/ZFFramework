/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import android.os.Handler;
import android.os.Message;

public final class ZFThreadTaskRequest {
    public static Handler _taskHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFThreadTaskRequest.native_doExecute((Long)msg.obj);
        }
    };
    public static void native_taskRequest(long zfjniPointerToken) {
        _taskHandler.sendMessage(Message.obtain(_taskHandler, 0, Long.valueOf(zfjniPointerToken)));
    }
    private static native void native_doExecute(long zfjniPointerToken);
}
