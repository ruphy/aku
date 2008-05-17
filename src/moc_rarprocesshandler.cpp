/****************************************************************************
** Meta object code from reading C++ file 'rarprocesshandler.h'
**
** Created: Thu May 15 23:58:19 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rarprocesshandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rarprocesshandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_rarProcessHandler[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      42,   18,   18,   18, 0x05,
      62,   60,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      88,   18,   18,   18, 0x0a,
      99,   18,   18,   18, 0x0a,
     114,   18,   18,   18, 0x0a,
     129,   18,   18,   18, 0x0a,
     137,   18,   18,   18, 0x09,
     151,   18,   18,   18, 0x09,
     167,   18,   18,   18, 0x09,
     182,   18,   18,   18, 0x09,
     200,   18,   18,   18, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_rarProcessHandler[] = {
    "rarProcessHandler\0\0processCompleted(bool)\0"
    "processCanceled()\0,\0outputReady(QString,bool)\0"
    "getError()\0showProgress()\0handleCancel()\0"
    "start()\0initProcess()\0handleProcess()\0"
    "handlePaused()\0handleContinued()\0"
    "startSignals()\0"
};

const QMetaObject rarProcessHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_rarProcessHandler,
      qt_meta_data_rarProcessHandler, 0 }
};

const QMetaObject *rarProcessHandler::metaObject() const
{
    return &staticMetaObject;
}

void *rarProcessHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_rarProcessHandler))
	return static_cast<void*>(const_cast< rarProcessHandler*>(this));
    return QObject::qt_metacast(_clname);
}

int rarProcessHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: processCompleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: processCanceled(); break;
        case 2: outputReady((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: getError(); break;
        case 4: showProgress(); break;
        case 5: handleCancel(); break;
        case 6: start(); break;
        case 7: initProcess(); break;
        case 8: handleProcess(); break;
        case 9: handlePaused(); break;
        case 10: handleContinued(); break;
        case 11: startSignals(); break;
        }
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void rarProcessHandler::processCompleted(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void rarProcessHandler::processCanceled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void rarProcessHandler::outputReady(QString _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
