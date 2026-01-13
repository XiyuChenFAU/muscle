/****************************************************************************
** Meta object code from reading C++ file 'solvesettingpage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/qtwindow/page/solvesettingpage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'solvesettingpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_solvesettingpage_t {
    QByteArrayData data[13];
    char stringdata0[256];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_solvesettingpage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_solvesettingpage_t qt_meta_stringdata_solvesettingpage = {
    {
QT_MOC_LITERAL(0, 0, 16), // "solvesettingpage"
QT_MOC_LITERAL(1, 17, 11), // "savesetting"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 16), // "openFolderDialog"
QT_MOC_LITERAL(4, 47, 19), // "handleButtonClicked"
QT_MOC_LITERAL(5, 67, 16), // "QAbstractButton*"
QT_MOC_LITERAL(6, 84, 6), // "button"
QT_MOC_LITERAL(7, 91, 24), // "handleButtonClicked_mode"
QT_MOC_LITERAL(8, 116, 24), // "handleButtonClicked_body"
QT_MOC_LITERAL(9, 141, 31), // "handleCheckBoxChanged_collision"
QT_MOC_LITERAL(10, 173, 5), // "state"
QT_MOC_LITERAL(11, 179, 34), // "handleCheckBoxChanged_phi_eta..."
QT_MOC_LITERAL(12, 214, 41) // "handleCheckBoxChanged_all_mus..."

    },
    "solvesettingpage\0savesetting\0\0"
    "openFolderDialog\0handleButtonClicked\0"
    "QAbstractButton*\0button\0"
    "handleButtonClicked_mode\0"
    "handleButtonClicked_body\0"
    "handleCheckBoxChanged_collision\0state\0"
    "handleCheckBoxChanged_phi_eta_plus\0"
    "handleCheckBoxChanged_all_muscle_together"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_solvesettingpage[] = {

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
       4,    1,   56,    2, 0x08 /* Private */,
       7,    1,   59,    2, 0x08 /* Private */,
       8,    1,   62,    2, 0x08 /* Private */,
       9,    1,   65,    2, 0x08 /* Private */,
      11,    1,   68,    2, 0x08 /* Private */,
      12,    1,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void, QMetaType::Int,   10,

       0        // eod
};

void solvesettingpage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<solvesettingpage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->savesetting(); break;
        case 1: _t->openFolderDialog(); break;
        case 2: _t->handleButtonClicked((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 3: _t->handleButtonClicked_mode((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 4: _t->handleButtonClicked_body((*reinterpret_cast< QAbstractButton*(*)>(_a[1]))); break;
        case 5: _t->handleCheckBoxChanged_collision((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->handleCheckBoxChanged_phi_eta_plus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->handleCheckBoxChanged_all_muscle_together((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractButton* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject solvesettingpage::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_solvesettingpage.data,
    qt_meta_data_solvesettingpage,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *solvesettingpage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *solvesettingpage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_solvesettingpage.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int solvesettingpage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
