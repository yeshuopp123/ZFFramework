/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFAlgorithm;

public class ZFXmlVisitType {
    public static final int e_Enter = ZFXmlVisitType.native_e_Enter();
    private native static int native_e_Enter();
    public static final int e_Exit = ZFXmlVisitType.native_e_Exit();
    private native static int native_e_Exit();
    public static final int e_ExitChildren = ZFXmlVisitType.native_e_ExitChildren();
    private native static int native_e_ExitChildren();

    public static final int EnumDefault = ZFXmlVisitType.native_EnumDefault();
    private native static int native_EnumDefault();
}

