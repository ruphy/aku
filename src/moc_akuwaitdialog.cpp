/****************************************************************************
** Meta object code from reading C++ file 'akuwaitdialog.h'
**
** Created: Fri Apr 11 15:34:50 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akuwaitdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akuwaitdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuWaitDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      22,   14,   14,   14, 0x0a,
      29,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_akuWaitDialog[] = {
    "akuWaitDialog\0\0quit()\0exec()\0moveCursor()\0"
};

const QMetaObject akuWaitDialog::staticMetaObject = {
    { &KProgressDialog::staticMetaObject, qt_meta_stringdata_akuWaitDialog,
      qt_meta_data_akuWaitDialog, 0 }
};

const QMetaObject *akuWaitDialog::metaObject() const
{
    return &staticMetaObject;
}

void *akuWaitDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuWaitDialog))
	return static_cast<void*>(const_cast< akuWaitDialog*>(this));
    return KProgressDialog::qt_metacast(_clname);
}

int akuWaitDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KProgressDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: quit(); break;
        case 1: exec(); break;
        case 2: moveCursor(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
