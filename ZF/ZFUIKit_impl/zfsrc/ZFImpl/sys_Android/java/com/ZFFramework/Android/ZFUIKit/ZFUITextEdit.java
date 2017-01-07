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
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUIAlign;
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUITextAppearance;
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUITextEditKeyboardReturnType;
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUITextEditKeyboardType;
import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF.ZFMainEntry;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.Editable;
import android.text.TextWatcher;
import android.text.method.HideReturnsTransformationMethod;
import android.text.method.PasswordTransformationMethod;
import android.util.TypedValue;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.TextView;

@SuppressLint("RtlHardcoded")
public final class ZFUITextEdit extends EditText {
    public static Object native_nativeTextEditCreate(long zfjniPointerOwnerZFUITextEdit) {
        ZFUITextEdit ret = new ZFUITextEdit(ZFMainEntry.mainEntryActivity());
        ret.zfjniPointerOwnerZFUITextEdit = zfjniPointerOwnerZFUITextEdit;
        return ret;
    }
    public static void native_nativeTextEditDestroy(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.removeTextChangedListener(nativeTextEditTmp._textWatcher);
        nativeTextEditTmp._textWatcher = null;
        nativeTextEditTmp.setText(null);
        nativeTextEditTmp.setOnEditorActionListener(null);
        nativeTextEditTmp.zfjniPointerOwnerZFUITextEdit = 0;
    }

