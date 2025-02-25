QT += core gui widgets uitools

TEMPLATE = lib
TARGET = Lightscape
VERSION = 1.0.0
DEFINES += LIGHTSCAPE_LIBRARY

CONFIG += plugin c++17

RESOURCES += resources.qrc

INCLUDEPATH += \
    . \
    include \
    OpenRGB \
    OpenRGB/qt \
    OpenRGB/hidapi_wrapper \
    OpenRGB/SPDAccessor \
    OpenRGB/Controllers \
    OpenRGB/RGBController \
    OpenRGB/dependencies/hidapi \
    OpenRGB/dependencies/hidapi-win/include \
    OpenRGB/dependencies/json \
    OpenRGB/i2c_smbus \
    OpenRGB/i2c_tools \
    OpenRGB/net_port \
    generated_files

HEADERS += \
    include/core/Types.h \
    include/core/LightscapePlugin.h \
    include/core/DeviceManager.h \
    include/core/LightscapeWidget.h \
    include/core/SettingsManager.h \
    include/core/ThemeManager.h \
    include/core/ResourceHandler.h \
    include/core/TrayMenuManager.h \
    include/core/StateManager.h \
    include/core/VersionManager.h \
    include/core/ReferencePointSelector.h \
    include/grid/GridTypes.h \
    include/grid/SpatialGrid.h \
    include/grid/GridPanel.h \
    include/grid/GridSettingsDialog.h \
    include/grid/NonRGBGridManager.h \
    include/devices/CustomDeviceDialog.h \
    include/devices/DeviceControlWidget.h \
    include/devices/NonRGBDevice.h \
    include/devices/NonRGBDeviceTypes.h \
    include/devices/NonRGBDeviceManager.h \
    include/assignments/AssignmentsWidget.h \
    include/effects/EffectWidget.h \
    include/effects/EffectInfo.h \
    include/effects/EffectList.h \
    include/effects/EffectRegistry.h \
    include/effects/EffectManager.h \
    include/effects/BaseEffect.h \
    include/effects/TestEffect/TestEffect.h

SOURCES += \
    src/core/LightscapePlugin.cpp \
    src/core/DeviceManager.cpp \
    src/core/LightscapeWidget.cpp \
    src/core/SettingsManager.cpp \
    src/core/ThemeManager.cpp \
    src/core/ResourceHandler.cpp \
    src/core/TrayMenuManager.cpp \
    src/core/StateManager.cpp \
    src/core/VersionManager.cpp \
    src/core/ReferencePointSelector.cpp \
    src/grid/SpatialGrid.cpp \
    src/grid/GridPanel.cpp \
    src/grid/GridSettingsDialog.cpp \
    src/grid/NonRGBGridManager.cpp \
    src/devices/CustomDeviceDialog.cpp \
    src/devices/DeviceControlWidget.cpp \
    src/devices/NonRGBDevice.cpp \
    src/devices/NonRGBDeviceManager.cpp \
    src/assignments/AssignmentsWidget.cpp \
    src/effects/EffectWidget.cpp \
    src/effects/EffectList.cpp \
    src/effects/EffectRegistry.cpp \
    src/effects/EffectManager.cpp \
    src/effects/BaseEffect.cpp \
    src/effects/TestEffect/TestEffect.cpp

FORMS += \
    ui/devices/DeviceControlWidget.ui \
    ui/assignments/AssignmentsWidget.ui \
    ui/grid/GridSettingsDialog.ui \
    ui/effects/EffectWidget.ui

# Platform-specific settings
win32 {
    DEFINES += WIN32
    LIBS += -lsetupapi -lhid
    DESTDIR = $(APPDATA)/OpenRGB/plugins
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