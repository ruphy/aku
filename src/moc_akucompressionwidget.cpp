/****************************************************************************
** Meta object code from reading C++ file 'akucompressionwidget.h'
**
** Created: Fri Apr 11 15:34:50 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "akucompressionwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'akucompressionwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_akuCompressionWidget[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      22,   21,   21,   21, 0x05,
      39,   21,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   21,   21,   21, 0x09,
      67,   21,   21,   21, 0x09,
      85,   21,   21,   21, 0x09,
     108,   21,   21,   21, 0x09,
     117,   21,   21,   21, 0x09,
     137,   21,   21,   21, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_akuCompressionWidget[] = {
    "akuCompressionWidget\0\0creationCalled()\0"
    "canceled()\0enablePass(bool)\0"
    "enableSplit(bool)\0enableNamescheme(bool)\0"
    "create()\0updateTextEdit(int)\0"
    "handleSplitEditing(int)\0"
};

const QMetaObject akuCompressionWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_akuCompressionWidget,
      qt_meta_data_akuCompressionWidget, 0 }
};

const QMetaObject *akuCompressionWidget::metaObject() const
{
    return &staticMetaObject;
}

void *akuCompressionWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_akuCompressionWidget))
	return static_cast<void*>(const_cast< akuCompressionWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int akuCompressionWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: creationCalled(); break;
        case 1: canceled(); break;
        case 2: enablePass((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: enableSplit((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: enableNamescheme((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: create(); break;
        case 6: updateTextEdit((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: handleSplitEditing((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void akuCompressionWidget::creationCalled()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void akuCompressionWidget::canceled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
