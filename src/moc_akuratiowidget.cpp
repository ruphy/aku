/****************************************************************************
** Meta object code from reading C++ file 'akuratiowidget.h'
**
** Created: Fri Apr 11 15:34:48 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akuratiowidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akuratiowidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuRatioWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_akuRatioWidget[] = {
    "akuRatioWidget\0\0setRatio(float)\0"
};

const QMetaObject akuRatioWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_akuRatioWidget,
      qt_meta_data_akuRatioWidget, 0 }
};

const QMetaObject *akuRatioWidget::metaObject() const
{
    return &staticMetaObject;
}

void *akuRatioWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuRatioWidget))
	return static_cast<void*>(const_cast< akuRatioWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int akuRatioWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setRatio((*reinterpret_cast< float(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
