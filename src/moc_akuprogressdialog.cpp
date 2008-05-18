/****************************************************************************
** Meta object code from reading C++ file 'akuprogressdialog.h'
**
** Created: Sat May 17 20:42:42 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akuprogressdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akuprogressdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuProgressDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   10, // methods
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
     268,   18,   18,   18, 0x09,
     281,   18,   18,   18, 0x09,
     331,   18,   18,   18, 0x09,

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
    "sendToTray()\0"
    "showInfoOnTray(QSystemTrayIcon::ActivationReason)\0"
    "updateTooltip(int)\0"
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
        case 13: sendToTray(); break;
        case 14: showInfoOnTray((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 15: updateTooltip((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 16;
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
static const uint qt_meta_data_helpFilter[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_helpFilter[] = {
    "helpFilter\0\0hideTooltip()\0"
};

const QMetaObject helpFilter::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_helpFilter,
      qt_meta_data_helpFilter, 0 }
};

const QMetaObject *helpFilter::metaObject() const
{
    return &staticMetaObject;
}

void *helpFilter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_helpFilter))
	return static_cast<void*>(const_cast< helpFilter*>(this));
    return QObject::qt_metacast(_clname);
}

int helpFilter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: hideTooltip(); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
