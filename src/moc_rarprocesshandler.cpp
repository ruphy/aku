/****************************************************************************
** Meta object code from reading C++ file 'rarprocesshandler.h'
**
** Created: Fri Apr 11 19:38:53 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "rarprocesshandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rarprocesshandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_rarProcessHandler[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      19,   18,   18,   18, 0x05,
      42,   18,   18,   18, 0x05,
      60,   18,   18,   18, 0x05,

 // slots: signature, parameters, type, tag, flags
      81,   18,   18,   18, 0x0a,
      92,   18,   18,   18, 0x0a,
     107,   18,   18,   18, 0x0a,
     122,   18,   18,   18, 0x09,
     136,   18,   18,   18, 0x09,
     152,   18,   18,   18, 0x09,
     167,   18,   18,   18, 0x09,
     185,   18,   18,   18, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_rarProcessHandler[] = {
    "rarProcessHandler\0\0processCompleted(bool)\0"
    "processCanceled()\0outputReady(QString)\0"
    "getError()\0showProgress()\0handleCancel()\0"
    "initProcess()\0handleProcess()\0"
    "handlePaused()\0handleContinued()\0"
    "startSignals()\0"
};

const QMetaObject rarProcessHandler::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_rarProcessHandler,
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
    return QThread::qt_metacast(_clname);
}

int rarProcessHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: processCompleted((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: processCanceled(); break;
        case 2: outputReady((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: getError(); break;
        case 4: showProgress(); break;
        case 5: handleCancel(); break;
        case 6: initProcess(); break;
        case 7: handleProcess(); break;
        case 8: handlePaused(); break;
        case 9: handleContinued(); break;
        case 10: startSignals(); break;
        }
        _id -= 11;
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
void rarProcessHandler::outputReady(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
