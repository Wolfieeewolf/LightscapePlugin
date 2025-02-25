/****************************************************************************
** Meta object code from reading C++ file 'EffectWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/effects/EffectWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'EffectWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_EffectWidget_t {
    QByteArrayData data[15];
    char stringdata0[221];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_EffectWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_EffectWidget_t qt_meta_stringdata_EffectWidget = {
    {
QT_MOC_LITERAL(0, 0, 12), // "EffectWidget"
QT_MOC_LITERAL(1, 13, 14), // "previewToggled"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "enabled"
QT_MOC_LITERAL(4, 37, 14), // "effectSelected"
QT_MOC_LITERAL(5, 52, 10), // "effectName"
QT_MOC_LITERAL(6, 63, 22), // "deviceSelectionChanged"
QT_MOC_LITERAL(7, 86, 17), // "updateEffectsList"
QT_MOC_LITERAL(8, 104, 16), // "updateDeviceList"
QT_MOC_LITERAL(9, 121, 22), // "updateDeviceSelections"
QT_MOC_LITERAL(10, 144, 16), // "onPreviewToggled"
QT_MOC_LITERAL(11, 161, 7), // "checked"
QT_MOC_LITERAL(12, 169, 16), // "onEffectSelected"
QT_MOC_LITERAL(13, 186, 15), // "onDeviceToggled"
QT_MOC_LITERAL(14, 202, 18) // "onSelectAllClicked"

    },
    "EffectWidget\0previewToggled\0\0enabled\0"
    "effectSelected\0effectName\0"
    "deviceSelectionChanged\0updateEffectsList\0"
    "updateDeviceList\0updateDeviceSelections\0"
    "onPreviewToggled\0checked\0onEffectSelected\0"
    "onDeviceToggled\0onSelectAllClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_EffectWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    1,   67,    2, 0x06 /* Public */,
       6,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   71,    2, 0x0a /* Public */,
       8,    0,   72,    2, 0x0a /* Public */,
       9,    0,   73,    2, 0x0a /* Public */,
      10,    1,   74,    2, 0x08 /* Private */,
      12,    0,   77,    2, 0x08 /* Private */,
      13,    0,   78,    2, 0x08 /* Private */,
      14,    0,   79,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void EffectWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<EffectWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->previewToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->effectSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->deviceSelectionChanged(); break;
        case 3: _t->updateEffectsList(); break;
        case 4: _t->updateDeviceList(); break;
        case 5: _t->updateDeviceSelections(); break;
        case 6: _t->onPreviewToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->onEffectSelected(); break;
        case 8: _t->onDeviceToggled(); break;
        case 9: _t->onSelectAllClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (EffectWidget::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EffectWidget::previewToggled)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (EffectWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EffectWidget::effectSelected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (EffectWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&EffectWidget::deviceSelectionChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject EffectWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_EffectWidget.data,
    qt_meta_data_EffectWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *EffectWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *EffectWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_EffectWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int EffectWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void EffectWidget::previewToggled(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void EffectWidget::effectSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void EffectWidget::deviceSelectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
