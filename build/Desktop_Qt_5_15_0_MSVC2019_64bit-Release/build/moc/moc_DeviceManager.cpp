/****************************************************************************
** Meta object code from reading C++ file 'DeviceManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/core/DeviceManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DeviceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DeviceManager_t {
    QByteArrayData data[18];
    char stringdata0[239];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceManager_t qt_meta_stringdata_DeviceManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DeviceManager"
QT_MOC_LITERAL(1, 14, 17), // "deviceListChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "deviceUpdated"
QT_MOC_LITERAL(4, 47, 11), // "deviceIndex"
QT_MOC_LITERAL(5, 59, 22), // "Lightscape::DeviceType"
QT_MOC_LITERAL(6, 82, 4), // "type"
QT_MOC_LITERAL(7, 87, 13), // "errorOccurred"
QT_MOC_LITERAL(8, 101, 5), // "error"
QT_MOC_LITERAL(9, 107, 17), // "nonRGBDeviceAdded"
QT_MOC_LITERAL(10, 125, 13), // "NonRGBDevice*"
QT_MOC_LITERAL(11, 139, 6), // "device"
QT_MOC_LITERAL(12, 146, 19), // "nonRGBDeviceRemoved"
QT_MOC_LITERAL(13, 166, 10), // "deviceName"
QT_MOC_LITERAL(14, 177, 20), // "nonRGBDeviceModified"
QT_MOC_LITERAL(15, 198, 23), // "deviceAssignmentChanged"
QT_MOC_LITERAL(16, 222, 5), // "index"
QT_MOC_LITERAL(17, 228, 10) // "isAssigned"

    },
    "DeviceManager\0deviceListChanged\0\0"
    "deviceUpdated\0deviceIndex\0"
    "Lightscape::DeviceType\0type\0errorOccurred\0"
    "error\0nonRGBDeviceAdded\0NonRGBDevice*\0"
    "device\0nonRGBDeviceRemoved\0deviceName\0"
    "nonRGBDeviceModified\0deviceAssignmentChanged\0"
    "index\0isAssigned"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    2,   50,    2, 0x06 /* Public */,
       7,    1,   55,    2, 0x06 /* Public */,
       9,    1,   58,    2, 0x06 /* Public */,
      12,    1,   61,    2, 0x06 /* Public */,
      14,    1,   64,    2, 0x06 /* Public */,
      15,    3,   67,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 5,    4,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 5, QMetaType::Bool,   16,    6,   17,

       0        // eod
};

void DeviceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DeviceManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceListChanged(); break;
        case 1: _t->deviceUpdated((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Lightscape::DeviceType(*)>(_a[2]))); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->nonRGBDeviceAdded((*reinterpret_cast< NonRGBDevice*(*)>(_a[1]))); break;
        case 4: _t->nonRGBDeviceRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->nonRGBDeviceModified((*reinterpret_cast< NonRGBDevice*(*)>(_a[1]))); break;
        case 6: _t->deviceAssignmentChanged((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< Lightscape::DeviceType(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DeviceManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::deviceListChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(int , Lightscape::DeviceType );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::deviceUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(NonRGBDevice * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::nonRGBDeviceAdded)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::nonRGBDeviceRemoved)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(NonRGBDevice * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::nonRGBDeviceModified)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (DeviceManager::*)(unsigned int , Lightscape::DeviceType , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceManager::deviceAssignmentChanged)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DeviceManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_DeviceManager.data,
    qt_meta_data_DeviceManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DeviceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DeviceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void DeviceManager::deviceListChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DeviceManager::deviceUpdated(int _t1, Lightscape::DeviceType _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DeviceManager::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void DeviceManager::nonRGBDeviceAdded(NonRGBDevice * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void DeviceManager::nonRGBDeviceRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void DeviceManager::nonRGBDeviceModified(NonRGBDevice * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void DeviceManager::deviceAssignmentChanged(unsigned int _t1, Lightscape::DeviceType _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
