/****************************************************************************
** Meta object code from reading C++ file 'AssignmentsWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../../include/assignments/AssignmentsWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AssignmentsWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AssignmentsWidget_t {
    QByteArrayData data[22];
    char stringdata0[309];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AssignmentsWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AssignmentsWidget_t qt_meta_stringdata_AssignmentsWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "AssignmentsWidget"
QT_MOC_LITERAL(1, 18, 15), // "assignRequested"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 11), // "deviceIndex"
QT_MOC_LITERAL(4, 47, 22), // "Lightscape::DeviceType"
QT_MOC_LITERAL(5, 70, 4), // "type"
QT_MOC_LITERAL(6, 75, 13), // "selectionName"
QT_MOC_LITERAL(7, 89, 12), // "GridPosition"
QT_MOC_LITERAL(8, 102, 3), // "pos"
QT_MOC_LITERAL(9, 106, 15), // "removeRequested"
QT_MOC_LITERAL(10, 122, 16), // "QListWidgetItem*"
QT_MOC_LITERAL(11, 139, 4), // "item"
QT_MOC_LITERAL(12, 144, 20), // "colorChangeRequested"
QT_MOC_LITERAL(13, 165, 8), // "RGBColor"
QT_MOC_LITERAL(14, 174, 5), // "color"
QT_MOC_LITERAL(15, 180, 21), // "onAssignButtonClicked"
QT_MOC_LITERAL(16, 202, 21), // "onRemoveButtonClicked"
QT_MOC_LITERAL(17, 224, 20), // "onClearButtonClicked"
QT_MOC_LITERAL(18, 245, 20), // "onColorButtonClicked"
QT_MOC_LITERAL(19, 266, 25), // "onDeviceAssignmentChanged"
QT_MOC_LITERAL(20, 292, 5), // "index"
QT_MOC_LITERAL(21, 298, 10) // "isAssigned"

    },
    "AssignmentsWidget\0assignRequested\0\0"
    "deviceIndex\0Lightscape::DeviceType\0"
    "type\0selectionName\0GridPosition\0pos\0"
    "removeRequested\0QListWidgetItem*\0item\0"
    "colorChangeRequested\0RGBColor\0color\0"
    "onAssignButtonClicked\0onRemoveButtonClicked\0"
    "onClearButtonClicked\0onColorButtonClicked\0"
    "onDeviceAssignmentChanged\0index\0"
    "isAssigned"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AssignmentsWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   54,    2, 0x06 /* Public */,
       9,    1,   63,    2, 0x06 /* Public */,
      12,    1,   66,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   69,    2, 0x08 /* Private */,
      16,    0,   70,    2, 0x08 /* Private */,
      17,    0,   71,    2, 0x08 /* Private */,
      18,    0,   72,    2, 0x08 /* Private */,
      19,    3,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4, QMetaType::QString, 0x80000000 | 7,    3,    5,    6,    8,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void, 0x80000000 | 13,   14,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, 0x80000000 | 4, QMetaType::Bool,   20,    5,   21,

       0        // eod
};

void AssignmentsWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AssignmentsWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->assignRequested((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< Lightscape::DeviceType(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])),(*reinterpret_cast< GridPosition(*)>(_a[4]))); break;
        case 1: _t->removeRequested((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 2: _t->colorChangeRequested((*reinterpret_cast< RGBColor(*)>(_a[1]))); break;
        case 3: _t->onAssignButtonClicked(); break;
        case 4: _t->onRemoveButtonClicked(); break;
        case 5: _t->onClearButtonClicked(); break;
        case 6: _t->onColorButtonClicked(); break;
        case 7: _t->onDeviceAssignmentChanged((*reinterpret_cast< uint(*)>(_a[1])),(*reinterpret_cast< Lightscape::DeviceType(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (AssignmentsWidget::*)(int , Lightscape::DeviceType , QString , GridPosition );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AssignmentsWidget::assignRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (AssignmentsWidget::*)(QListWidgetItem * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AssignmentsWidget::removeRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (AssignmentsWidget::*)(RGBColor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&AssignmentsWidget::colorChangeRequested)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject AssignmentsWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_AssignmentsWidget.data,
    qt_meta_data_AssignmentsWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *AssignmentsWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AssignmentsWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AssignmentsWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AssignmentsWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void AssignmentsWidget::assignRequested(int _t1, Lightscape::DeviceType _t2, QString _t3, GridPosition _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AssignmentsWidget::removeRequested(QListWidgetItem * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AssignmentsWidget::colorChangeRequested(RGBColor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
