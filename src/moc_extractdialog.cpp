/****************************************************************************
** Meta object code from reading C++ file 'extractdialog.h'
**
** Created: Thu May 15 23:58:18 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "extractdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'extractdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_extractDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      30,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      43,   14,   14,   14, 0x0a,
      58,   14,   14,   14, 0x0a,
      91,   14,   14,   14, 0x0a,
     109,   14,   14,   14, 0x0a,
     122,   14,   14,   14, 0x0a,
     136,   14,   14,   14, 0x0a,
     154,   14,   14,   14, 0x0a,
     169,   14,   14,   14, 0x0a,
     184,   14,   14,   14, 0x0a,
     202,   14,   14,   14, 0x0a,
     228,   14,   14,   14, 0x0a,
     248,   14,   14,   14, 0x0a,
     271,   14,   14,   14, 0x0a,
     289,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_extractDialog[] = {
    "extractDialog\0\0closeArchive()\0"
    "cantDelete()\0completerBox()\0"
    "updateTreeViewSelection(QString)\0"
    "hiddenFiles(bool)\0extraction()\0"
    "closeDialog()\0defaultSettings()\0"
    "loadSettings()\0saveSettings()\0"
    "updateCombo(KUrl)\0openDestinationPath(bool)\0"
    "deleteArchive(bool)\0deleteArchiveAsk(bool)\0"
    "createNewFolder()\0checkPath()\0"
};

const QMetaObject extractDialog::staticMetaObject = {
    { &KDialog::staticMetaObject, qt_meta_stringdata_extractDialog,
      qt_meta_data_extractDialog, 0 }
};

const QMetaObject *extractDialog::metaObject() const
{
    return &staticMetaObject;
}

void *extractDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_extractDialog))
	return static_cast<void*>(const_cast< extractDialog*>(this));
    return KDialog::qt_metacast(_clname);
}

int extractDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: closeArchive(); break;
        case 1: cantDelete(); break;
        case 2: completerBox(); break;
        case 3: updateTreeViewSelection((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: hiddenFiles((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: extraction(); break;
        case 6: closeDialog(); break;
        case 7: defaultSettings(); break;
        case 8: loadSettings(); break;
        case 9: saveSettings(); break;
        case 10: updateCombo((*reinterpret_cast< KUrl(*)>(_a[1]))); break;
        case 11: openDestinationPath((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: deleteArchive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: deleteArchiveAsk((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: createNewFolder(); break;
        case 15: checkPath(); break;
        }
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void extractDialog::closeArchive()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void extractDialog::cantDelete()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