    // ============================================================
    public static void native_textEditEnableSet(Object nativeTextEdit,
                                                boolean textEditEnable) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setEnabled(textEditEnable);
    }
    public static void native_textEditSecuredSet(Object nativeTextEdit,
                                                 boolean textEditSecured) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditSecured) {
            nativeTextEditTmp.setTransformationMethod(PasswordTransformationMethod.getInstance());
        }
        else {
            nativeTextEditTmp.setTransformationMethod(HideReturnsTransformationMethod.getInstance());
        }
    }
    public static void native_textEditKeyboardTypeSet(Object nativeTextEdit,
                                                      int textEditKeyboardType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditKeyboardType == ZFUITextEditKeyboardType.e_Normal) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        }
        else if(textEditKeyboardType == ZFUITextEditKeyboardType.e_CharBased) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_TEXT);
        }
        else if(textEditKeyboardType == ZFUITextEditKeyboardType.e_PhonePad) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_PHONE);
        }
        else if(textEditKeyboardType == ZFUITextEditKeyboardType.e_NumberPad) {
            nativeTextEditTmp.setInputType(EditorInfo.TYPE_CLASS_NUMBER);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }
    public static void native_textEditKeyboardReturnTypeSet(Object nativeTextEdit,
                                                            int textEditKeyboardReturnType) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Normal) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NONE);
        }
        else if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Next) {
            nativeTextEditTmp.setImeActionLabel(null, 0);
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_NEXT);
        }
        else if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Search) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEARCH);
        }
        else if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Done) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_DONE);
        }
        else if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Go) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_GO);
        }
        else if(textEditKeyboardReturnType == ZFUITextEditKeyboardReturnType.e_Send) {
            nativeTextEditTmp.setImeOptions(EditorInfo.IME_ACTION_SEND);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }

    public static int native_textSelectRange_start(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        return nativeTextEditTmp.getSelectionStart();
    }
    public static int native_textSelectRange_count(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        return nativeTextEditTmp.getSelectionEnd() - nativeTextEditTmp.getSelectionStart();
    }

    public static void native_textSelectRangeSet(Object nativeTextEdit, int start, int count) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(count == 0) {
            nativeTextEditTmp.setSelection(start);
        }
        else {
            nativeTextEditTmp.setSelection(start, start + count);
        }
    }

    // ============================================================
    public static void native_textContentSet(Object nativeTextEdit,
                                             Object nativeText) {
        ((ZFUITextEdit)nativeTextEdit).setText((String)nativeText);
    }
    public static void native_textAppearanceSet(Object nativeTextEdit,
                                                int textAppearance) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if(textAppearance == ZFUITextAppearance.e_Normal) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
        else if(textAppearance == ZFUITextAppearance.e_Bold) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        }
        else if(textAppearance == ZFUITextAppearance.e_Italic) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.ITALIC));
        }
        else if(textAppearance == ZFUITextAppearance.e_BoldItalic) {
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD_ITALIC));
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
            nativeTextEditTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
    }
    public static void native_textAlignSet(Object nativeTextEdit,
                                           int textAlign) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        if((textAlign & ZFUIAlign.e_LeftInner) == ZFUIAlign.e_LeftInner) {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
        else if((textAlign & ZFUIAlign.e_RightInner) == ZFUIAlign.e_RightInner) {
            nativeTextEditTmp.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);
        }
        else if(textAlign == ZFUIAlign.e_Center) {
            nativeTextEditTmp.setGravity(Gravity.CENTER);
        }
        else {
            nativeTextEditTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }
    public static void native_textColorSet(Object nativeTextEdit,
                                           int textColor) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setTextColor(textColor);
    }
    public static void native_textShadowColorSet(Object nativeTextEdit,
                                                 int textShadowColor) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp._textShadowColor = textShadowColor;
        nativeTextEditTmp.setShadowLayer(
            1,
            nativeTextEditTmp._textShadowOffsetX,
            nativeTextEditTmp._textShadowOffsetY,
            nativeTextEditTmp._textShadowColor);
    }
    public static void native_textShadowOffsetSet(Object nativeTextEdit,
                                                  int textShadowOffsetX,
                                                  int textShadowOffsetY) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp._textShadowOffsetX = textShadowOffsetX;
        nativeTextEditTmp._textShadowOffsetY = textShadowOffsetY;
        nativeTextEditTmp.setShadowLayer(
            1,
            nativeTextEditTmp._textShadowOffsetX,
            nativeTextEditTmp._textShadowOffsetY,
            nativeTextEditTmp._textShadowColor);
    }
    public static void native_textSizeSet(Object nativeTextEdit,
                                          int textSize) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);
    }
    private static ZFAndroidSize _native_measureNativeTextEdit_sizeCache = new ZFAndroidSize();
    public static Object native_measureNativeTextEdit(Object nativeTextEdit,
                                                      int maxWidthOrNegative,
                                                      int maxHeightOrNegative,
                                                      int textSize) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        float savedTextSize = nativeTextEditTmp.getTextSize();
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);

        int widthOld = nativeTextEditTmp.getMeasuredWidth();
        int heightOld = nativeTextEditTmp.getMeasuredHeight();

        nativeTextEditTmp.measure(
            (maxWidthOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
            (maxHeightOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));

        int padding = (int)(2 * ZFAndroidUI.screenDensity(nativeTextEditTmp.getContext()));
        _native_measureNativeTextEdit_sizeCache.set(
            nativeTextEditTmp.getMeasuredWidth() + padding,
            nativeTextEditTmp.getMeasuredHeight() + padding);
        nativeTextEditTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, savedTextSize);

        // measured size must be restored, otherwise text's draw step would cause strange error
        nativeTextEditTmp.setMeasuredDimension(widthOld, heightOld);

        return _native_measureNativeTextEdit_sizeCache;
    }

    // ============================================================
    public static void native_textEditBegin(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.requestFocus();
    }
    public static void native_textEditEnd(Object nativeTextEdit) {
        ZFUITextEdit nativeTextEditTmp = (ZFUITextEdit)nativeTextEdit;
        nativeTextEditTmp.clearFocus();
    }

    // ============================================================
    private static native boolean native_notifyCheckTextShouldChange(long zfjniPointerOwnerZFUITextEdit,
                                                                     Object newString);
    private static native void native_notifyTextChange(long zfjniPointerOwnerZFUITextEdit,
                                                       Object newString);
    private static native void native_notifyTextReturnClicked(long zfjniPointerOwnerZFUITextEdit);
    private static native void native_notifyTextEditBegin(long zfjniPointerOwnerZFUITextEdit);
    private static native void native_notifyTextEditEnd(long zfjniPointerOwnerZFUITextEdit);

    // ============================================================
    @Override
    protected void onFocusChanged(boolean gainFocus, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(gainFocus, direction, previouslyFocusedRect);
        if(gainFocus) {
            ZFUITextEdit.native_notifyTextEditBegin(this.zfjniPointerOwnerZFUITextEdit);
        }
        else {
            ZFUITextEdit.native_notifyTextEditEnd(this.zfjniPointerOwnerZFUITextEdit);
        }
    }
    private static class _TextWatcher implements TextWatcher {
        private WeakReference<ZFUITextEdit> owner = null;
        public _TextWatcher(ZFUITextEdit owner) {
            this.owner = new WeakReference<ZFUITextEdit>(owner);
        }

        private String savedString = null;
        private boolean overrided = false;
        @Override
        public void beforeTextChanged(CharSequence s,
                                      int start,
                                      int count,
                                      int after) {
            this.savedString = s.toString();
        }
        @Override
        public void onTextChanged(CharSequence s,
                                  int start,
                                  int before,
                                  int count) {
        }
        @Override
        public void afterTextChanged(Editable s) {
            if(this.owner.get() == null || this.overrided) {
                return ;
            }

            String newString = s.toString();
            if(!ZFUITextEdit.native_notifyCheckTextShouldChange(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString)) {
                this.overrided = true;
                this.owner.get().setText(this.savedString);
                this.overrided = false;
            }
            else {
                ZFUITextEdit.native_notifyTextChange(this.owner.get().zfjniPointerOwnerZFUITextEdit, newString);
            }
        }
    }
    private long zfjniPointerOwnerZFUITextEdit = 0;
    private int _textShadowColor = Color.TRANSPARENT;
    private int _textShadowOffsetX = 1;
    private int _textShadowOffsetY = 1;
    private TextWatcher _textWatcher = null;

    // ============================================================
    @SuppressWarnings("deprecation")
    public ZFUITextEdit(Context context) {
        super(context);

        this.setTextColor(Color.BLACK);
        this.setBackgroundDrawable(null);
        this.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        this.setPadding(0, 0, 0, 0);
        this.setSingleLine();
        this.setImeOptions(EditorInfo.IME_ACTION_NONE);

        _textWatcher = new _TextWatcher(this);
        this.addTextChangedListener(_textWatcher);

        this.setOnEditorActionListener(new OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView v, int actionId, KeyEvent event) {
                switch(actionId) {
                    case EditorInfo.IME_ACTION_NONE:
                    case EditorInfo.IME_ACTION_NEXT:
                    case EditorInfo.IME_ACTION_SEARCH:
                    case EditorInfo.IME_ACTION_DONE:
                    case EditorInfo.IME_ACTION_GO:
                    case EditorInfo.IME_ACTION_SEND:
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                        return true;
                    default:
                        break;
                }
                if(event != null && event.getKeyCode() == KeyEvent.KEYCODE_ENTER) {
                    if(event.getAction() == KeyEvent.ACTION_UP) {
                        ZFUITextEdit.native_notifyTextReturnClicked(ZFUITextEdit.this.zfjniPointerOwnerZFUITextEdit);
                    }
                    return true;
                }
                return false;
            }
        });
    }
    @Override
    public boolean dispatchKeyEvent(KeyEvent event) {
        if(event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEvent(event);
        }
        if(event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }
    @Override
    public boolean dispatchKeyEventPreIme(KeyEvent event) {
        if(event.getKeyCode() != KeyEvent.KEYCODE_ENTER) {
            return super.dispatchKeyEventPreIme(event);
        }
        if(event.getAction() == KeyEvent.ACTION_UP) {
            ZFUITextEdit.native_notifyTextReturnClicked(this.zfjniPointerOwnerZFUITextEdit);
        }
        return true;
    }
}
