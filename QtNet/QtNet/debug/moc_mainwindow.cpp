/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[21];
    char stringdata0[348];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 8), // "ReadData"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 9), // "ReadError"
QT_MOC_LITERAL(4, 31, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(5, 60, 17), // "NewConnectionSlot"
QT_MOC_LITERAL(6, 78, 16), // "disconnectedSlot"
QT_MOC_LITERAL(7, 95, 14), // "ServerReadData"
QT_MOC_LITERAL(8, 110, 18), // "on_BtnConn_clicked"
QT_MOC_LITERAL(9, 129, 23), // "on_BtnClearRecv_clicked"
QT_MOC_LITERAL(10, 153, 18), // "on_BtnSend_clicked"
QT_MOC_LITERAL(11, 172, 22), // "on_radioClient_clicked"
QT_MOC_LITERAL(12, 195, 22), // "on_radioServer_clicked"
QT_MOC_LITERAL(13, 218, 18), // "on_btnOpen_clicked"
QT_MOC_LITERAL(14, 237, 18), // "on_btnSend_clicked"
QT_MOC_LITERAL(15, 256, 9), // "Read_Data"
QT_MOC_LITERAL(16, 266, 19), // "on_btnClear_clicked"
QT_MOC_LITERAL(17, 286, 27), // "on_textRecv_2_copyAvailable"
QT_MOC_LITERAL(18, 314, 1), // "b"
QT_MOC_LITERAL(19, 316, 15), // "on_cmd1_clicked"
QT_MOC_LITERAL(20, 332, 15) // "on_cmd2_clicked"

    },
    "MainWindow\0ReadData\0\0ReadError\0"
    "QAbstractSocket::SocketError\0"
    "NewConnectionSlot\0disconnectedSlot\0"
    "ServerReadData\0on_BtnConn_clicked\0"
    "on_BtnClearRecv_clicked\0on_BtnSend_clicked\0"
    "on_radioClient_clicked\0on_radioServer_clicked\0"
    "on_btnOpen_clicked\0on_btnSend_clicked\0"
    "Read_Data\0on_btnClear_clicked\0"
    "on_textRecv_2_copyAvailable\0b\0"
    "on_cmd1_clicked\0on_cmd2_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    1,  100,    2, 0x08 /* Private */,
       5,    0,  103,    2, 0x08 /* Private */,
       6,    0,  104,    2, 0x08 /* Private */,
       7,    0,  105,    2, 0x08 /* Private */,
       8,    0,  106,    2, 0x08 /* Private */,
       9,    0,  107,    2, 0x08 /* Private */,
      10,    0,  108,    2, 0x08 /* Private */,
      11,    0,  109,    2, 0x08 /* Private */,
      12,    0,  110,    2, 0x08 /* Private */,
      13,    0,  111,    2, 0x08 /* Private */,
      14,    0,  112,    2, 0x08 /* Private */,
      15,    0,  113,    2, 0x08 /* Private */,
      16,    0,  114,    2, 0x08 /* Private */,
      17,    1,  115,    2, 0x08 /* Private */,
      19,    0,  118,    2, 0x08 /* Private */,
      20,    0,  119,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
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
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   18,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ReadData(); break;
        case 1: _t->ReadError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 2: _t->NewConnectionSlot(); break;
        case 3: _t->disconnectedSlot(); break;
        case 4: _t->ServerReadData(); break;
        case 5: _t->on_BtnConn_clicked(); break;
        case 6: _t->on_BtnClearRecv_clicked(); break;
        case 7: _t->on_BtnSend_clicked(); break;
        case 8: _t->on_radioClient_clicked(); break;
        case 9: _t->on_radioServer_clicked(); break;
        case 10: _t->on_btnOpen_clicked(); break;
        case 11: _t->on_btnSend_clicked(); break;
        case 12: _t->Read_Data(); break;
        case 13: _t->on_btnClear_clicked(); break;
        case 14: _t->on_textRecv_2_copyAvailable((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->on_cmd1_clicked(); break;
        case 16: _t->on_cmd2_clicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
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
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
