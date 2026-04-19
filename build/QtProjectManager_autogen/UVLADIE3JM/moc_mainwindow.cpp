/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[259];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 14), // "updateLanguage"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 17), // "showWelcomeScreen"
QT_MOC_LITERAL(4, 45, 26), // "showProjectSelectionScreen"
QT_MOC_LITERAL(5, 72, 13), // "createProject"
QT_MOC_LITERAL(6, 86, 12), // "templateName"
QT_MOC_LITERAL(7, 99, 6), // "goBack"
QT_MOC_LITERAL(8, 106, 14), // "changeLanguage"
QT_MOC_LITERAL(9, 121, 12), // "showSettings"
QT_MOC_LITERAL(10, 134, 7), // "newFile"
QT_MOC_LITERAL(11, 142, 8), // "openFile"
QT_MOC_LITERAL(12, 151, 8), // "saveFile"
QT_MOC_LITERAL(13, 160, 10), // "saveFileAs"
QT_MOC_LITERAL(14, 171, 9), // "closeFile"
QT_MOC_LITERAL(15, 181, 9), // "showAbout"
QT_MOC_LITERAL(16, 191, 19), // "applyEditorSettings"
QT_MOC_LITERAL(17, 211, 13), // "onFileClicked"
QT_MOC_LITERAL(18, 225, 11), // "QModelIndex"
QT_MOC_LITERAL(19, 237, 5), // "index"
QT_MOC_LITERAL(20, 243, 15) // "openCurrentFile"

    },
    "MainWindow\0updateLanguage\0\0showWelcomeScreen\0"
    "showProjectSelectionScreen\0createProject\0"
    "templateName\0goBack\0changeLanguage\0"
    "showSettings\0newFile\0openFile\0saveFile\0"
    "saveFileAs\0closeFile\0showAbout\0"
    "applyEditorSettings\0onFileClicked\0"
    "QModelIndex\0index\0openCurrentFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x0a /* Public */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    1,   97,    2, 0x08 /* Private */,
       7,    0,  100,    2, 0x08 /* Private */,
       8,    0,  101,    2, 0x08 /* Private */,
       9,    0,  102,    2, 0x08 /* Private */,
      10,    0,  103,    2, 0x08 /* Private */,
      11,    0,  104,    2, 0x08 /* Private */,
      12,    0,  105,    2, 0x08 /* Private */,
      13,    0,  106,    2, 0x08 /* Private */,
      14,    0,  107,    2, 0x08 /* Private */,
      15,    0,  108,    2, 0x08 /* Private */,
      16,    0,  109,    2, 0x08 /* Private */,
      17,    1,  110,    2, 0x08 /* Private */,
      20,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateLanguage(); break;
        case 1: _t->showWelcomeScreen(); break;
        case 2: _t->showProjectSelectionScreen(); break;
        case 3: _t->createProject((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->goBack(); break;
        case 5: _t->changeLanguage(); break;
        case 6: _t->showSettings(); break;
        case 7: _t->newFile(); break;
        case 8: _t->openFile(); break;
        case 9: _t->saveFile(); break;
        case 10: _t->saveFileAs(); break;
        case 11: _t->closeFile(); break;
        case 12: _t->showAbout(); break;
        case 13: _t->applyEditorSettings(); break;
        case 14: _t->onFileClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 15: _t->openCurrentFile(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
