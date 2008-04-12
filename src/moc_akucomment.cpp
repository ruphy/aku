/****************************************************************************
** Meta object code from reading C++ file 'akucomment.h'
**
** Created: Fri Apr 11 15:34:50 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akucomment.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akucomment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuComment[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   11,   11,   11, 0x0a,
      46,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_akuComment[] = {
    "akuComment\0\0okClicked(QString)\0"
    "writeComment()\0loadFromFile()\0"
};

const QMetaObject akuComment::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_akuComment,
      qt_meta_data_akuComment, 0 }
};

const QMetaObject *akuComment::metaObject() const
{
    return &staticMetaObject;
}

void *akuComment::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuComment))
	return static_cast<void*>(const_cast< akuComment*>(this));
    return QDialog::qt_metacast(_clname);
}

int akuComment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: okClicked((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: writeComment(); break;
        case 2: loadFromFile(); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void akuComment::okClicked(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
