/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import java.util.Timer;
import java.util.TimerTask;
import android.os.Handler;
import android.os.Message;

public final class ZFTimer {
    public Timer timer = null;
    public ZFTimerTask timerTask = null;
    public Object zfnativeImpl() {
        return this.timer;
    }

    protected static Object native_nativeTimerCreate() {
        return new ZFTimer();
    }

    private static Handler _mainThreadCallback = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            ZFTimerTask timerTask = (ZFTimerTask)msg.obj;
            if(timerTask._running) {
                ZFTimer.native_notifyTimerActivate(timerTask._zfjniPointerToken);
            }
        }
    };
    protected static class ZFTimerTask extends TimerTask {
        protected boolean _firstTime = true;
        protected boolean _running = true;
        protected long _zfjniPointerToken = 0;
        protected boolean _timerActivateInMainThread = false;
        public ZFTimerTask(long zfjniPointerToken, boolean timerActivateInMainThread) {
            _zfjniPointerToken = zfjniPointerToken;
            _timerActivateInMainThread = timerActivateInMainThread;
        }
        @Override
        public void run() {
            if(_running) {
                if(_firstTime) {
                    _firstTime = false;
                    ZFTimer.native_notifyTimerStart(_zfjniPointerToken);
                }
                if(_timerActivateInMainThread) {
                    _mainThreadCallback.sendMessage(Message.obtain(_mainThreadCallback, 0, this));
                }
                else {
                    ZFTimer.native_notifyTimerActivate(_zfjniPointerToken);
                }
            }
        }
        @Override
        public boolean cancel() {
            if(_running) {
                ZFTimer.native_notifyTimerStop(_zfjniPointerToken);
            }
            _running = false;
            _zfjniPointerToken = 0;
            return super.cancel();
        }
    }
    protected static void native_timerStart(Object nativeTimer, long zfjniPointerToken, long timerDelay, long timerInterval, boolean timerActivateInMainThread) {
        ZFTimer holder = (ZFTimer)nativeTimer;
        holder.timer = new Timer();
        holder.timerTask = new ZFTimerTask(zfjniPointerToken, timerActivateInMainThread);
        holder.timer.schedule(holder.timerTask, timerDelay, timerInterval);
    }
    protected static void native_timerStop(Object nativeTimer) {
        ZFTimer holder = (ZFTimer)nativeTimer;
        holder.timerTask.cancel();
        holder.timer.cancel();
        holder.timer.purge();
        holder.timer = null;
        holder.timerTask = null;
    }
    protected native static void native_notifyTimerStart(long zfjniPointerToken);
    protected native static void native_notifyTimerActivate(long zfjniPointerToken);
    protected native static void native_notifyTimerStop(long zfjniPointerToken);
}
