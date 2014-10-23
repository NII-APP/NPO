/****************************************************************************
** Meta object code from reading C++ file 'geometrywidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../NPO/geometrywidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'geometrywidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GeometryWidget_t {
    QByteArrayData data[17];
    char stringdata[210];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GeometryWidget_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GeometryWidget_t qt_meta_stringdata_GeometryWidget = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 19),
QT_MOC_LITERAL(2, 35, 17),
QT_MOC_LITERAL(3, 53, 0),
QT_MOC_LITERAL(4, 54, 12),
QT_MOC_LITERAL(5, 67, 1),
QT_MOC_LITERAL(6, 69, 12),
QT_MOC_LITERAL(7, 82, 13),
QT_MOC_LITERAL(8, 96, 13),
QT_MOC_LITERAL(9, 110, 12),
QT_MOC_LITERAL(10, 123, 12),
QT_MOC_LITERAL(11, 136, 13),
QT_MOC_LITERAL(12, 150, 13),
QT_MOC_LITERAL(13, 164, 16),
QT_MOC_LITERAL(14, 181, 8),
QT_MOC_LITERAL(15, 190, 8),
QT_MOC_LITERAL(16, 199, 10)
    },
    "GeometryWidget\0getAnimationOptions\0"
    "AnimationOptions*\0\0setMagnitude\0v\0"
    "getMagnitude\0magnitudeIncr\0magnitudeDecr\0"
    "setFrequency\0getFrequency\0frequencyIncr\0"
    "frequencyDecr\0initialAnimation\0formIncr\0"
    "formDecr\0netTrigger"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GeometryWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    3, 0x0a /* Public */,
       4,    1,   80,    3, 0x0a /* Public */,
       6,    0,   83,    3, 0x0a /* Public */,
       7,    0,   84,    3, 0x0a /* Public */,
       8,    0,   85,    3, 0x0a /* Public */,
       9,    1,   86,    3, 0x0a /* Public */,
      10,    0,   89,    3, 0x0a /* Public */,
      11,    0,   90,    3, 0x0a /* Public */,
      12,    0,   91,    3, 0x0a /* Public */,
      13,    0,   92,    3, 0x0a /* Public */,
      14,    0,   93,    3, 0x0a /* Public */,
      15,    0,   94,    3, 0x0a /* Public */,
      16,    0,   95,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Float,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,    5,
    QMetaType::Float,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GeometryWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GeometryWidget *_t = static_cast<GeometryWidget *>(_o);
        switch (_id) {
        case 0: { AnimationOptions* _r = _t->getAnimationOptions();
            if (_a[0]) *reinterpret_cast< AnimationOptions**>(_a[0]) = _r; }  break;
        case 1: _t->setMagnitude((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: { float _r = _t->getMagnitude();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 3: _t->magnitudeIncr(); break;
        case 4: _t->magnitudeDecr(); break;
        case 5: _t->setFrequency((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: { float _r = _t->getFrequency();
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 7: _t->frequencyIncr(); break;
        case 8: _t->frequencyDecr(); break;
        case 9: _t->initialAnimation(); break;
        case 10: _t->formIncr(); break;
        case 11: _t->formDecr(); break;
        case 12: _t->netTrigger(); break;
        default: ;
        }
    }
}

const QMetaObject GeometryWidget::staticMetaObject = {
    { &CGLWidget::staticMetaObject, qt_meta_stringdata_GeometryWidget.data,
      qt_meta_data_GeometryWidget,  qt_static_metacall, 0, 0}
};


const QMetaObject *GeometryWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GeometryWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GeometryWidget.stringdata))
        return static_cast<void*>(const_cast< GeometryWidget*>(this));
    return CGLWidget::qt_metacast(_clname);
}

int GeometryWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
