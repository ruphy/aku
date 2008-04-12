/****************************************************************************
** Meta object code from reading C++ file 'akutreewidget.h'
**
** Created: Fri Apr 11 15:34:50 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akutreewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akutreewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuTreeWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x09,
      30,   14,   14,   14, 0x09,
      62,   14,   14,   14, 0x09,
      78,   14,   14,   14, 0x09,
      99,   14,   14,   14, 0x09,
     116,   14,   14,   14, 0x09,
     134,   14,   14,   14, 0x09,
     149,   14,   14,   14, 0x09,
     170,   14,   14,   14, 0x09,
     189,   14,   14,   14, 0x09,
     203,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_akuTreeWidget[] = {
    "akuTreeWidget\0\0setupActions()\0"
    "headerCustomContextMenu(QPoint)\0"
    "showratio(bool)\0showattributes(bool)\0"
    "showmethod(bool)\0showversion(bool)\0"
    "showsize(bool)\0showpackedsize(bool)\0"
    "showmodified(bool)\0showcrc(bool)\0"
    "showmime(bool)\0"
};

const QMetaObject akuTreeWidget::staticMetaObject = {
    { &QTreeWidget::staticMetaObject, qt_meta_stringdata_akuTreeWidget,
      qt_meta_data_akuTreeWidget, 0 }
};

const QMetaObject *akuTreeWidget::metaObject() const
{
    return &staticMetaObject;
}

void *akuTreeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuTreeWidget))
	return static_cast<void*>(const_cast< akuTreeWidget*>(this));
    return QTreeWidget::qt_metacast(_clname);
}

int akuTreeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setupActions(); break;
        case 1: headerCustomContextMenu((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 2: showratio((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: showattributes((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: showmethod((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: showversion((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: showsize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: showpackedsize((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: showmodified((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: showcrc((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: showmime((*reinterpret_cast< bool(*)>(_a[1]))); break;
        }
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
