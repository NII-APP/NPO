#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:42:41
# then we go hard
#
#-------------------------------------------------

QT       += core opengl gui svg widgets

RC_ICONS += "resource/images/Bez_imeni-1.ico"

TARGET = ModalView

TEMPLATE = app

CONFIG += c++11

include(source/source.pri)

RESOURCES += \
    res.qrc

DISTFILES += \
    resource/css.css \
    resource/main.json \
#    readBDF.py \
