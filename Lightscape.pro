#-----------------------------------------------------------------------------------------------#
# Lightscape Plugin QMake Project                                                              #
#-----------------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------------#
# Qt Configuration                                                                              #
#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \
    widgets                                                                                     \
    uitools                                                                                     \

DEFINES += LIGHTSCAPE_LIBRARY
TEMPLATE = lib
TARGET = Lightscape
# Explicitly set the filename to ensure it's just Lightscape.dll without version numbers
win32:TARGET_EXT = .dll

#-----------------------------------------------------------------------------------------------#
# Build Configuration                                                                           #
#-----------------------------------------------------------------------------------------------#
CONFIG +=                                                                                       \
    plugin                                                                                      \
    c++17                                                                                       \
    silent                                                                                      \

#-----------------------------------------------------------------------------------------------#
# Application Version Information                                                               #
#-----------------------------------------------------------------------------------------------#
MAJOR       = 1
MINOR       = 0
SUFFIX      = alpha

SHORTHASH   = $$system("git rev-parse --short=7 HEAD")
LASTTAG     = "release_"$$MAJOR"."$$MINOR
COMMAND     = "git rev-list --count "$$LASTTAG"..HEAD"
COMMITS     = $$system($$COMMAND)

VERSION_NUM = $$MAJOR"."$$MINOR"."$$COMMITS
VERSION_STR = $$MAJOR"."$$MINOR

VERSION_DEB = $$VERSION_NUM
VERSION_WIX = $$VERSION_NUM".0"
VERSION_AUR = $$VERSION_NUM
VERSION_RPM = $$VERSION_NUM

equals(SUFFIX, "git") {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX$$COMMITS")"
VERSION_DEB = $$VERSION_DEB"~git"$$SHORTHASH
VERSION_AUR = $$VERSION_AUR".g"$$SHORTHASH
VERSION_RPM = $$VERSION_RPM"^git"$$SHORTHASH
} else {
    !isEmpty(SUFFIX) {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX")"
VERSION_DEB = $$VERSION_DEB"~"$$SUFFIX
VERSION_AUR = $$VERSION_AUR"."$$SUFFIX
VERSION_RPM = $$VERSION_RPM"^"$$SUFFIX
    }
}

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

#-----------------------------------------------------------------------------------------------#
# Inject vars in defines                                                                        #
#-----------------------------------------------------------------------------------------------#
DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$VERSION_STR\\"\"\"                                                  \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"                                                       \

#-----------------------------------------------------------------------------------------------#
# Resources                                                                                     #
#-----------------------------------------------------------------------------------------------#
RESOURCES += resources.qrc

#-----------------------------------------------------------------------------------------------#
# Include Paths                                                                                 #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    .                                                                                           \
    include                                                                                     \
    OpenRGB                                                                                     \
    OpenRGB/qt                                                                                  \
    OpenRGB/hidapi_wrapper                                                                      \
    OpenRGB/SPDAccessor                                                                         \
    OpenRGB/Controllers                                                                         \
    OpenRGB/RGBController                                                                       \
    OpenRGB/dependencies/hidapi                                                                 \
    OpenRGB/dependencies/hidapi-win/include                                                     \
    OpenRGB/dependencies/json                                                                   \
    OpenRGB/i2c_smbus                                                                           \
    OpenRGB/i2c_tools                                                                           \
    OpenRGB/net_port                                                                            \
    generated_files                                                                             \

