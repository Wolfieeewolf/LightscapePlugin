/****************************************************************************
** Meta object code from reading C++ file 'LightscapePlugin.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/core/LightscapePlugin.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qplugin.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LightscapePlugin.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LightscapePlugin_t {
    QByteArrayData data[10];
    char stringdata0[146];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LightscapePlugin_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LightscapePlugin_t qt_meta_stringdata_LightscapePlugin = {
    {
QT_MOC_LITERAL(0, 0, 16), // "LightscapePlugin"
QT_MOC_LITERAL(1, 17, 19), // "handleResourceError"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 5), // "error"
QT_MOC_LITERAL(4, 44, 27), // "handleGridVisibilityChanged"
QT_MOC_LITERAL(5, 72, 7), // "visible"
QT_MOC_LITERAL(6, 80, 11), // "handleReset"
QT_MOC_LITERAL(7, 92, 18), // "handleStateChanged"
QT_MOC_LITERAL(8, 111, 25), // "StateManager::PluginState"
QT_MOC_LITERAL(9, 137, 8) // "newState"

    },
    "LightscapePlugin\0handleResourceError\0"
    "\0error\0handleGridVisibilityChanged\0"
    "visible\0handleReset\0handleStateChanged\0"
    "StateManager::PluginState\0newState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LightscapePlugin[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       6,    0,   40,    2, 0x08 /* Private */,
       7,    1,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,

       0        // eod
};

void LightscapePlugin::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LightscapePlugin *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->handleResourceError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->handleGridVisibilityChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->handleReset(); break;
        case 3: _t->handleStateChanged((*reinterpret_cast< StateManager::PluginState(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LightscapePlugin::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_LightscapePlugin.data,
    qt_meta_data_LightscapePlugin,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LightscapePlugin::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LightscapePlugin::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LightscapePlugin.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "OpenRGBPluginInterface"))
        return static_cast< OpenRGBPluginInterface*>(this);
    if (!strcmp(_clname, "com.OpenRGBPluginInterface"))
        return static_cast< OpenRGBPluginInterface*>(this);
    return QObject::qt_metacast(_clname);
}

int LightscapePlugin::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

QT_PLUGIN_METADATA_SECTION
static constexpr unsigned char qt_pluginMetaData[] = {
    'Q', 'T', 'M', 'E', 'T', 'A', 'D', 'A', 'T', 'A', ' ', '!',
    // metadata version, Qt version, architectural requirements
    0, QT_VERSION_MAJOR, QT_VERSION_MINOR, qPluginArchRequirements(),
    0xbf, 
    // "IID"
    0x02,  0x78,  0x1a,  'c',  'o',  'm',  '.',  'O', 
    'p',  'e',  'n',  'R',  'G',  'B',  'P',  'l', 
    'u',  'g',  'i',  'n',  'I',  'n',  't',  'e', 
    'r',  'f',  'a',  'c',  'e', 
    // "className"
    0x03,  0x70,  'L',  'i',  'g',  'h',  't',  's', 
    'c',  'a',  'p',  'e',  'P',  'l',  'u',  'g', 
    'i',  'n', 
    0xff, 
};
QT_MOC_EXPORT_PLUGIN(LightscapePlugin, LightscapePlugin)

QT_WARNING_POP
QT_END_MOC_NAMESPACE
