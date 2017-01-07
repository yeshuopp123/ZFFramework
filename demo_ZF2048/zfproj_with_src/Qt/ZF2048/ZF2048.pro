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
ZF_PROJ_NAME = ZF2048

# build path
ZF_BUILD_PATH = $$_PRO_FILE_PWD_/../../../../_tmp

# src path of your project
# can hold one or more paths, separated by space
ZF_PROJ_SRC_PATH =
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../zfsrc

ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFCore/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFAlgorithm/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUtility/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUIKit/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFAddition/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUIWidget/zfsrc

ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZF_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZF_loader_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFCore_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFAlgorithm_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUtility_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUIKit_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFAddition_impl/zfsrc
ZF_PROJ_SRC_PATH += $$_PRO_FILE_PWD_/../../../../../ZFFramework/ZF/ZFUIWidget_impl/zfsrc

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

system($${_ZF_SCRIPT_CALL} $$system_path($$ZF_ROOT_PATH/zf3rd_setup.$${_ZF_SCRIPT_EXT}))
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
# INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFDependencyLib/zfsrc
# QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFDependencyLib/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)


INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFCore/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFCore/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFAlgorithm/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFAlgorithm/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUtility/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUtility/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUIKit/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUIKit/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFAddition/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFAddition/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUIWidget/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUIWidget/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)


INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZF_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZF_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZF_loader_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZF_loader_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFCore_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFCore_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFAlgorithm_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFAlgorithm_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUtility_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUtility_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUIKit_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUIKit_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFAddition_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFAddition_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)

INCLUDEPATH += $$ZF_ROOT_PATH/ZF/ZFUIWidget_impl/zfsrc
QMAKE_POST_LINK += $$_ZF_copy_res $$system_path($$ZF_ROOT_PATH/ZF/ZFUIWidget_impl/zfres) $$_ZF_RES_DEPLOY_PATH $$escape_expand(\\n\\t)


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

