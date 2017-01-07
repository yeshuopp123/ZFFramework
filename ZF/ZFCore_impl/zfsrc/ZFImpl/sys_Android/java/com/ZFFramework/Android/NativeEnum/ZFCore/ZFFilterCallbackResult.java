/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFFilterCallbackResult {
    public static final int ZFFilterCallbackResultNotSpecified = ZFFilterCallbackResult.native_ZFFilterCallbackResultNotSpecified();
    private native static int native_ZFFilterCallbackResultNotSpecified();
    public static final int ZFFilterCallbackResultActive = ZFFilterCallbackResult.native_ZFFilterCallbackResultActive();
    private native static int native_ZFFilterCallbackResultActive();
    public static final int ZFFilterCallbackResultNotActive = ZFFilterCallbackResult.native_ZFFilterCallbackResultNotActive();
    private native static int native_ZFFilterCallbackResultNotActive();
}

