/****************************************************************************
** Meta object code from reading C++ file 'qplot.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Plotting/qplot.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_qplot_t {
    QByteArrayData data[8];
    char stringdata0[68];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_qplot_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_qplot_t qt_meta_stringdata_qplot = {
    {
QT_MOC_LITERAL(0, 0, 5), // "qplot"
QT_MOC_LITERAL(1, 6, 10), // "Mouse_Down"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 8), // "Mouse_Up"
QT_MOC_LITERAL(4, 27, 9), // "Mouse_Pos"
QT_MOC_LITERAL(5, 37, 10), // "Mouse_Left"
QT_MOC_LITERAL(6, 48, 12), // "Update_Image"
QT_MOC_LITERAL(7, 61, 6) // "qplot*"

    },
    "qplot\0Mouse_Down\0\0Mouse_Up\0Mouse_Pos\0"
    "Mouse_Left\0Update_Image\0qplot*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_qplot[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x06 /* Public */,
       3,    0,   40,    2, 0x06 /* Public */,
       4,    0,   41,    2, 0x06 /* Public */,
       5,    0,   42,    2, 0x06 /* Public */,
       6,    1,   43,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    2,

       0        // eod
};

void qplot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        qplot *_t = static_cast<qplot *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Mouse_Down(); break;
        case 1: _t->Mouse_Up(); break;
        case 2: _t->Mouse_Pos(); break;
        case 3: _t->Mouse_Left(); break;
        case 4: _t->Update_Image((*reinterpret_cast< qplot*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< qplot* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (qplot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qplot::Mouse_Down)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (qplot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qplot::Mouse_Up)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (qplot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qplot::Mouse_Pos)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (qplot::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qplot::Mouse_Left)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (qplot::*_t)(qplot * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&qplot::Update_Image)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject qplot::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_qplot.data,
      qt_meta_data_qplot,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *qplot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *qplot::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_qplot.stringdata0))
        return static_cast<void*>(this);
    return QLabel::qt_metacast(_clname);
}

int qplot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void qplot::Mouse_Down()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void qplot::Mouse_Up()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void qplot::Mouse_Pos()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void qplot::Mouse_Left()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void qplot::Update_Image(qplot * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
