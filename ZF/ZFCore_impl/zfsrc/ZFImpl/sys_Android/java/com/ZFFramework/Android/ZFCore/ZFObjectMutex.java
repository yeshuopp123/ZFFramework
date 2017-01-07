/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFCore;

import java.util.concurrent.locks.ReentrantLock;

public final class ZFObjectMutex {
    public static Object native_nativeMutexCreate() {
        return new ReentrantLock();
    }
    public static void native_mutexLock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).lock();
    }
    public static boolean native_mutexTryLock(Object nativeMutex) {
        return ((ReentrantLock)nativeMutex).tryLock();
    }
    public static void native_mutexUnlock(Object nativeMutex) {
        ((ReentrantLock)nativeMutex).unlock();
    }
}
