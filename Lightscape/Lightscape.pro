include("../OpenRGB.plugins.pri")

QT += core gui widgets

TEMPLATE = lib
TARGET = Lightscape
VERSION = 1.0.0
DEFINES += LIGHTSCAPE_LIBRARY

CONFIG += plugin c++17

RESOURCES += resources.qrc

INCLUDEPATH += \
    . \
    include \
    ../../ \
    ../../qt/ \
    ../../hidapi_wrapper \
    ../../SPDAccessor \
    ../../Controllers \
    ../../RGBController \
    ../../dependencies/hidapi \
    ../../dependencies/hidapi-win/include \
    ../../dependencies/json \
    ../../i2c_smbus \
    ../../i2c_tools \
    ../../net_port \
    generated_files

HEADERS += \
    include/core/LightscapePlugin.h \
    include/core/DeviceManager.h \
    include/core/LightscapeWidget.h \
    include/core/ThemeManager.h \
    include/core/ResourceHandler.h \
    include/core/TrayMenuManager.h \
    include/core/StateManager.h \
    include/core/VersionManager.h \
    include/grid/SpatialGrid.h \
    include/grid/GridPanel.h \
    include/grid/GridSettingsDialog.h \
    include/grid/NonRGBGridManager.h \
    include/devices/DeviceControlWidget.h \
    include/devices/NonRGBDevice.h \
    include/devices/NonRGBDeviceTypes.h \
    include/devices/NonRGBDeviceManager.h \
    include/devices/NonRGBDeviceWidget.h \
    include/devices/NonRGBDevicesPanel.h \
    include/assignments/AssignmentsWidget.h \
    include/effects/base/EffectEngine.h \
    include/effects/game/GameEffectBase.h \
    include/effects/game/GameEffectManager.h \
    include/effects/game/ExplosionEffect.h \
    include/effects/ui/EffectsPanel.h \
    include/game/GameDataHandler.h

SOURCES += \
    src/core/LightscapePlugin.cpp \
    src/core/DeviceManager.cpp \
    src/core/LightscapeWidget.cpp \
    src/core/ThemeManager.cpp \
    src/core/ResourceHandler.cpp \
    src/core/TrayMenuManager.cpp \
    src/core/StateManager.cpp \
    src/core/VersionManager.cpp \
    src/grid/SpatialGrid.cpp \
    src/grid/GridPanel.cpp \
    src/grid/GridSettingsDialog.cpp \
    src/grid/NonRGBGridManager.cpp \
    src/devices/DeviceControlWidget.cpp \
    src/devices/NonRGBDevice.cpp \
    src/devices/NonRGBDeviceManager.cpp \
    src/devices/NonRGBDeviceWidget.cpp \
    src/devices/NonRGBDevicesPanel.cpp \
    src/assignments/AssignmentsWidget.cpp \
    src/effects/base/EffectEngine.cpp \
    src/effects/game/GameEffectBase.cpp \
    src/effects/game/GameEffectManager.cpp \
    src/effects/game/ExplosionEffect.cpp \
    src/effects/ui/EffectsPanel.cpp \
    src/game/GameDataHandler.cpp

FORMS += \
    ui/core/LightscapeWidget.ui \
    ui/devices/DeviceControlWidget.ui \
    ui/assignments/AssignmentsWidget.ui \
    ui/grid/GridSettingsDialog.ui

# Platform-specific settings
win32 {
    DEFINES += WIN32
    LIBS += -lsetupapi -lhid
    DESTDIR = $$(APPDATA)/OpenRGB/plugins
}

unix:!macx {
    LIBS += -ludev
    DESTDIR = ~/.config/OpenRGB/plugins
}

macx {
    LIBS += -framework IOKit -framework CoreFoundation
    DESTDIR = ~/Library/Application Support/OpenRGB/plugins
}

# Generate files in build directory
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
UI_DIR = build/ui
RCC_DIR = build/rcc