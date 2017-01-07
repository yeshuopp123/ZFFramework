/* ====================================================================== *
 * Copyright (c) 2010-2016 ZFFramework
 * home page: http://ZFFramework.com
 * blog: http://zsaber.com
 * contact: master@zsaber.com (Chinese and English only)
 * Distributed under MIT license:
 *   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
 * ====================================================================== */
package com.ZFFramework.Android.ZF;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.List;
import com.ZFFramework.Android.NativeUtil.ZFAndroidLog;
import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

/**
 * @brief main entry as a Activity in Android
 *
 * under Android, start ZFMainEntry as a Activity
 * to startup main entry of ZFFramework,
 * ZFMainEntry must be single task and only one instance is allowed\n
 * before use, you must declare in your AndroidManifest.xml like this:
 * @code
 *   <activity
 *       android:name="com.ZFFramework.Android.ZFCore.ZFMainEntry"
 *       android:label="ZFFramework"
 *       android:launchMode="singleTask" >
 *   </activity>
 * @endcode
 * \n
 * typical start steps:
 * -  start your launcher activity
 * -  load any so files of ZFFramework and your app's cpp code
 * -  start ZFMainEntry as single task activity
 * -  finish your launcher activity
 */
public final class ZFMainEntry extends Activity {
    // ============================================================
    // global Activity event
    public static interface ActivityEventListener {
        public void action(Activity activity, View activityContentView);
    }
    public static final List<ActivityEventListener> activityAfterCreateListeners = new ArrayList<ActivityEventListener>();
    public static void activityAfterCreate(Activity activity, View activityContentView) {
        for(int i = 0; i < ZFMainEntry.activityAfterCreateListeners.size(); ++i) {
            ZFMainEntry.activityAfterCreateListeners.get(i).action(activity, activityContentView);
        }
    }
    public static final List<ActivityEventListener> activityBeforeDestroyListeners = new ArrayList<ActivityEventListener>();
    public static void activityBeforeDestroy(Activity activity, View activityContentView) {
        for(int i = 0; i < ZFMainEntry.activityAfterCreateListeners.size(); ++i) {
            ZFMainEntry.activityBeforeDestroyListeners.get(i).action(activity, activityContentView);
        }
    }

    // ============================================================
    // global static methods
    private static Context _appContext = null;
    public static Context appContext() {
        return _appContext;
    }

    public static AssetManager assetManager() {
        return ZFMainEntry.appContext().getAssets();
    }

    private static WeakReference<ZFMainEntry> _mainEntryActivity = null;
    public static ZFMainEntry mainEntryActivity() {
        return _mainEntryActivity.get();
    }

    // ============================================================
    // ZFMainEntry instance method
    public static class MainLayout extends FrameLayout {
        public MainLayout(Context context) {
            super(context);
        }
        public MainLayout(Context context, AttributeSet attrs) {
            super(context, attrs);
        }
        public MainLayout(Context context, AttributeSet attrs, int defStyle) {
            super(context, attrs, defStyle);
        }
    }
    private MainLayout _mainLayout = null;
    public MainLayout mainLayout() {
        return _mainLayout;
    }
    private boolean _resumed = false;
    public boolean resumed() {
        return _resumed;
    }

    // ============================================================
    // activity entry
    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(_appContext == null) {
            _appContext = this.getApplicationContext();
        }
        if(_mainEntryActivity == null) {
            _mainEntryActivity = new WeakReference<ZFMainEntry>(this);
        }

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        _mainLayout = new MainLayout(this);
        super.setContentView(_mainLayout);

        ZFFrameworkInit();
        ZFMainExecute(null);

        ZFMainEntry.activityAfterCreate(this, _mainLayout);
    }
    @Override
    protected void onDestroy() {
        ZFMainEntry.activityBeforeDestroy(this, _mainLayout);
        super.onDestroy();
        ZFFrameworkCleanup();
        _mainEntryActivity = null;
        _mainLayout = null;
    }
    @Override
    protected void onResume() {
        super.onResume();
        _resumed = true;
    }
    @Override
    protected void onPause() {
        super.onPause();
        _resumed = false;
    }

    @Override
    public void setContentView(int layoutResID) {
        ZFAndroidLog.assertMsg(ZFAndroidLog.logTagForClass(ZFMainEntry.class),
            "you should use mainLayout to add view");
    }
    @Override
    public void setContentView(View view, LayoutParams params) {
        ZFAndroidLog.assertMsg(ZFAndroidLog.logTagForClass(ZFMainEntry.class),
            "you should use mainLayout to add view");
    }
    @Override
    public void setContentView(View view) {
        ZFAndroidLog.assertMsg(ZFAndroidLog.logTagForClass(ZFMainEntry.class),
            "you should use mainLayout to add view");
    }

    // ============================================================
    // native
    private static boolean _mainEntryIsStarted = false;
    private static void ZFFrameworkInit() {
        if(!_mainEntryIsStarted) {
            _mainEntryIsStarted = true;
            native_ZFFrameworkInit();
        }
    }
    private static void ZFFrameworkCleanup() {
        if(_mainEntryIsStarted) {
            _mainEntryIsStarted = false;
            native_ZFFrameworkCleanup();
        }
    }
    private static int ZFMainExecute(Object param) {
        return native_ZFMainExecute(param);
    }

    // ============================================================
    // native communication
    protected static Object native_appContext() {
        return ZFMainEntry.appContext();
    }
    protected static Object native_assetManager() {
        return ZFMainEntry.assetManager();
    }
    protected static Object native_mainEntryActivity() {
        return ZFMainEntry.mainEntryActivity();
    }
    private native static void native_ZFFrameworkInit();
    private native static void native_ZFFrameworkCleanup();
    private native static int native_ZFMainExecute(Object param);
}
