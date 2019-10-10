#-------------------------------------------------
#
# Project created by QtCreator 2019-09-26T16:50:35
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ElectricityManager
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


SOURCES += \
        main.cpp \
    TerminalGraphicsWidget.cpp \
    DatabaseProxy.cpp \
    Elec.cpp \
    BasicTreeWidgetItem.cpp \
    CenterWidget.cpp \
    OrganizationTreeWidgetItem.cpp \
    FarsiLoginDlg.cpp \
    BasicEditDlg.cpp \
    ConcentratorEditDlg.cpp \
    LineEditDlg.cpp \
    AddTerminalDlg.cpp \
    ModifyTerminalDlg.cpp \
    SummaryWidget.cpp \
    DatabaseConfigDlg.cpp \
    WorkerConfigDlg.cpp \
    UserConfigDlg.cpp \
    ServerConfigDlg.cpp \
    PopupInformation.cpp \
    ConcentratorConfigDlg.cpp

HEADERS += \
    TerminalGraphicsWidget.h \
    DatabaseProxy.h \
    Elec.h \
    BasicTreeWidgetItem.h \
    CenterWidget.h \
    OrganizationTreeWidgetItem.h \
    FarsiLoginDlg.h \
    BasicEditDlg.h \
    ConcentratorEditDlg.h \
    LineEditDlg.h \
    AddTerminalDlg.h \
    ModifyTerminalDlg.h \
    SummaryWidget.h \
    DatabaseConfigDlg.h \
    WorkerConfigDlg.h \
    UserConfigDlg.h \
    ServerConfigDlg.h \
    PopupInformation.h \
    ConcentratorConfigDlg.h

FORMS += \
    Elec.ui \
    CenterWidget.ui \
    FarsiLoginDlg.ui \
    BasicEditDlg.ui \
    ConcentratorEditDlg.ui \
    LineEditDlg.ui \
    AddTerminalDlg.ui \
    ModifyTerminalDlg.ui \
    SummaryWidget.ui \
    DatabaseConfigDlg.ui \
    WorkerConfigDlg.ui \
    UserConfigDlg.ui \
    ServerConfigDlg.ui \
    PopupInformation.ui \
    ConcentratorConfigDlg.ui

RESOURCES += \
    resource.qrc
