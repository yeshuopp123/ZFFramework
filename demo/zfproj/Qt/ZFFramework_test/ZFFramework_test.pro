# ======================================================================
# Copyright (c) 2010-2016 ZFFramework
# home page: http://ZFFramework.com
# blog: http://zsaber.com
# contact: master@zsaber.com (Chinese and English only)
# Distributed under MIT license:
#   https://github.com/ZFFramework/ZFFramework/blob/master/license/license.txt
# ======================================================================

# ======================================================================
# app template for ZFFramework
# ======================================================================

# ======================================================================
# ZF settings
# ======================================================================

# whether to build static lib
ZF_BUILD_STATIC_LIB = 0

# whether to use unity builds
# NOTE: you must ensure no Q_OBJECT used while unity builds enabled
ZF_UNITY_BUILD = 0

# ZFFramework's root path
ZF_ROOT_PATH = $$_PRO_FILE_PWD_/../../../../../ZFFramework
ZF_TOOLS_PATH = $$ZF_ROOT_PATH/tools

# name of your project
ZF_PROJ_NAME = ZFFramework_test

# build path
ZF_BUILD_PATH = $$_PRO_FILE_PWD_/../../../../_tmp

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH =
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../zfsrc

# res path of your project
# can hold one or more paths, separated by space
ZF_PROJ_RES_PATH = $$_PRO_FILE_PWD_/../../../zfres

win32 {
    _ZF_QT_TYPE=Qt_Windows
}
unix:!macx {
    _ZF_QT_TYPE=Qt_Posix
}
macx {
    _ZF_QT_TYPE=Qt_MacOS
}


# ======================================================================
# your custom project settings here
# ======================================================================
win32 {
    system(call $$system_path($$_PRO_FILE_PWD_/../../../ZFSetup.bat))
}
unix {
    system(sh $$system_path($$_PRO_FILE_PWD_/../../../ZFSetup.sh))
}

# Qt modules
# QT += gui widgets
# qtHaveModule(webenginewidgets) {QT += webenginewidgets} else {qtHaveModule(webkitwidgets) : QT += webkitwidgets}
QT += gui widgets


# ======================================================================
# Qt project settings, no need to change for most case
# ======================================================================
INCLUDEPATH += $$_PRO_FILE_PWD_/../../../zfsrc
INCLUDEPATH += $$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/include

QT += core

TARGET = $$ZF_PROJ_NAME
TEMPLATE = app

QMAKE_CXXFLAGS += -Wno-unused-parameter
CONFIG += warn_off

exists($${ZF_PROJ_NAME}.ico) {
    RC_ICONS = $${ZF_PROJ_NAME}.ico
}
exists($${ZF_PROJ_NAME}.icns) {
    ICON = $${ZF_PROJ_NAME}.icns
}


# ======================================================================
# no need to change these
# ======================================================================
win32 {
    # NOTE: for 32-bit MinGW, it's too easy to reach section limit
    #       (too many sections), disable it for Windows by default
    ZF_UNITY_BUILD = 0
}

win32 {
    _ZF_QT_TYPE=Qt_Windows
    _ZF_SCRIPT_CALL=call
    _ZF_SCRIPT_EXT=bat
}
unix:!macx {
    _ZF_QT_TYPE=Qt_Posix
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
}
macx {
    _ZF_QT_TYPE=Qt_MacOS
    _ZF_SCRIPT_CALL=sh
    _ZF_SCRIPT_EXT=sh
}

system($${_ZF_SCRIPT_CALL} $$system_path($$ZF_TOOLS_PATH/release/release_$${_ZF_QT_TYPE}_check.$${_ZF_SCRIPT_EXT}))
system($${_ZF_SCRIPT_CALL} $$system_path($$ZF_TOOLS_PATH/common/zfsh_if_exist.$${_ZF_SCRIPT_EXT}) $$system_path($$_PRO_FILE_PWD_/../../../zfscript/zfmodule_setup.zfsh) $$_ZF_QT_TYPE)

