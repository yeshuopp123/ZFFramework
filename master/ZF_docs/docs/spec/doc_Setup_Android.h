/**
 * @page DocTag_Setup_Android Android
 *
 * you must have these settings being prepared:
 * -  have <a href="http://developer.android.com/tools/sdk/ndk/">Android NDK</a> installed and in your system path
 * -  have <a href="http://ant.apache.org/">ant</a> installed and in your system path,
 *   and have "ANDROID_HOME" been set properly
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup "Quick setup" for quick setup your project files\n
 * \n
 * \n
 * if you really want to setup manually:
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own Android project
 * -# merge all files under _release folder (assets, libs) to your project's path
 * -# set up JNI env like other Android JNI project,
 *   especially to specify header include path of ZFFramework,
 *   which located in ~/_release\n
 *   for example:
 *   @code
 *     // in your Android.mk
 *     LOCAL_C_INCLUDES := path_to_ZFFramework_include
 *
 *     // link ZFFramework's lib's so files
 *     include $(CLEAR_VARS)
 *     LOCAL_MODULE := MyLibName
 *     LOCAL_SRC_FILES : path_to_ZFFramework/$(TARGET_ARCH_ABI)/libMyLibName.so
 *     include $(PREBUILT_SHARED_LIBRARY)
 *   @endcode
 * -# in your Android project's AndroidManifest.xml,
 *   add these activity declaration to your \<application\> node:
 *   @code
 *     <activity
 *         android:name="com.ZFFramework.Android.ZF.ZFMainEntry"
 *         android:label="ZFFramework"
 *         android:launchMode="singleTask" >
 *     </activity>
 *     <activity
 *         android:name="com.ZFFramework.Android.ZFUIKit.ZFUISysWindow"
 *         android:configChanges="orientation|keyboardHidden|screenSize"
 *         android:label="ZFFramework"
 *         android:windowSoftInputMode="adjustResize" >
 *     </activity>
 *   @endcode
 *   and these permission:
 *   @code
 *     <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
 *     <uses-permission android:name="android.permission.INTERNET" />
 *   @endcode
 * -# in your Android project,
 *   create a loader activity that,
 *   load necessary libraries,
 *   and start ZFMainEntry then finish itself:
 *   @code
 *     static {
 *         System.loadLibrary("stlport_shared");
 *         System.loadLibrary("ZFCore");
 *         // all other ZF libs here
 *         System.loadLibrary("your_app_lib_name");
 *     }
 *
 *     @Override
 *     protected void onCreate(Bundle savedInstanceState) {
 *         Intent intent = new Intent(Loader.this, ZFMainEntry.class);
 *         startActivity(intent);
 *         this.finish();
 *     }
 *   @endcode
 * -# ensure all of your resource files put in "assets/zfres" directory of your Android project,
 *   and ensured all of them have an extra ".mp2" as file name postfix if they are large than 1M bytes
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial "Tutorial" to code with ZFFramework, enjoy
 */
