/****************************************************************************
** Meta object code from reading C++ file 'jointpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/qtwindow/page/jointpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'jointpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_jointpage_t {
    QByteArrayData data[12];
    char stringdata0[172];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_jointpage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_jointpage_t qt_meta_stringdata_jointpage = {
    {
QT_MOC_LITERAL(0, 0, 9), // "jointpage"
QT_MOC_LITERAL(1, 10, 11), // "savesetting"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 13), // "deletesetting"
QT_MOC_LITERAL(4, 37, 17), // "plusbuttonsetting"
QT_MOC_LITERAL(5, 55, 21), // "newjointbuttonsetting"
QT_MOC_LITERAL(6, 77, 16), // "showjointsetting"
QT_MOC_LITERAL(7, 94, 5), // "index"
QT_MOC_LITERAL(8, 100, 23), // "handleButtonClickedbody"
QT_MOC_LITERAL(9, 124, 16), // "QAbstractButton*"
QT_MOC_LITERAL(10, 141, 6), // "button"
QT_MOC_LITERAL(11, 148, 23) // "handleButtonClickedtype"

    },
    "jointpage\0savesetting\0\0deletesetting\0"
    "plusbuttonsetting\0newjointbuttonsetting\0"
    "showjointsetting\0index\0handleButtonClickedbody\0"
    "QAbstractButton*\0button\0handleButtonClickedtype"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_jointpage[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    1,   53,    2, 0x08 /* Private */,
       8,    1,   56,    2, 0x08 /* Private */,
      11,    1,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void, 0x80000000 | 9,   10,

       0        // eod
};

void jointpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<jointpage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->savesetting(); break;
        case 1: _t->deletesetting(); break;
        case 2: _t->plusbuttonsetting(); break;
        case 3: _t->newjointbuttonsetting(); break;
        case 4: _t->showjointsetting((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->handleButtonClickedbody((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 6: _t->handleButtonClickedtype((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
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
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject jointpage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_jointpage.data,
    qt_meta_data_jointpage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *jointpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *jointpage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_jointpage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int jointpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
