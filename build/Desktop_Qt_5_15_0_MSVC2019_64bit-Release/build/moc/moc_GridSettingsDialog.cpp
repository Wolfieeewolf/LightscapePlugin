/****************************************************************************
** Meta object code from reading C++ file 'GridSettingsDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/grid/GridSettingsDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GridSettingsDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GridSettingsDialog_t {
    QByteArrayData data[18];
    char stringdata0[281];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GridSettingsDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GridSettingsDialog_t qt_meta_stringdata_GridSettingsDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "GridSettingsDialog"
QT_MOC_LITERAL(1, 19, 11), // "gridUpdated"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 20), // "userPositionRequired"
QT_MOC_LITERAL(4, 53, 7), // "warning"
QT_MOC_LITERAL(5, 61, 19), // "userPositionChanged"
QT_MOC_LITERAL(6, 81, 12), // "GridPosition"
QT_MOC_LITERAL(7, 94, 3), // "pos"
QT_MOC_LITERAL(8, 98, 20), // "on_dimensionsChanged"
QT_MOC_LITERAL(9, 119, 15), // "on_labelChanged"
QT_MOC_LITERAL(10, 135, 3), // "row"
QT_MOC_LITERAL(11, 139, 6), // "column"
QT_MOC_LITERAL(12, 146, 20), // "on_layerLabelChanged"
QT_MOC_LITERAL(13, 167, 5), // "index"
QT_MOC_LITERAL(14, 173, 15), // "on_applyClicked"
QT_MOC_LITERAL(15, 189, 23), // "on_resetDefaultsClicked"
QT_MOC_LITERAL(16, 213, 32), // "on_setUserPositionButton_clicked"
QT_MOC_LITERAL(17, 246, 34) // "on_clearUserPositionButton_cl..."

    },
    "GridSettingsDialog\0gridUpdated\0\0"
    "userPositionRequired\0warning\0"
    "userPositionChanged\0GridPosition\0pos\0"
    "on_dimensionsChanged\0on_labelChanged\0"
    "row\0column\0on_layerLabelChanged\0index\0"
    "on_applyClicked\0on_resetDefaultsClicked\0"
    "on_setUserPositionButton_clicked\0"
    "on_clearUserPositionButton_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GridSettingsDialog[] = {

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
       1,    0,   64,    2, 0x06 /* Public */,
       3,    1,   65,    2, 0x06 /* Public */,
       5,    1,   68,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   71,    2, 0x08 /* Private */,
       9,    2,   72,    2, 0x08 /* Private */,
      12,    1,   77,    2, 0x08 /* Private */,
      14,    0,   80,    2, 0x08 /* Private */,
      15,    0,   81,    2, 0x08 /* Private */,
      16,    0,   82,    2, 0x08 /* Private */,
      17,    0,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GridSettingsDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GridSettingsDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->gridUpdated(); break;
        case 1: _t->userPositionRequired((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->userPositionChanged((*reinterpret_cast< const GridPosition(*)>(_a[1]))); break;
        case 3: _t->on_dimensionsChanged(); break;
        case 4: _t->on_labelChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->on_layerLabelChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->on_applyClicked(); break;
        case 7: _t->on_resetDefaultsClicked(); break;
        case 8: _t->on_setUserPositionButton_clicked(); break;
        case 9: _t->on_clearUserPositionButton_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GridSettingsDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridSettingsDialog::gridUpdated)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GridSettingsDialog::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridSettingsDialog::userPositionRequired)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GridSettingsDialog::*)(const GridPosition & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridSettingsDialog::userPositionChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GridSettingsDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_GridSettingsDialog.data,
    qt_meta_data_GridSettingsDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GridSettingsDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GridSettingsDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GridSettingsDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int GridSettingsDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void GridSettingsDialog::gridUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void GridSettingsDialog::userPositionRequired(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GridSettingsDialog::userPositionChanged(const GridPosition & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
