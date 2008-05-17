/****************************************************************************
** Meta object code from reading C++ file 'akumetawidget.h'
**
** Created: Thu May 15 23:58:18 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akumetawidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akumetawidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuMetaWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x0a,
      44,   36,   14,   14, 0x0a,
      70,   14,   14,   14, 0x2a,
      91,   14,   14,   14, 0x0a,
     112,   14,   14,   14, 0x0a,
     128,   14,   14,   14, 0x0a,
     136,   14,   14,   14, 0x0a,
     153,   14,   14,   14, 0x0a,
     176,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_akuMetaWidget[] = {
    "akuMetaWidget\0\0setMimeIcon(QPixmap)\0"
    ",folder\0setFileName(QString,bool)\0"
    "setFileName(QString)\0setFileSize(QString)\0"
    "setRatio(float)\0clear()\0setMime(QString)\0"
    "setPreview(QByteArray)\0"
    "handleItemSelections(QList<QTreeWidgetItem*>)\0"
};

const QMetaObject akuMetaWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_akuMetaWidget,
      qt_meta_data_akuMetaWidget, 0 }
};

const QMetaObject *akuMetaWidget::metaObject() const
{
    return &staticMetaObject;
}

void *akuMetaWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuMetaWidget))
	return static_cast<void*>(const_cast< akuMetaWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int akuMetaWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setMimeIcon((*reinterpret_cast< QPixmap(*)>(_a[1]))); break;
        case 1: setFileName((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: setFileName((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: setFileSize((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: setRatio((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 5: clear(); break;
        case 6: setMime((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: setPreview((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 8: handleItemSelections((*reinterpret_cast< QList<QTreeWidgetItem*>(*)>(_a[1]))); break;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
