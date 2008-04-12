/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Fri Apr 11 19:25:37 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0-snapshot-20080325)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0-snapshot-20080325. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      47,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      25,   11,   11,   11, 0x0a,
      37,   11,   11,   11, 0x0a,
      57,   11,   11,   11, 0x0a,
      73,   11,   11,   11, 0x0a,
      89,   11,   11,   11, 0x0a,
     106,   11,   11,   11, 0x0a,
     129,   11,   11,   11, 0x0a,
     150,   11,   11,   11, 0x0a,
     167,   11,   11,   11, 0x0a,
     206,   11,   11,   11, 0x0a,
     236,   11,   11,   11, 0x0a,
     249,   11,   11,   11, 0x0a,
     266,   11,   11,   11, 0x0a,
     277,   11,   11,   11, 0x0a,
     292,  290,   11,   11, 0x0a,
     324,   11,   11,   11, 0x0a,
     336,   11,   11,   11, 0x0a,
     358,   11,   11,   11, 0x0a,
     381,   11,   11,   11, 0x0a,
     395,   11,   11,   11, 0x0a,
     406,   11,   11,   11, 0x0a,
     431,   11,   11,   11, 0x0a,
     452,  290,   11,   11, 0x0a,
     486,   11,   11,   11, 0x0a,
     513,  509,   11,   11, 0x0a,
     529,   11,   11,   11, 0x2a,
     541,  509,   11,   11, 0x0a,
     555,   11,   11,   11, 0x2a,
     568,  565,   11,   11, 0x0a,
     588,   11,   11,   11, 0x2a,
     604,   11,   11,   11, 0x0a,
     624,   11,   11,   11, 0x0a,
     644,   11,   11,   11, 0x0a,
     666,   11,   11,   11, 0x09,
     680,   11,   11,   11, 0x09,
     693,   11,   11,   11, 0x09,
     721,   11,   11,   11, 0x09,
     738,   11,   11,   11, 0x09,
     757,   11,   11,   11, 0x09,
     778,   11,   11,   11, 0x09,
     799,   11,   11,   11, 0x09,
     816,   11,   11,   11, 0x09,
     829,   11,   11,   11, 0x09,
     844,   11,   11,   11, 0x09,
     858,   11,   11,   11, 0x09,
     879,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0openDialog()\0debugging()\0"
    "singleExtractInit()\0singleExtract()\0"
    "setPopUpMenus()\0setConnections()\0"
    "setSelectionInverted()\0setInformation(bool)\0"
    "setFolderIcons()\0"
    "recursiveFolderIcons(QTreeWidgetItem*)\0"
    "getMetaInfo(QTreeWidgetItem*)\0"
    "newArchive()\0embeddedViewer()\0find(bool)\0"
    "renameItem()\0,\0rarRename(QTreeWidgetItem*,int)\0"
    "rarDelete()\0expandTopLevelItems()\0"
    "sumSelectedItemsSize()\0setupForNew()\0"
    "closeRar()\0closeNewArchiveGUI(bool)\0"
    "openRecentFile(KUrl)\0"
    "openItemUrl(QTreeWidgetItem*,int)\0"
    "insertComment(QString)\0pwd\0addFolder(bool)\0"
    "addFolder()\0addFile(bool)\0addFile()\0"
    "ok\0reloadArchive(bool)\0reloadArchive()\0"
    "enableActions(bool)\0cantDeleteMessage()\0"
    "closeDeletedArchive()\0lockArchive()\0"
    "addComment()\0handleRestrictions(QString)\0"
    "setupStatusBar()\0setupCommentView()\0"
    "completeRename(bool)\0completeDelete(bool)\0"
    "encryptArchive()\0addFilePwd()\0"
    "addFolderPwd()\0checkRarExe()\0"
    "showStatusInfo(bool)\0parseAndShow(QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &KXmlGuiWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
	return static_cast<void*>(const_cast< MainWindow*>(this));
    return KXmlGuiWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KXmlGuiWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openDialog(); break;
        case 1: debugging(); break;
        case 2: singleExtractInit(); break;
        case 3: singleExtract(); break;
        case 4: setPopUpMenus(); break;
        case 5: setConnections(); break;
        case 6: setSelectionInverted(); break;
        case 7: setInformation((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: setFolderIcons(); break;
        case 9: recursiveFolderIcons((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 10: getMetaInfo((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 11: newArchive(); break;
        case 12: embeddedViewer(); break;
        case 13: find((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: renameItem(); break;
        case 15: rarRename((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 16: rarDelete(); break;
        case 17: expandTopLevelItems(); break;
        case 18: sumSelectedItemsSize(); break;
        case 19: setupForNew(); break;
        case 20: closeRar(); break;
        case 21: closeNewArchiveGUI((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: openRecentFile((*reinterpret_cast< KUrl(*)>(_a[1]))); break;
        case 23: openItemUrl((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 24: insertComment((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 25: addFolder((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: addFolder(); break;
        case 27: addFile((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 28: addFile(); break;
        case 29: reloadArchive((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 30: reloadArchive(); break;
        case 31: enableActions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: cantDeleteMessage(); break;
        case 33: closeDeletedArchive(); break;
        case 34: lockArchive(); break;
        case 35: addComment(); break;
        case 36: handleRestrictions((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 37: setupStatusBar(); break;
        case 38: setupCommentView(); break;
        case 39: completeRename((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 40: completeDelete((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: encryptArchive(); break;
        case 42: addFilePwd(); break;
        case 43: addFolderPwd(); break;
        case 44: checkRarExe(); break;
        case 45: showStatusInfo((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 46: parseAndShow((*reinterpret_cast< QString(*)>(_a[1]))); break;
        }
        _id -= 47;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
