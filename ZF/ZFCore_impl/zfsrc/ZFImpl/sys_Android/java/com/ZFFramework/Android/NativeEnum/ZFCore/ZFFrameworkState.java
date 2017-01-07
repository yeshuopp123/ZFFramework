/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFFrameworkState {
    public static final int ZFFrameworkStateNotAvailable = ZFFrameworkState.native_ZFFrameworkStateNotAvailable();
    private native static int native_ZFFrameworkStateNotAvailable();
    public static final int ZFFrameworkStateInitProcessing = ZFFrameworkState.native_ZFFrameworkStateInitProcessing();
    private native static int native_ZFFrameworkStateInitProcessing();
    public static final int ZFFrameworkStateAvailable = ZFFrameworkState.native_ZFFrameworkStateAvailable();
    private native static int native_ZFFrameworkStateAvailable();
    public static final int ZFFrameworkStateCleanupProcessing = ZFFrameworkState.native_ZFFrameworkStateCleanupProcessing();
    private native static int native_ZFFrameworkStateCleanupProcessing();
}

