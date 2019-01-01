#-------------------------------------------------
#
# Project created by QtCreator 2018-07-22T14:08:19
#
#-------------------------------------------------

QT       += core gui webenginewidgets webchannel sql

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTreeNote
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
        MainWindow.cpp \
    NoteButton.cpp \
    NoteLineEdit.cpp \
    NoteTreeView.cpp \
    NoteWebEngineView.cpp \
    NoteAppAction.cpp \
    NoteDataBase.cpp \
    NoteListView.cpp \
    NoteStackedWidget.cpp \
    NoteAppGlobalInfo.cpp \
    NoteWebEnginePage.cpp \
    NoteScriptContext.cpp \
    NoteMisc.cpp

HEADERS += \
        MainWindow.h \
    NoteButton.h \
    NoteLineEdit.h \
    NoteTreeView.h \
    NoteWebEngineView.h \
    TreeItemDelegate.h \
    NoteAppAction.h \
    NoteDataBase.h \
    NoteListView.h \
    NoteStackedWidget.h \
    NoteAppGlobalInfo.h \
    NoteWebEnginePage.h \
    NoteScriptContext.h \
    NoteMisc.h

FORMS += \
        MainWindow.ui

RESOURCES += \
    resource.qrc

RC_ICONS = QTreeNote.ico
