/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeEnum.ZFCore;

public class ZFMethodPrivilegeType {
    public static final int ZFMethodPrivilegeTypePublic = ZFMethodPrivilegeType.native_ZFMethodPrivilegeTypePublic();
    private native static int native_ZFMethodPrivilegeTypePublic();
    public static final int ZFMethodPrivilegeTypeProtected = ZFMethodPrivilegeType.native_ZFMethodPrivilegeTypeProtected();
    private native static int native_ZFMethodPrivilegeTypeProtected();
    public static final int ZFMethodPrivilegeTypePrivate = ZFMethodPrivilegeType.native_ZFMethodPrivilegeTypePrivate();
    private native static int native_ZFMethodPrivilegeTypePrivate();
}

