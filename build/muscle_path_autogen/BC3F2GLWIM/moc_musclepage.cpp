/****************************************************************************
** Meta object code from reading C++ file 'musclepage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/qtwindow/page/musclepage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'musclepage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_musclepage_t {
    QByteArrayData data[13];
    char stringdata0[205];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_musclepage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_musclepage_t qt_meta_stringdata_musclepage = {
    {
QT_MOC_LITERAL(0, 0, 10), // "musclepage"
QT_MOC_LITERAL(1, 11, 17), // "plusbuttonsetting"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 17), // "savebuttonsetting"
QT_MOC_LITERAL(4, 48, 19), // "deletebuttonsetting"
QT_MOC_LITERAL(5, 68, 22), // "newmusclebuttonsetting"
QT_MOC_LITERAL(6, 91, 17), // "showmusclesetting"
QT_MOC_LITERAL(7, 109, 5), // "index"
QT_MOC_LITERAL(8, 115, 20), // "handleButtonClickedo"
QT_MOC_LITERAL(9, 136, 16), // "QAbstractButton*"
QT_MOC_LITERAL(10, 153, 6), // "button"
QT_MOC_LITERAL(11, 160, 20), // "handleButtonClickedi"
QT_MOC_LITERAL(12, 181, 23) // "handleButtonClickedtype"

    },
    "musclepage\0plusbuttonsetting\0\0"
    "savebuttonsetting\0deletebuttonsetting\0"
    "newmusclebuttonsetting\0showmusclesetting\0"
    "index\0handleButtonClickedo\0QAbstractButton*\0"
    "button\0handleButtonClickedi\0"
    "handleButtonClickedtype"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_musclepage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    1,   58,    2, 0x08 /* Private */,
       8,    1,   61,    2, 0x08 /* Private */,
      11,    1,   64,    2, 0x08 /* Private */,
      12,    1,   67,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void musclepage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<musclepage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->plusbuttonsetting(); break;
        case 1: _t->savebuttonsetting(); break;
        case 2: _t->deletebuttonsetting(); break;
        case 3: _t->newmusclebuttonsetting(); break;
        case 4: _t->showmusclesetting((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->handleButtonClickedo((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 6: _t->handleButtonClickedi((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 7: _t->handleButtonClickedtype((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject musclepage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_musclepage.data,
    qt_meta_data_musclepage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *musclepage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *musclepage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_musclepage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int musclepage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
