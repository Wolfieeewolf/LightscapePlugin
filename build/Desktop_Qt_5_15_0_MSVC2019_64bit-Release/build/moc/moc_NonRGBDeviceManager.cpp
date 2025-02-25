/****************************************************************************
** Meta object code from reading C++ file 'NonRGBDeviceManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/devices/NonRGBDeviceManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NonRGBDeviceManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NonRGBDeviceManager_t {
    QByteArrayData data[10];
    char stringdata0[108];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NonRGBDeviceManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NonRGBDeviceManager_t qt_meta_stringdata_NonRGBDeviceManager = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NonRGBDeviceManager"
QT_MOC_LITERAL(1, 20, 11), // "deviceAdded"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 13), // "NonRGBDevice*"
QT_MOC_LITERAL(4, 47, 6), // "device"
QT_MOC_LITERAL(5, 54, 13), // "deviceRemoved"
QT_MOC_LITERAL(6, 68, 4), // "name"
QT_MOC_LITERAL(7, 73, 14), // "deviceModified"
QT_MOC_LITERAL(8, 88, 13), // "errorOccurred"
QT_MOC_LITERAL(9, 102, 5) // "error"

    },
    "NonRGBDeviceManager\0deviceAdded\0\0"
    "NonRGBDevice*\0device\0deviceRemoved\0"
    "name\0deviceModified\0errorOccurred\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NonRGBDeviceManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       5,    1,   37,    2, 0x06 /* Public */,
       7,    1,   40,    2, 0x06 /* Public */,
       8,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QString,    9,

       0        // eod
};

void NonRGBDeviceManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NonRGBDeviceManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceAdded((*reinterpret_cast< NonRGBDevice*(*)>(_a[1]))); break;
        case 1: _t->deviceRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->deviceModified((*reinterpret_cast< NonRGBDevice*(*)>(_a[1]))); break;
        case 3: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NonRGBDeviceManager::*)(NonRGBDevice * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NonRGBDeviceManager::deviceAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NonRGBDeviceManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NonRGBDeviceManager::deviceRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NonRGBDeviceManager::*)(NonRGBDevice * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NonRGBDeviceManager::deviceModified)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NonRGBDeviceManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NonRGBDeviceManager::errorOccurred)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NonRGBDeviceManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NonRGBDeviceManager.data,
    qt_meta_data_NonRGBDeviceManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NonRGBDeviceManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NonRGBDeviceManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NonRGBDeviceManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NonRGBDeviceManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void NonRGBDeviceManager::deviceAdded(NonRGBDevice * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NonRGBDeviceManager::deviceRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NonRGBDeviceManager::deviceModified(NonRGBDevice * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NonRGBDeviceManager::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