CONFIG(debug, debug|release) {
    _ZF_BUILD_TYPE=debug
} else {
    _ZF_BUILD_TYPE=release
}

DESTDIR = $$ZF_BUILD_PATH/$$ZF_PROJ_NAME/$$_ZF_QT_TYPE/$$_ZF_BUILD_TYPE
OBJECTS_DIR = $${DESTDIR}/.obj
MOC_DIR = $${DESTDIR}/.moc
RCC_DIR = $${DESTDIR}/.rcc
UI_DIR = $${DESTDIR}/.ui

win32 {
    _ZF_copy_res = $$system_path($$ZF_TOOLS_PATH/common/copy_res.bat)
    _ZF_install_lib = $$system_path($$ZF_TOOLS_PATH/common/install_lib.bat)
} else {
    _ZF_copy_res = sh $$system_path($$ZF_TOOLS_PATH/common/copy_res.sh)
    _ZF_install_lib = sh $$system_path($$ZF_TOOLS_PATH/common/install_lib.sh)
}

equals(ZF_BUILD_STATIC_LIB, 1) {
    _ZF_install_lib = echo
}

win32 {
    _ZF_RES_DEPLOY_PATH=$$system_path($$DESTDIR/zfres)
    _ZF_LIB_DEPLOY_PATH=$$system_path($$DESTDIR/.)
}
unix:!macx {
    _ZF_RES_DEPLOY_PATH=$$system_path($$DESTDIR/zfres)
    _ZF_LIB_DEPLOY_PATH=$$system_path($$DESTDIR/.)
}
macx {
    _ZF_RES_DEPLOY_PATH=$$system_path($$DESTDIR/"$$TARGET".app/Contents/Resources/zfres)
    _ZF_LIB_DEPLOY_PATH=$$system_path($$DESTDIR/"$$TARGET".app/Contents/Frameworks)
}

equals(ZF_BUILD_STATIC_LIB, 1) {
    macx {
        _ZF_LINKER_FLAGS = -Wl,-all_load
    } else {
        _ZF_LINKER_FLAGS = -Wl,--whole-archive
    }
} else {
    _ZF_LINKER_FLAGS =
}

# ======================================================================
# lib dependency
# ======================================================================
# ZF dependency
# LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib -lZFDependencyLib
# QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFDependencyLib/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
# QMAKE_POST_LINK += $$_ZF_install_lib ZFDependencyLib $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFDependencyLib/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
# macx {QMAKE_POST_LINK += install_name_tool -change "libZFDependency.1.dylib" "@rpath/libZFDependency.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}


LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFCore
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFCore/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFCore $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFCore/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFCore.1.dylib" "@rpath/libZFCore.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFAlgorithm
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAlgorithm/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFAlgorithm $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAlgorithm/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFAlgorithm.1.dylib" "@rpath/libZFAlgorithm.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUtility
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUtility/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUtility $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUtility/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUtility.1.dylib" "@rpath/libZFUtility.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIKit
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIKit/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIKit $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIKit/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIKit.1.dylib" "@rpath/libZFUIKit.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFAddition
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAddition/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFAddition $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAddition/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFAddition.1.dylib" "@rpath/libZFAddition.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIWidget
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWidget/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIWidget $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWidget/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIWidget.1.dylib" "@rpath/libZFUIWidget.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIWebKit
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWebKit/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIWebKit $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWebKit/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIWebKit.1.dylib" "@rpath/libZFUIWebKit.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}



LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZF_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZF_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZF_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZF_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZF_impl.1.dylib" "@rpath/libZF_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZF_loader_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZF_loader_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZF_loader_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZF_loader_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZF_loader_impl.1.dylib" "@rpath/libZF_loader_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFCore_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFCore_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFCore_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFCore_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFCore_impl.1.dylib" "@rpath/libZFCore_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFAlgorithm_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAlgorithm_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFAlgorithm_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAlgorithm_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFAlgorithm_impl.1.dylib" "@rpath/libZFAlgorithm_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUtility_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUtility_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUtility_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUtility_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUtility_impl.1.dylib" "@rpath/libZFUtility_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIKit_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIKit_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIKit_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIKit_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIKit_impl.1.dylib" "@rpath/libZFUIKit_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFAddition_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAddition_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFAddition_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFAddition_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFAddition_impl.1.dylib" "@rpath/libZFAddition_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIWidget_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWidget_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIWidget_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWidget_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIWidget_impl.1.dylib" "@rpath/libZFUIWidget_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}

LIBS += -L$$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/all/lib $$_ZF_LINKER_FLAGS -lZFUIWebKit_impl
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWebKit_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
QMAKE_POST_LINK += $$_ZF_install_lib ZFUIWebKit_impl $$system_path($$ZF_ROOT_PATH/_release/$$_ZF_QT_TYPE/module/ZFUIWebKit_impl/lib) $$_ZF_LIB_DEPLOY_PATH $$escape_expand(\\n\\t)
macx {QMAKE_POST_LINK += install_name_tool -change "libZFUIWebKit_impl.1.dylib" "@rpath/libZFUIWebKit_impl.dylib" $$system_path($$DESTDIR/"$$TARGET".app/Contents/MacOS/$$ZF_PROJ_NAME) $$escape_expand(\\n\\t)}


# ======================================================================
exists(qt_main.cpp) {
    SOURCES += qt_main.cpp
}

equals(ZF_UNITY_BUILD, 1) {
    win32 {
        _ZF_unity_build_cmd = call $$system_path($$ZF_TOOLS_PATH/common/unity_build.bat)
    } else {
        _ZF_unity_build_cmd = sh $$system_path($$ZF_TOOLS_PATH/common/unity_build.sh)
    }
    for(src_path, ZF_PROJ_SRC_PATH) {
        _ZF_COMPILE_MODULE_NAME = $$src_path
        _ZF_COMPILE_MODULE_NAME = $$replace(_ZF_COMPILE_MODULE_NAME,[\\/\.:],_)
        _ZF_COMPILE_MODULE_NAME = $$replace(_ZF_COMPILE_MODULE_NAME,__+,_)
        _ZF_UNITY_BUILD_FILE = zfgensrc_$${ZF_PROJ_NAME}_$${_ZF_COMPILE_MODULE_NAME}.cpp
        system($$_ZF_unity_build_cmd $$system_path($$_ZF_UNITY_BUILD_FILE) $$system_path($$src_path))
        SOURCES += $$system_path($$_ZF_UNITY_BUILD_FILE)
    }
} else {
    win32 {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("dir /s /b $$system_path($$path\\*.cpp) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$path\\*.h) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$path\\*.hh) 2>nul")
            HEADERS += $$system("dir /s /b $$system_path($$path\\*.hpp) 2>nul")
        }
    } else {
        for(path, ZF_PROJ_SRC_PATH) {
            SOURCES += $$system("find $$system_path($$path) -name \*.cpp 2>/dev/null")
            HEADERS += $$system("find $$system_path($$path) -name \*.h 2>/dev/null")
            HEADERS += $$system("find $$system_path($$path) -name \*.hh 2>/dev/null")
            HEADERS += $$system("find $$system_path($$path) -name \*.hpp 2>/dev/null")
        }
    }
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

for(path, ZF_PROJ_RES_PATH) {
    QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$path) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)
}

equals(ZF_BUILD_STATIC_LIB, 0) {
    unix:!macx {
        QMAKE_LFLAGS += -Wl,--rpath=${ORIGIN}
    }
}

macx {
    QMAKE_POST_LINK += macdeployqt $$system_path($$DESTDIR/"$$TARGET".app) >/dev/null 2>&1 $$escape_expand(\\n\\t)
}

