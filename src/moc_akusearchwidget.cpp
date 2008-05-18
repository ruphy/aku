/****************************************************************************
** Meta object code from reading C++ file 'akusearchwidget.h'
**
** Created: Sat May 17 20:42:41 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akusearchwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akusearchwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuSearchWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x09,
      26,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_akuSearchWidget[] = {
    "akuSearchWidget\0\0hideMe()\0"
    "handleCloseAction(bool)\0"
};

const QMetaObject akuSearchWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_akuSearchWidget,
      qt_meta_data_akuSearchWidget, 0 }
};

const QMetaObject *akuSearchWidget::metaObject() const
{
    return &staticMetaObject;
}

void *akuSearchWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuSearchWidget))
	return static_cast<void*>(const_cast< akuSearchWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int akuSearchWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: hideMe(); break;
        case 1: handleCloseAction((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
