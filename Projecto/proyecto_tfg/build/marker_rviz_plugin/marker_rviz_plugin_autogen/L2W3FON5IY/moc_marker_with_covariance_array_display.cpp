/****************************************************************************
** Meta object code from reading C++ file 'marker_with_covariance_array_display.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/marker_rviz_plugin/include/marker_with_covariance_array/marker_with_covariance_array_display.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'marker_with_covariance_array_display.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay_t {
    QByteArrayData data[3];
    char stringdata0[67];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay_t qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay = {
    {
QT_MOC_LITERAL(0, 0, 52), // "marker_rviz_plugin::MarkerWit..."
QT_MOC_LITERAL(1, 53, 12), // "updateVisual"
QT_MOC_LITERAL(2, 66, 0) // ""

    },
    "marker_rviz_plugin::MarkerWithCovarianceArrayDisplay\0"
    "updateVisual\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void marker_rviz_plugin::MarkerWithCovarianceArrayDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MarkerWithCovarianceArrayDisplay *_t = static_cast<MarkerWithCovarianceArrayDisplay *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updateVisual(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject marker_rviz_plugin::MarkerWithCovarianceArrayDisplay::staticMetaObject = {
    { &rviz::MessageFilterDisplay<marker_msgs::MarkerWithCovarianceArray>::staticMetaObject, qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay.data,
      qt_meta_data_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *marker_rviz_plugin::MarkerWithCovarianceArrayDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *marker_rviz_plugin::MarkerWithCovarianceArrayDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_marker_rviz_plugin__MarkerWithCovarianceArrayDisplay.stringdata0))
        return static_cast<void*>(this);
    return rviz::MessageFilterDisplay<marker_msgs::MarkerWithCovarianceArray>::qt_metacast(_clname);
}

int marker_rviz_plugin::MarkerWithCovarianceArrayDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = rviz::MessageFilterDisplay<marker_msgs::MarkerWithCovarianceArray>::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
