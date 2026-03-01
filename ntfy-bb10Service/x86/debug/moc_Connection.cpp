/****************************************************************************
** Meta object code from reading C++ file 'Connection.hpp'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../src/msg/Connection.hpp"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Connection.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_msg__Connection[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      31,   16,   16,   16, 0x08,
      49,   16,   16,   16, 0x08,
      93,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_msg__Connection[] = {
    "msg::Connection\0\0onReadyRead()\0"
    "onReplyFinished()\0"
    "onNetworkError(QNetworkReply::NetworkError)\0"
    "onSslErrors(QList<QSslError>)\0"
};

void msg::Connection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Connection *_t = static_cast<Connection *>(_o);
        switch (_id) {
        case 0: _t->onReadyRead(); break;
        case 1: _t->onReplyFinished(); break;
        case 2: _t->onNetworkError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 3: _t->onSslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData msg::Connection::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject msg::Connection::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_msg__Connection,
      qt_meta_data_msg__Connection, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &msg::Connection::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *msg::Connection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *msg::Connection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_msg__Connection))
        return static_cast<void*>(const_cast< Connection*>(this));
    return QObject::qt_metacast(_clname);
}

int msg::Connection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
