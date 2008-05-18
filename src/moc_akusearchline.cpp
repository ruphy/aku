/****************************************************************************
** Meta object code from reading C++ file 'akusearchline.h'
**
** Created: Sat May 17 20:42:42 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akusearchline.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akusearchline.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuSearchLine[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      31,   14,   14,   14, 0x0a,
      50,   14,   14,   14, 0x0a,
      77,   14,   14,   14, 0x0a,
     104,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_akuSearchLine[] = {
    "akuSearchLine\0\0filter(QString)\0"
    "expandFoundItems()\0setFilterBySelection(bool)\0"
    "setFilterByExclusion(bool)\0"
    "setFilterCaseSensitivity(bool)\0"
};

const QMetaObject akuSearchLine::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_akuSearchLine,
      qt_meta_data_akuSearchLine, 0 }
};

const QMetaObject *akuSearchLine::metaObject() const
{
    return &staticMetaObject;
}

void *akuSearchLine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuSearchLine))
	return static_cast<void*>(const_cast< akuSearchLine*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int akuSearchLine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: filter((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: expandFoundItems(); break;
        case 2: setFilterBySelection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: setFilterByExclusion((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: setFilterCaseSensitivity((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
