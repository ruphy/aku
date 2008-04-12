/****************************************************************************
** Meta object code from reading C++ file 'akuprogressdialog.h'
**
** Created: Fri Apr 11 15:34:50 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akuprogressdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akuprogressdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuProgressDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      30,   18,   18,   18, 0x05,
      39,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   18,   18,   18, 0x0a,
      75,   18,   18,   18, 0x0a,
     103,   18,   18,   18, 0x0a,
     131,   18,   18,   18, 0x0a,
     159,   18,   18,   18, 0x0a,
     193,   18,   18,   18, 0x0a,
     202,   18,   18,   18, 0x0a,
     221,   18,   18,   18, 0x0a,
     236,   18,   18,   18, 0x0a,
     252,   18,   18,   18, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_akuProgressDialog[] = {
    "akuProgressDialog\0\0canceled()\0paused()\0"
    "continued()\0setArchiveName(QString)\0"
    "setCurrentFileName(QString)\0"
    "setCurrentFileSize(QString)\0"
    "setCurrentFileProgress(int)\0"
    "setCurrentFileProgressToMaximum()\0"
    "cancel()\0incrementOverall()\0pauseClicked()\0"
    "setMaximum(int)\0cancelPressed()\0"
};

const QMetaObject akuProgressDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_akuProgressDialog,
      qt_meta_data_akuProgressDialog, 0 }
};

const QMetaObject *akuProgressDialog::metaObject() const
{
    return &staticMetaObject;
}

void *akuProgressDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuProgressDialog))
	return static_cast<void*>(const_cast< akuProgressDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int akuProgressDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: canceled(); break;
        case 1: paused(); break;
        case 2: continued(); break;
        case 3: setArchiveName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: setCurrentFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: setCurrentFileSize((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: setCurrentFileProgress((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: setCurrentFileProgressToMaximum(); break;
        case 8: cancel(); break;
        case 9: incrementOverall(); break;
        case 10: pauseClicked(); break;
        case 11: setMaximum((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: cancelPressed(); break;
        }
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void akuProgressDialog::canceled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void akuProgressDialog::paused()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void akuProgressDialog::continued()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
QT_END_MOC_NAMESPACE
