/****************************************************************************
** Meta object code from reading C++ file 'akuviewer.h'
**
** Created: Thu May 15 23:58:18 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akuviewer.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akuviewer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuViewer[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x0a,
      28,   10,   10,   10, 0x0a,
      47,   10,   10,   10, 0x0a,
      68,   10,   10,   10, 0x0a,
      75,   10,   10,   10, 0x0a,
      92,   10,   10,   10, 0x0a,
     108,   10,   10,   10, 0x0a,
     128,   10,   10,   10, 0x0a,
     149,   10,   10,   10, 0x0a,
     165,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_akuViewer[] = {
    "akuViewer\0\0setText(QString)\0"
    "setTextFont(QFont)\0setTextSize(QString)\0"
    "find()\0searchBackward()\0searchForward()\0"
    "startTimer(QString)\0setFileName(QString)\0"
    "setupTextView()\0setData(QByteArray)\0"
};

const QMetaObject akuViewer::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_akuViewer,
      qt_meta_data_akuViewer, 0 }
};

const QMetaObject *akuViewer::metaObject() const
{
    return &staticMetaObject;
}

void *akuViewer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuViewer))
	return static_cast<void*>(const_cast< akuViewer*>(this));
    return QDialog::qt_metacast(_clname);
}

int akuViewer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setText((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: setTextFont((*reinterpret_cast< QFont(*)>(_a[1]))); break;
        case 2: setTextSize((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: find(); break;
        case 4: searchBackward(); break;
        case 5: searchForward(); break;
        case 6: startTimer((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: setFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: setupTextView(); break;
        case 9: setData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        }
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
