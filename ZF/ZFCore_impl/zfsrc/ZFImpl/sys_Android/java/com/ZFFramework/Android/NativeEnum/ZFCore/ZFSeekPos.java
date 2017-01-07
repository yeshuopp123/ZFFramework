/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFSeekPos {
    public static final int ZFSeekPosBegin = ZFSeekPos.native_ZFSeekPosBegin();
    private native static int native_ZFSeekPosBegin();
    public static final int ZFSeekPosCur = ZFSeekPos.native_ZFSeekPosCur();
    private native static int native_ZFSeekPosCur();
    public static final int ZFSeekPosCurReversely = ZFSeekPos.native_ZFSeekPosCurReversely();
    private native static int native_ZFSeekPosCurReversely();
    public static final int ZFSeekPosEnd = ZFSeekPos.native_ZFSeekPosEnd();
    private native static int native_ZFSeekPosEnd();
}

