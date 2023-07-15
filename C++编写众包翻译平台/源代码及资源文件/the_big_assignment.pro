#-------------------------------------------------
#
# Project created by QtCreator 2019-07-24T12:49:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = the_big_assignment
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += sql
CONFIG += c++11

SOURCES += \
        admin_view_cer.cpp \
        administrator.cpp \
        assignment.cpp \
        assignment_ui.cpp \
        database.cpp \
        dlg_allco_c.cpp \
        dlg_c_view.cpp \
        dlg_delay_ddl.cpp \
        dlg_r_alllco.cpp \
        dlg_register.cpp \
        dlg_system_message.cpp \
        dlg_update.cpp \
        dlg_user_task_ui.cpp \
        dlglogin.cpp \
        main.cpp \
        message.cpp \
        message_ui.cpp \
        realease_task.cpp \
        user.cpp \
        user_ui.cpp \
        user_ui_task.cpp \
        widget.cpp

HEADERS += \
        admin_view_cer.h \
        administrator.h \
        assignment.h \
        assignment_ui.h \
        database.h \
        dlg_allco_c.h \
        dlg_c_view.h \
        dlg_delay_ddl.h \
        dlg_r_alllco.h \
        dlg_register.h \
        dlg_system_message.h \
        dlg_update.h \
        dlg_user_task_ui.h \
        dlglogin.h \
        message.h \
        message_ui.h \
        realease_task.h \
        user.h \
        user_ui.h \
        user_ui_task.h \
        widget.h

FORMS += \
        admin_view_cer.ui \
        administrator.ui \
        assignment_ui.ui \
        dlg_allco_c.ui \
        dlg_c_view.ui \
        dlg_delay_ddl.ui \
        dlg_r_alllco.ui \
        dlg_register.ui \
        dlg_system_message.ui \
        dlg_update.ui \
        dlg_user_task_ui.ui \
        dlglogin.ui \
        message_ui.ui \
        realease_task.ui \
        user_ui.ui \
        user_ui_task.ui \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc

DISTFILES +=
