#ifndef NOTEAPPGOBALINFO_H
#define NOTEAPPGOBALINFO_H

#include <QObject>
#include <QStandardItemModel>
#include <QWebEnginePage>

#include "NoteAppAction.h"
#include "NoteDataBase.h"
#include "NoteTreeView.h"

class MainWindow;


class NoteAppGlobalInfo : public QObject
{
    Q_OBJECT

private:
    explicit NoteAppGlobalInfo(QObject *parent = nullptr);
public:

    static NoteAppGlobalInfo* instance();

    static MainWindow* getNoteMainWindow();
    static void setNoteMainWindow(MainWindow *mainWin);

    static QString& getDataBaseFilePath();
    static void setDataBaseFilePath(QString filePath);

    static NoteAppAction *getNoteAppAction();
    static void setNoteAppAction(NoteAppAction *noteAppAction);


    static void setRecycleBinItem(QStandardItem *recycleBinItem);
    static QStandardItem * getRecycleBinItem();

    static void setDB(NoteDataBase *noteDB);
    static NoteDataBase *getDB();

    static void setNotePage(QWebEnginePage* notePage);
    static QWebEnginePage *getNotePage();

    static void setTreeView(NoteTreeView* noteTreeView);
    static NoteTreeView* getTreeView();


public:
    ~NoteAppGlobalInfo();
signals:

public slots:

};

#endif // NOTEAPPGOBALINFO_H
