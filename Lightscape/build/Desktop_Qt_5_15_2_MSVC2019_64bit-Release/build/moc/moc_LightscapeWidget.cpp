/****************************************************************************
** Meta object code from reading C++ file 'LightscapeWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/LightscapeWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LightscapeWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LightscapeWidget_t {
    QByteArrayData data[22];
    char stringdata0[414];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LightscapeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LightscapeWidget_t qt_meta_stringdata_LightscapeWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "LightscapeWidget"
QT_MOC_LITERAL(1, 17, 26), // "on_configureButton_clicked"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 21), // "on_deviceComboChanged"
QT_MOC_LITERAL(4, 67, 5), // "index"
QT_MOC_LITERAL(5, 73, 24), // "on_assignmentTypeChanged"
QT_MOC_LITERAL(6, 98, 22), // "on_assignButtonClicked"
QT_MOC_LITERAL(7, 121, 22), // "on_removeButtonClicked"
QT_MOC_LITERAL(8, 144, 21), // "on_clearButtonClicked"
QT_MOC_LITERAL(9, 166, 21), // "on_colorButtonClicked"
QT_MOC_LITERAL(10, 188, 23), // "on_gridPositionSelected"
QT_MOC_LITERAL(11, 212, 12), // "GridPosition"
QT_MOC_LITERAL(12, 225, 3), // "pos"
QT_MOC_LITERAL(13, 229, 25), // "on_setUserPositionClicked"
QT_MOC_LITERAL(14, 255, 27), // "on_clearUserPositionClicked"
QT_MOC_LITERAL(15, 283, 23), // "on_userPositionRequired"
QT_MOC_LITERAL(16, 307, 7), // "warning"
QT_MOC_LITERAL(17, 315, 22), // "on_userPositionChanged"
QT_MOC_LITERAL(18, 338, 21), // "on_effectComboChanged"
QT_MOC_LITERAL(19, 360, 21), // "on_speedSliderChanged"
QT_MOC_LITERAL(20, 382, 5), // "value"
QT_MOC_LITERAL(21, 388, 25) // "on_intensitySliderChanged"

    },
    "LightscapeWidget\0on_configureButton_clicked\0"
    "\0on_deviceComboChanged\0index\0"
    "on_assignmentTypeChanged\0"
    "on_assignButtonClicked\0on_removeButtonClicked\0"
    "on_clearButtonClicked\0on_colorButtonClicked\0"
    "on_gridPositionSelected\0GridPosition\0"
    "pos\0on_setUserPositionClicked\0"
    "on_clearUserPositionClicked\0"
    "on_userPositionRequired\0warning\0"
    "on_userPositionChanged\0on_effectComboChanged\0"
    "on_speedSliderChanged\0value\0"
    "on_intensitySliderChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LightscapeWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x08 /* Private */,
       3,    1,   90,    2, 0x08 /* Private */,
       5,    0,   93,    2, 0x08 /* Private */,
       6,    0,   94,    2, 0x08 /* Private */,
       7,    0,   95,    2, 0x08 /* Private */,
       8,    0,   96,    2, 0x08 /* Private */,
       9,    0,   97,    2, 0x08 /* Private */,
      10,    1,   98,    2, 0x08 /* Private */,
      13,    0,  101,    2, 0x08 /* Private */,
      14,    0,  102,    2, 0x08 /* Private */,
      15,    1,  103,    2, 0x08 /* Private */,
      17,    1,  106,    2, 0x08 /* Private */,
      18,    1,  109,    2, 0x08 /* Private */,
      19,    1,  112,    2, 0x08 /* Private */,
      21,    1,  115,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   16,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::Int,   20,

       0        // eod
};

void LightscapeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LightscapeWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_configureButton_clicked(); break;
        case 1: _t->on_deviceComboChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->on_assignmentTypeChanged(); break;
        case 3: _t->on_assignButtonClicked(); break;
        case 4: _t->on_removeButtonClicked(); break;
        case 5: _t->on_clearButtonClicked(); break;
        case 6: _t->on_colorButtonClicked(); break;
        case 7: _t->on_gridPositionSelected((*reinterpret_cast< const GridPosition(*)>(_a[1]))); break;
        case 8: _t->on_setUserPositionClicked(); break;
        case 9: _t->on_clearUserPositionClicked(); break;
        case 10: _t->on_userPositionRequired((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->on_userPositionChanged((*reinterpret_cast< const GridPosition(*)>(_a[1]))); break;
        case 12: _t->on_effectComboChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_speedSliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->on_intensitySliderChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LightscapeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_LightscapeWidget.data,
    qt_meta_data_LightscapeWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LightscapeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LightscapeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LightscapeWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int LightscapeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
