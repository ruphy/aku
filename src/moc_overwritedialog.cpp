/****************************************************************************
** Meta object code from reading C++ file 'overwritedialog.h'
**
** Created: Thu May 15 23:58:19 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "overwritedialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'overwritedialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_overwriteDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,
      32,   16,   16,   16, 0x0a,
      47,   16,   16,   16, 0x0a,
      61,   16,   16,   16, 0x0a,
      79,   16,   16,   16, 0x0a,
      97,   16,   16,   16, 0x0a,
     131,  128,   16,   16, 0x0a,
     175,   16,   16,   16, 0x09,
     203,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_overwriteDialog[] = {
    "overwriteDialog\0\0renameAction()\0"
    "yesOverwrite()\0noOverwrite()\0"
    "cancelOverwrite()\0renameOverwrite()\0"
    "setDestinationDetails(QString)\0,,\0"
    "setSourceDetails(QString,KDateTime,QString)\0"
    "setDestinationIcon(QString)\0"
    "setSourceIcon(QString)\0"
};

const QMetaObject overwriteDialog::staticMetaObject = {
    { &KDialog::staticMetaObject, qt_meta_stringdata_overwriteDialog,
      qt_meta_data_overwriteDialog, 0 }
};

const QMetaObject *overwriteDialog::metaObject() const
{
    return &staticMetaObject;
}

void *overwriteDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_overwriteDialog))
	return static_cast<void*>(const_cast< overwriteDialog*>(this));
    return KDialog::qt_metacast(_clname);
}

int overwriteDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: renameAction(); break;
        case 1: yesOverwrite(); break;
        case 2: noOverwrite(); break;
        case 3: cancelOverwrite(); break;
        case 4: renameOverwrite(); break;
        case 5: setDestinationDetails((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: setSourceDetails((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< KDateTime(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: setDestinationIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: setSourceIcon((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