#-----------------------------------------------------------------------------------------------#
# Headers                                                                                       #
#-----------------------------------------------------------------------------------------------#
HEADERS +=                                                                                      \
    include/core/Types.h                                                                        \
    include/core/LightscapePlugin.h                                                             \
    include/core/DeviceManager.h                                                                \
    include/core/LightscapeWidget.h                                                             \
    include/core/SettingsManager.h                                                              \
    include/core/ThemeManager.h                                                                 \
    include/core/ResourceHandler.h                                                              \
    include/core/TrayMenuManager.h                                                              \
    include/core/StateManager.h                                                                 \
    include/core/VersionManager.h                                                               \
    include/core/ReferencePointSelector.h                                                       \
    include/grid/GridTypes.h                                                                    \
    include/grid/SpatialGrid.h                                                                  \
    include/grid/GridPanel.h                                                                    \
    include/grid/GridSettingsDialog.h                                                           \
    include/grid/NonRGBGridManager.h                                                            \
    include/devices/CustomDeviceDialog.h                                                        \
    include/devices/DeviceControlWidget.h                                                       \
    include/devices/NonRGBDevice.h                                                              \
    include/devices/NonRGBDeviceTypes.h                                                         \
    include/devices/NonRGBDeviceManager.h                                                       \
    include/assignments/AssignmentsWidget.h                                                     \
    include/effects/EffectWidget.h                                                              \
    include/effects/EffectInfo.h                                                                \
    include/effects/EffectList.h                                                                \
    include/effects/EffectRegistry.h                                                            \
    include/effects/EffectManager.h                                                             \
    include/effects/BaseEffect.h                                                                \
    include/effects/TestEffect/TestEffect.h                                                     \

#-----------------------------------------------------------------------------------------------#
# Sources                                                                                       #
#-----------------------------------------------------------------------------------------------#
SOURCES +=                                                                                      \
    src/core/LightscapePlugin.cpp                                                               \
    src/core/DeviceManager.cpp                                                                  \
    src/core/LightscapeWidget.cpp                                                               \
    src/core/SettingsManager.cpp                                                                \
    src/core/ThemeManager.cpp                                                                   \
    src/core/ResourceHandler.cpp                                                                \
    src/core/TrayMenuManager.cpp                                                                \
    src/core/StateManager.cpp                                                                   \
    src/core/VersionManager.cpp                                                                 \
    src/core/ReferencePointSelector.cpp                                                         \
    src/grid/SpatialGrid.cpp                                                                    \
    src/grid/GridPanel.cpp                                                                      \
    src/grid/GridSettingsDialog.cpp                                                             \
    src/grid/NonRGBGridManager.cpp                                                              \
    src/devices/CustomDeviceDialog.cpp                                                          \
    src/devices/DeviceControlWidget.cpp                                                         \
    src/devices/NonRGBDevice.cpp                                                                \
    src/devices/NonRGBDeviceManager.cpp                                                         \
    src/assignments/AssignmentsWidget.cpp                                                       \
    src/effects/EffectWidget.cpp                                                                \
    src/effects/EffectList.cpp                                                                  \
    src/effects/EffectRegistry.cpp                                                              \
    src/effects/EffectManager.cpp                                                               \
    src/effects/BaseEffect.cpp                                                                  \
    src/effects/TestEffect/TestEffect.cpp                                                       \

#-----------------------------------------------------------------------------------------------#
# Forms                                                                                         #
#-----------------------------------------------------------------------------------------------#
FORMS +=                                                                                        \
    ui/devices/DeviceControlWidget.ui                                                           \
    ui/assignments/AssignmentsWidget.ui                                                         \
    ui/grid/GridSettingsDialog.ui                                                               \
    ui/effects/EffectWidget.ui                                                                  \

#-----------------------------------------------------------------------------------------------#
# Platform-specific settings                                                                    #
#-----------------------------------------------------------------------------------------------#
win32 {
    DEFINES += WIN32 _MBCS _CRT_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS WIN32_LEAN_AND_MEAN
    LIBS += -lsetupapi -lhid -lws2_32 -lole32
    DESTDIR = $(APPDATA)/OpenRGB/plugins
}

unix:!macx {
    LIBS += -ludev
    DESTDIR = ~/.config/OpenRGB/plugins
    target.path=$$PREFIX/lib/openrgb/plugins/
    INSTALLS += target
}

macx {
    LIBS += -framework IOKit -framework CoreFoundation
    DESTDIR = ~/Library/Application Support/OpenRGB/plugins
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

#-----------------------------------------------------------------------------------------------#
# Build Directories                                                                             #
#-----------------------------------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

unix:!macx:OBJECTS_DIR = build/obj
unix:!macx:MOC_DIR = build/moc
unix:!macx:UI_DIR = build/ui
unix:!macx:RCC_DIR = build/rcc

macx:OBJECTS_DIR = build/obj
macx:MOC_DIR = build/moc
macx:UI_DIR = build/ui
macx:RCC_DIR = build/rcc