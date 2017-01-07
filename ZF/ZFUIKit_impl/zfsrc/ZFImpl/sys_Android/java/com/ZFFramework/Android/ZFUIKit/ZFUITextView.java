/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZFUIKit;

import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUIAlign;
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUITextAppearance;
import com.ZFFramework.Android.NativeEnum.ZFUIKit.ZFUITextTruncateMode;
import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import com.ZFFramework.Android.NativeUtil.ZFAndroidSize;
import com.ZFFramework.Android.NativeUtil.ZFAndroidUI;
import com.ZFFramework.Android.ZF.ZFMainEntry;
import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.text.TextUtils.TruncateAt;
import android.util.TypedValue;
import android.view.Gravity;
import android.widget.TextView;

@SuppressLint("RtlHardcoded")
public final class ZFUITextView extends TextView {
    public static Object native_nativeTextViewCreate() {
        return new ZFUITextView(ZFMainEntry.mainEntryActivity());
    }
    public static void native_nativeTextViewDestroy(Object nativeTextView) {
        ((ZFUITextView)nativeTextView).setText(null);
    }

    public static void native_textContentSet(Object nativeTextView,
                                             Object nativeText) {
        ((ZFUITextView)nativeTextView).setText((String)nativeText);
    }
    public static void native_textAppearanceSet(Object nativeTextView,
                                                int textAppearance) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        if(textAppearance == ZFUITextAppearance.e_Normal) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
        else if(textAppearance == ZFUITextAppearance.e_Bold) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
        }
        else if(textAppearance == ZFUITextAppearance.e_Italic) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.ITALIC));
        }
        else if(textAppearance == ZFUITextAppearance.e_BoldItalic) {
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD_ITALIC));
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
            nativeTextViewTmp.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
        }
    }
    public static void native_textAlignSet(Object nativeTextView,
                                           int textAlign) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        if((textAlign & ZFUIAlign.e_LeftInner) == ZFUIAlign.e_LeftInner) {
            nativeTextViewTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
        else if((textAlign & ZFUIAlign.e_RightInner) == ZFUIAlign.e_RightInner) {
            nativeTextViewTmp.setGravity(Gravity.RIGHT | Gravity.CENTER_VERTICAL);
        }
        else if(textAlign == ZFUIAlign.e_Center) {
            nativeTextViewTmp.setGravity(Gravity.CENTER);
        }
        else {
            nativeTextViewTmp.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        }
    }
    public static void native_textColorSet(Object nativeTextView,
                                           int textColor) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        nativeTextViewTmp.setTextColor(textColor);
    }
    public static void native_textShadowColorSet(Object nativeTextView,
                                                 int textShadowColor) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        nativeTextViewTmp._textShadowColor = textShadowColor;
        nativeTextViewTmp.setShadowLayer(
            1,
            nativeTextViewTmp._textShadowOffsetX,
            nativeTextViewTmp._textShadowOffsetY,
            nativeTextViewTmp._textShadowColor);
    }
    public static void native_textShadowOffsetSet(Object nativeTextView,
                                                  int textShadowOffsetX,
                                                  int textShadowOffsetY) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        nativeTextViewTmp._textShadowOffsetX = textShadowOffsetX;
        nativeTextViewTmp._textShadowOffsetY = textShadowOffsetY;
        nativeTextViewTmp.setShadowLayer(
            1,
            nativeTextViewTmp._textShadowOffsetX,
            nativeTextViewTmp._textShadowOffsetY,
            nativeTextViewTmp._textShadowColor);
    }
    public static void native_textSingleLineSet(Object nativeTextView,
                                                boolean textSingleLine) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        nativeTextViewTmp.setSingleLine(textSingleLine);
    }
    public static void native_textTruncateModeSet(Object nativeTextView,
                                                  int textTruncateMode) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        if(textTruncateMode == ZFUITextTruncateMode.e_Disable) {
            nativeTextViewTmp.setEllipsize(TruncateAt.END);
        }
        else if(textTruncateMode == ZFUITextTruncateMode.e_Tail) {
            nativeTextViewTmp.setEllipsize(TruncateAt.END);
        }
        else if(textTruncateMode == ZFUITextTruncateMode.e_Middle) {
            nativeTextViewTmp.setEllipsize(TruncateAt.MIDDLE);
        }
        else if(textTruncateMode == ZFUITextTruncateMode.e_Head) {
            nativeTextViewTmp.setEllipsize(TruncateAt.START);
        }
        else {
            ZFAndroidLog.shouldNotGoHere();
        }
    }
    private static ZFAndroidSize _native_measureNativeTextView_sizeCache = new ZFAndroidSize();
    public static Object native_measureNativeTextView(Object nativeTextView,
                                                      int maxWidthOrNegative,
                                                      int maxHeightOrNegative,
                                                      int textSize) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        float savedTextSize = nativeTextViewTmp.getTextSize();
        nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSize);

        int widthOld = nativeTextViewTmp.getMeasuredWidth();
        int heightOld = nativeTextViewTmp.getMeasuredHeight();

        nativeTextViewTmp.measure(
            (maxWidthOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxWidthOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED),
            (maxHeightOrNegative >= 0)
                ? MeasureSpec.makeMeasureSpec(maxHeightOrNegative, MeasureSpec.AT_MOST)
                : MeasureSpec.makeMeasureSpec(0, MeasureSpec.UNSPECIFIED));

        int padding = (int)(2 * ZFAndroidUI.screenDensity(nativeTextViewTmp.getContext()));
        _native_measureNativeTextView_sizeCache.set(
            nativeTextViewTmp.getMeasuredWidth() + padding,
            nativeTextViewTmp.getMeasuredHeight() + padding);
        nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, savedTextSize);

        // measured size must be restored, otherwise text's draw step would cause strange error
        nativeTextViewTmp.setMeasuredDimension(widthOld, heightOld);

        return _native_measureNativeTextView_sizeCache;
    }
    public static int native_textSizeCurrent(Object nativeTextView) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        return (int)(nativeTextViewTmp.getTextSize());
    }
    public static void native_textSizeAutoChangeCurrentValueSet(Object nativeTextView,
                                                                int textSizeCurrent) {
        ZFUITextView nativeTextViewTmp = (ZFUITextView)nativeTextView;
        nativeTextViewTmp.setTextSize(TypedValue.COMPLEX_UNIT_PX, textSizeCurrent);
    }

    // ============================================================
    private int _textShadowColor = Color.TRANSPARENT;
    private int _textShadowOffsetX = 1;
    private int _textShadowOffsetY = 1;

    // ============================================================
    @SuppressWarnings("deprecation")
    public ZFUITextView(Context context) {
        super(context);

        this.setTextColor(Color.BLACK);
        this.setBackgroundDrawable(null);
        this.setGravity(Gravity.LEFT | Gravity.CENTER_VERTICAL);
        this.setSingleLine(true);
        this.setPadding(0, 0, 0, 0);
    }
}
