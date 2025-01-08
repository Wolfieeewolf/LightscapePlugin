OPENRGB_INCLUDEPATH += \
    $$PWD/.. \
    $$PWD/../qt \
    $$PWD/../hidapi_wrapper \
    $$PWD/../SPDAccessor \
    $$PWD/../Controllers \
    $$PWD/../RGBController \
    $$PWD/../dependencies/hidapi \
    $$PWD/../dependencies/json \
    $$PWD/../i2c_smbus \
    $$PWD/../net_port \
    $$PWD/../qt/OpenRGBPluginInterface

CONFIG += plugin c++17

DESTDIR = $$PWD/../bin/plugins