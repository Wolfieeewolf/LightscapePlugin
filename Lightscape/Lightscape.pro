include("../OpenRGB.plugins.pri")

QT += core gui widgets

TEMPLATE = lib
TARGET = Lightscape
VERSION = 1.0.0
DEFINES += LIGHTSCAPE_LIBRARY

CONFIG += plugin c++17

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
    include/LightscapePlugin.h \
    include/DeviceManager.h \
    include/SpatialGrid.h \
    include/GridSettingsDialog.h \
    include/LightscapeWidget.h \
    include/effects/EffectEngine.h

SOURCES += \
    src/LightscapePlugin.cpp \
    src/DeviceManager.cpp \
    src/SpatialGrid.cpp \
    src/GridSettingsDialog.cpp \
    src/LightscapeWidget.cpp \
    src/effects/EffectEngine.cpp

FORMS += \
    GridSettingsDialog.ui \
    LightscapeWidget.ui

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