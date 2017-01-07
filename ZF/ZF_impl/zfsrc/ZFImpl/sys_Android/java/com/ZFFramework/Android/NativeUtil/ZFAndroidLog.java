/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.NativeUtil;

import java.lang.reflect.Field;
import java.lang.reflect.Modifier;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import android.annotation.SuppressLint;
import android.graphics.Rect;
import android.graphics.RectF;
import android.os.Handler;
import android.os.Message;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.MeasureSpec;
import android.view.ViewGroup;
import android.widget.TextView;

@SuppressLint("DefaultLocale")
public class ZFAndroidLog {
    public static void p(String msg) {
        android.util.Log.e(ZFAndroidLog.defaultLogTag, msg);
    }
    public static void p(String tag, String msg) {
        android.util.Log.e(tag, msg);
    }
    public static void p(Class<?> cls, String msg) {
        android.util.Log.e(cls.getSimpleName(), msg);
    }

    public static String logTagForClass(Class<?> cls) {
        return cls.getSimpleName();
    }
    public static String defaultLogTag = "";

    public static void assertMsg(String sMsg) {
        ZFAndroidLog._printAssertMsg(ZFAndroidLog.defaultLogTag, sMsg);
    }
    public static void assertMsg(String sTag, String sMsg) {
        ZFAndroidLog._printAssertMsg(sTag, sMsg);
    }

    public static void shouldNotGoHere() {
        ZFAndroidLog._printAssertMsg(ZFAndroidLog.defaultLogTag, "should not go here");
    }
    public static void shouldNotGoHere(String sTag) {
        ZFAndroidLog._printAssertMsg(sTag, "should not go here");
    }
    private static void _printAssertMsg(String sTag, String sMsg) {
        ZFAndroidLog.p(sTag, "====================  assert message   ====================");
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        ZFAndroidLog.p(sTag, String.format("| called by %s [%s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber()));
        ZFAndroidLog.p(sTag, "| " + sMsg);
        ZFAndroidLog.p(sTag, "====================  assert message   ====================");
    }

    // caller info
    public static String callerInfo() {
        return ZFAndroidLog._callerInfo(1);
    }
    public static String callerInfo(int ignoreLevel) {
        return ZFAndroidLog._callerInfo(ignoreLevel);
    }
    @SuppressLint("DefaultLocale")
    private static String _callerInfo(int ignoreLevel) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4+ignoreLevel];
        return String.format("[%s %s (%d)]", ste.getMethodName(), ste.getFileName(), ste.getLineNumber());
    }

    // log called function
    public static void logFunc() {
        ZFAndroidLog._logFunc(ZFAndroidLog.defaultLogTag, "");
    }
    public static void logFunc(String msg) {
        ZFAndroidLog._logFunc(ZFAndroidLog.defaultLogTag, msg);
    }
    public static void logFunc(String tag, String msg) {
        ZFAndroidLog._logFunc(tag, msg);
    }
    private static void _logFunc(String tag, String msg) {
        StackTraceElement ste = Thread.currentThread().getStackTrace()[4];
        ZFAndroidLog.p(tag, String.format("%s %s", ste.getMethodName(), msg));
    }

    // stack trace
    public static String stackTrace() {
        return ZFAndroidLog._stackTrace(0, Integer.MAX_VALUE);
    }
    public static String stackTrace(int levelIgnore) {
        return ZFAndroidLog._stackTrace(levelIgnore, Integer.MAX_VALUE);
    }
    public static String stackTrace(int levelIgnore, int maxNum) {
        return ZFAndroidLog._stackTrace(levelIgnore, maxNum);
    }
    private static String _stackTrace(int levelIgnore, int maxNum) {
        StringBuilder sb = new StringBuilder();
        StackTraceElement ste[] = Thread.currentThread().getStackTrace();
        sb.append("==================== stack trace begin ====================");
        sb.append('\n');
        for(int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            sb.append(String.format("|  %s [%s (%d)]",
                ste[i].getMethodName(),
                ste[i].getFileName(),
                ste[i].getLineNumber()));
            sb.append('\n');
        }
        sb.append("====================  stack trace end  ====================");
        sb.append('\n');
        return sb.toString();
    }

