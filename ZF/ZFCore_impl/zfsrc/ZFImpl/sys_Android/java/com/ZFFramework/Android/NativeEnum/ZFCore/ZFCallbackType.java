/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFCallbackType {
    public static final int ZFCallbackTypeDummy = ZFCallbackType.native_ZFCallbackTypeDummy();
    private native static int native_ZFCallbackTypeDummy();
    public static final int ZFCallbackTypeMethod = ZFCallbackType.native_ZFCallbackTypeMethod();
    private native static int native_ZFCallbackTypeMethod();
    public static final int ZFCallbackTypeMemberMethod = ZFCallbackType.native_ZFCallbackTypeMemberMethod();
    private native static int native_ZFCallbackTypeMemberMethod();
    public static final int ZFCallbackTypeRawFunction = ZFCallbackType.native_ZFCallbackTypeRawFunction();
    private native static int native_ZFCallbackTypeRawFunction();
}

