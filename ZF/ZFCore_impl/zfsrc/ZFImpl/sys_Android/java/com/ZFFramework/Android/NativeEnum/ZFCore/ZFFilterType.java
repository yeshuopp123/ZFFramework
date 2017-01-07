/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFFilterType {
    public static final int ZFFilterTypeInclude = ZFFilterType.native_ZFFilterTypeInclude();
    private native static int native_ZFFilterTypeInclude();
    public static final int ZFFilterTypeExclude = ZFFilterType.native_ZFFilterTypeExclude();
    private native static int native_ZFFilterTypeExclude();
}