    public static void stackTracePrint() {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, 0, Integer.MAX_VALUE);
    }
    public static void stackTracePrint(int levelIgnore) {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, levelIgnore, Integer.MAX_VALUE);
    }
    public static void stackTracePrint(int levelIgnore, int maxNum) {
        ZFAndroidLog._stackTracePrint(ZFAndroidLog.defaultLogTag, levelIgnore, maxNum);
    }
    public static void stackTracePrint(String sTag, int levelIgnore, int maxNum) {
        ZFAndroidLog._stackTracePrint(sTag, levelIgnore, maxNum);
    }
    private static void _stackTracePrint(String sTag, int levelIgnore, int maxNum) {
        StackTraceElement ste[] = Thread.currentThread().getStackTrace();
        ZFAndroidLog.p(sTag, "==================== stack trace begin ====================");
        for(int i = 4 + levelIgnore, num = 0; i < ste.length && num < maxNum; ++i, ++num) {
            ZFAndroidLog.p(sTag, String.format("|  %s [%s (%d)]",
                ste[i].getMethodName(),
                ste[i].getFileName(),
                ste[i].getLineNumber()));
        }
        ZFAndroidLog.p(sTag, "====================  stack trace end  ====================");
    }

    // view tree
    public static void viewTreePrint(long miliSeconds, View view) {
        viewTreePrintDelayed(2000, "", view);
    }
    private static class _ViewTreePrintData {
        public String tag = null;
        public View view = null;
    }
    private static Handler _viewTreePrintDelayedHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            super.handleMessage(msg);
            _ViewTreePrintData data = (_ViewTreePrintData)msg.obj;
            viewTreePrint(data.tag, data.view);
        }
    };
    public static void viewTreePrintDelayed(long miliSeconds, String sTag, View view) {
        _ViewTreePrintData data = new _ViewTreePrintData();
        data.tag = sTag;
        data.view = view;
        _viewTreePrintDelayedHandler.sendMessageDelayed(Message.obtain(_viewTreePrintDelayedHandler, 0, data), miliSeconds);
    }
    public static void viewTreePrint(View view) {
        viewTreePrint("", view);
    }
    public static void viewTreePrint(String sTag, View view) {
        ZFAndroidLog.p(sTag, "====================  view tree begin  ====================");
        _viewTreePrintRecursion(sTag, view, 0, 0);
        ZFAndroidLog.p(sTag, "====================   view tree end   ====================");
    }
    private static void _viewTreePrintRecursion(String sTag, View view, int depth, int index) {
        if(view == null) {
            ZFAndroidLog.p(sTag, "| null");
            return ;
        }
        String sIndent = "";
        for(int i = 0; i < depth; ++i) {
            sIndent += "| ";
        }

        String sDetail = "";
        if(view.getTag() != null && (view.getTag() instanceof String)) {
            sDetail += " (" + ((String)view.getTag()) + ")";
        }
        if(view instanceof TextView) {
            sDetail += " \"" + ((TextView)view).getText().toString() + "\"";
        }
        if(view.isFocusable()) {
            sDetail += " focusable";
        }

        ZFAndroidLog.p(sTag, String.format("|%2d %s[%s %08X (%d, %d, %d, %d)%s]%s%s",
            index,
            sIndent,
            view.getClass().getSimpleName(),
            view.hashCode(),
            view.getLeft(), view.getTop(), view.getWidth(), view.getHeight(),
            sDetail,
            (view.getVisibility() == View.VISIBLE) ? "" : (" " + ZFAndroidLog.visibilityToString(view.getVisibility())),
            view.isEnabled() ? "" : " Disabled"
            ));

        if(view instanceof ViewGroup) {
            ViewGroup viewGroup = (ViewGroup)view;
            int n = viewGroup.getChildCount();
            for(int i = 0; i < n; ++i) {
                _viewTreePrintRecursion(sTag, viewGroup.getChildAt(i), depth + 1, i);
            }
        }
    }

    /**
     * @brief get info of native ZFObject, param must a (ZFObject *) pointer
     *   converted by #JNIConvertZFObjectFromJNIType
     */
    public native static String native_objectInfo(long zfjniPointerZFObject);

    private static boolean _isSysClass(Class<?> cls) {
        return (false
            || cls == Object.class
            || cls == String.class
            || cls == View.class
            );
    }
    public static String objectInfo(Object obj) {
        return ZFAndroidLog.objectInfo(obj, null);
    }
    public static String objectInfo(Object obj, List<String> ignorePropertyNames) {
        if(obj == null) {
            return "<null>";
        }

        StringBuilder sb = new StringBuilder();
        sb.append(String.format("<%s %08X", obj.getClass().getSimpleName(), obj.hashCode()));

        for(Class<?> cls = obj.getClass();
            cls != null && !ZFAndroidLog._isSysClass(cls);
            cls = cls.getSuperclass()) {
            Field[] allProperty = cls.getDeclaredFields();
            for(int i = 0; i < allProperty.length; ++i) {
                allProperty[i].setAccessible(true);
                if(Modifier.isStatic(allProperty[i].getModifiers())) {
                    continue;
                }

                if(ignorePropertyNames != null) {
                    boolean ignore = false;
                    for(int iIgnore = 0; iIgnore < ignorePropertyNames.size(); ++iIgnore) {
                        if(ignorePropertyNames.get(iIgnore).compareTo(allProperty[i].getName()) == 0) {
                            ignore = true;
                            break;
                        }
                    }
                    if(ignore) {
                        continue;
                    }
                }

                Object value = null;
                try {
                    value = allProperty[i].get(obj);
                }
                catch(Exception e) {
                }
                sb.append(String.format(" \"%s\"=\"%s\"",
                    allProperty[i].getName(),
                    value == null ? "<null>" : value.toString()));
            }
        }

        sb.append(">");
        return sb.toString();

    }

    // log utils
    public static String rectToString(Rect rect) {
        return String.format("(%d, %d, %d, %d)", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }
    public static String rectFToString(RectF rect) {
        return String.format("(%f, %f, %f, %f)", rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    }

    @SuppressLint("SimpleDateFormat")
    private static SimpleDateFormat _curTimeToStringFmtCache = new SimpleDateFormat("HH:mm:ss.SSS");
    public static String curTimeToString() {
        return _curTimeToStringFmtCache.format(new Date());
    }

    public static String visibilityToString(int visibility) {
        if(visibility == View.VISIBLE) {
            return "Visible";
        }
        else if(visibility == View.INVISIBLE) {
            return "Invisible";
        }
        else if(visibility == View.GONE) {
            return "Gone";
        }
        else {
            return String.format("<unknown visibility %d>", visibility);
        }
    }

    public static String measureSpecToString(int spec) {
        int mode = MeasureSpec.getMode(spec);
        int size = MeasureSpec.getSize(spec);
        if(mode == MeasureSpec.AT_MOST) {
            return String.format("(at most %d)", size);
        }
        else if(mode == MeasureSpec.EXACTLY) {
            return String.format("(exactly %d)", size);
        }
        else if(mode == MeasureSpec.UNSPECIFIED) {
            return String.format("(unspecified %d)", size);
        }
        else {
            return String.format("<unknown spec: %d %d(0x%08X)>", mode, size, spec);
        }
    }

    public static String mouseActionToString(int action) {
        switch(action) {
            case MotionEvent.ACTION_DOWN:
                return "MouseDown";
            case MotionEvent.ACTION_MOVE:
                return "MouseMove";
            case MotionEvent.ACTION_UP:
                return "MouseUp";
            case MotionEvent.ACTION_CANCEL:
                return "MouseCancel";
            case MotionEvent.ACTION_OUTSIDE:
                return "MouseOutside";
            default:
                return String.format("<MouseUnknown %d>", action);
        }
    }
    public static String mouseEventToString(MotionEvent event) {
        return String.format("%s (%d, %d)", ZFAndroidLog.mouseActionToString(event.getAction()), event.getX(), event.getY());
    }
    public static String keyActionToString(int action) {
        switch(action) {
            case KeyEvent.ACTION_DOWN:
                return "KeyDown";
            case KeyEvent.ACTION_UP:
                return "KeyUp";
            case KeyEvent.ACTION_MULTIPLE:
                return "KeyMultiple";
            default:
                return String.format("<KeyUnknown %d>", action);
        }
    }
    public static String keyCodeToString(int keyCode) {
        return KeyEvent.keyCodeToString(keyCode);
    }
    public static String keyEventToString(KeyEvent event) {
        return String.format("%s %s", ZFAndroidLog.keyActionToString(event.getAction()), ZFAndroidLog.keyCodeToString(event.getKeyCode()));
    }
}
