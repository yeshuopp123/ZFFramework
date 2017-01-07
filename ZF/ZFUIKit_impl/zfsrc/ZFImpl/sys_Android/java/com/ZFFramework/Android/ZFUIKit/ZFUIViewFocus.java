/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import java.lang.ref.WeakReference;
import android.content.Context;
import android.view.View;
import android.view.View.OnFocusChangeListener;
import android.view.ViewParent;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;

public class ZFUIViewFocus {
    private static WeakReference<ZFUIView> _focusedView = null;
    public static ZFUIView focusedView() {
        return ((_focusedView != null) ? _focusedView.get() : null);
    }

    private static OnFocusChangeListener _onFocusChangeListener = new OnFocusChangeListener() {
        @Override
        public void onFocusChange(View v, boolean hasFocus) {
            if(hasFocus && (v instanceof EditText)) {
                InputMethodManager imm = (InputMethodManager)v.getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.showSoftInput(v, InputMethodManager.SHOW_IMPLICIT);
            }
            ZFUIView tmp = null;
            if(v instanceof ZFUIView) {
                tmp = (ZFUIView)v;
            }
            else {
                ViewParent parent = v.getParent();
                if(parent instanceof ZFUIView) {
                    tmp = (ZFUIView)parent;
                }
            }
            if(tmp != null && tmp.zfjniPointerOwnerZFUIView != 0) {
                _focusedView = new WeakReference<ZFUIView>(tmp);
                ZFUIViewFocus.native_notifyViewFocusChanged(tmp.zfjniPointerOwnerZFUIView);
            }
        }
    };
    public static void ZFUIViewImplChanged(ZFUIView view, View implViewOld, View implViewNew) {
        if(implViewOld != null) {
            implViewOld.setOnFocusChangeListener(null);
        }
        if(implViewNew != null) {
            implViewNew.setFocusable(view.isFocusable());
            implViewNew.setFocusableInTouchMode(view.isFocusableInTouchMode());
            if(view.isFocusable()) {
                implViewNew.setOnFocusChangeListener(_onFocusChangeListener);
            }
        }
    }
    public static void native_viewFocusableSet(Object nativeView, boolean viewFocusable) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View viewToSet = nativeViewTmp;
        if(nativeViewTmp.nativeImplView != null) {
            viewToSet = nativeViewTmp.nativeImplView;
        }

        nativeViewTmp.setFocusable(viewFocusable);
        nativeViewTmp.setFocusableInTouchMode(viewFocusable);
        viewToSet.setFocusable(viewFocusable);
        viewToSet.setFocusableInTouchMode(viewFocusable);
        if(viewFocusable) {
            nativeViewTmp.setOnFocusChangeListener(_onFocusChangeListener);
            viewToSet.setOnFocusChangeListener(_onFocusChangeListener);
        }
        else {
            nativeViewTmp.setOnFocusChangeListener(null);
            viewToSet.setOnFocusChangeListener(null);
        }
    }
    public static boolean native_viewFocused(Object nativeView) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        return (nativeViewTmp.isFocused()
            || (nativeViewTmp.nativeImplView != null && nativeViewTmp.nativeImplView.isFocused()));
    }
    public static void native_viewFocusRequest(Object nativeView, boolean viewFocus) {
        ZFUIView nativeViewTmp = (ZFUIView)nativeView;
        View viewToSet = nativeViewTmp;
        if(nativeViewTmp.nativeImplView != null && nativeViewTmp.nativeImplView.isFocusable()) {
            viewToSet = nativeViewTmp.nativeImplView;
        }

        if(viewFocus) {
            viewToSet.requestFocus();
        }
        else {
            viewToSet.clearFocus();
        }
    }

    public static native void native_notifyViewFocusChanged(long zfjniPointerOwnerZFUIView);
}
