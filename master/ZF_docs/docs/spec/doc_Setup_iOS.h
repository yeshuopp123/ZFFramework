/**
 * @page DocTag_Setup_iOS iOS
 *
 * you must have these settings being prepared:
 * -  have XCode installed
 *
 * once prepared, it's recommended to follow @ref DocTag_Setup_QuickSetup "Quick setup" for quick setup your project files\n
 * \n
 * \n
 * if you really want to setup manually:
 * -# build necessary libraries of ZFFramework, by one of these methods:
 *   -  tools from ~/tools/release
 *   -  manually build projects under ~/ZF
 * -# create your own iOS project
 * -# drag and drop ZFFramework's libraries (*.a files) to your project
 * -# modify your project's target settings:
 *   -  Header Search Paths
 *   -  Library Search Paths
 *   -  Other Linker Flags:\n
 *     "-Objc -all_load"
 * -# ensure these libraries seleted in "Link Binary With Libraries" of XCode's target settings:
 *   -  Foundation.framework
 *   -  UIKit.framework
 *   -  QuartzCore.framework
 *   -  CoreGraphics.framework
 *   -  all ZFFramework's *.a files
 * -# add "New Run Script Phase" in your project's target's "Build Phases",
 *   to copy all resource files:
 *   @code
 *     ZF_ROOT_PATH=${SRCROOT}/path_to_ZFFramework
 *     ZF_TOOLS_PATH=$ZF_ROOT_PATH/tools
 *     RES_DST_PATH=${CONFIGURATION_BUILD_DIR}/${CONTENTS_FOLDER_PATH}/zfres
 *
 *     sh "$ZF_TOOLS_PATH/common/copy_res.sh" "$ZF_ROOT_PATH/_release/iOS/module/required_ZF_lib_name/zfres" "$RES_DST_PATH"
 *     # ...
 *
 *     sh "$ZF_TOOLS_PATH/common/copy_res.sh" "${SRCROOT}/../../../zfres" "$RES_DST_PATH"
 *   @endcode
 *   ensure all your project's resource files put under the "zfres" directory,
 *   and all of them must be copied by the script above,
 *   you should not add them to your project
 * -# create cpp files in your project
 *   and follow the @ref DocTag_Tutorial "Tutorial" to code with ZFFramework, enjoy
 */

