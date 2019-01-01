#include "NoteAppGlobalInfo.h"

#include <QStandardItemModel>
#include <QWebEnginePage>


static MainWindow* g_MainWindow = nullptr;
static QString g_NoteDataBaseFilePath = "";
static NoteAppAction * g_noteAppAction = nullptr;
static QStandardItem* g_recycleBinItem = nullptr;
static NoteDataBase *g_noteDataBase = nullptr;
static QWebEnginePage *g_NotePage = nullptr;
static NoteTreeView *g_NoteTreeView = nullptr;

NoteAppGlobalInfo::NoteAppGlobalInfo(QObject *parent) : QObject(parent)
{

}

NoteAppGlobalInfo::~NoteAppGlobalInfo()
{
    g_MainWindow = NULL;
}



NoteAppGlobalInfo* NoteAppGlobalInfo::instance()
{
    static NoteAppGlobalInfo noteInstance;
    return &noteInstance;
}

void NoteAppGlobalInfo::setNoteMainWindow(MainWindow *mainWin)
{
    g_MainWindow = mainWin;

}

void NoteAppGlobalInfo::setDataBaseFilePath(QString filePath)
{
    g_NoteDataBaseFilePath = filePath;
}

MainWindow* NoteAppGlobalInfo::getNoteMainWindow()
{
    return g_MainWindow;
}

QString& NoteAppGlobalInfo::getDataBaseFilePath()
{
    return g_NoteDataBaseFilePath;
}

void NoteAppGlobalInfo::setNoteAppAction(NoteAppAction *noteAppAction)
{
     g_noteAppAction = noteAppAction;
}


NoteAppAction *NoteAppGlobalInfo::getNoteAppAction()
{
    return g_noteAppAction;
}

void NoteAppGlobalInfo::setRecycleBinItem(QStandardItem *recycleBinItem)
{
     g_recycleBinItem = recycleBinItem;
}


QStandardItem *NoteAppGlobalInfo::getRecycleBinItem()
{
    return g_recycleBinItem;
}

void NoteAppGlobalInfo::setDB(NoteDataBase *noteDB)
{
     g_noteDataBase = noteDB;
}


NoteDataBase *NoteAppGlobalInfo::getDB()
{
    return g_noteDataBase;
}


void NoteAppGlobalInfo::setNotePage(QWebEnginePage *notePage)
{
     g_NotePage = notePage;
}


QWebEnginePage *NoteAppGlobalInfo::getNotePage()
{
    return g_NotePage;
}

void NoteAppGlobalInfo::setTreeView(NoteTreeView* noteTreeView)
{
    g_NoteTreeView = noteTreeView;
}

NoteTreeView* NoteAppGlobalInfo::getTreeView()
{
    return g_NoteTreeView;
}




