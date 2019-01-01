#include <QDebug>
#include <QDir>
#include <QIcon>
#include <QStack>
#include <QWebEnginePage>
#include <QMessageBox>
#include <QApplication>

#include "NoteAppAction.h"
#include "NoteAppGlobalInfo.h"
#include "NoteMisc.h"
#include "MainWindow.h"
#include "NoteLineEdit.h"
#include "NoteStackedWidget.h"
#include "NoteListView.h"


NoteAppAction::NoteAppAction()
{

}


void NoteAppAction::actionHomePage()
{
    NoteAppGlobalInfo::instance()->getNotePage()
        ->load(QUrl(QApplication::applicationDirPath() + "/Data/start.html"));

    NoteAppGlobalInfo::instance()->getTreeView()->clearSelection();
}

void NoteAppAction::actionEnableEdit()
{
    NoteAppGlobalInfo::instance()->getNotePage()->runJavaScript("enableEditor()");
}

void NoteAppAction::actionAddNote()
{
    NoteTreeView *treeView = NoteAppGlobalInfo::instance()->getTreeView();
    int id = treeView->currentIndex().data(Qt::UserRole+1).toInt();

    NoteAppGlobalInfo::getDB()->addItem(id,[treeView](int insertId){

        QString pathNewNote =QApplication::applicationDirPath() + "/Data/%1";
        pathNewNote = pathNewNote.arg(insertId);

        QDir dirNote;
        if(!dirNote.exists(pathNewNote)){
            dirNote.mkpath(pathNewNote);
            QFile::copy(QApplication::applicationDirPath()
                        + "/Files/template/default/note.html",
                        pathNewNote + "/note.html");
        }

        treeView->insertChild(insertId);

    });

}

void NoteAppAction::actionDelNote()
{
    NoteTreeView *treeView = NoteAppGlobalInfo::instance()->getTreeView();

    QModelIndex index = treeView->selectionModel()->currentIndex();
    if(!index.isValid()){
        qDebug() << "index is not valid" ;
        return;
    }

    int id = index.data(Qt::UserRole+1).toInt();

    if( id == 1 ){ // Recycle Bin Item can not be deleted!
        return;
    }

    treeView->removeItem();

    bool flagDelAtRecycleBin = false;
    QModelIndex indexTmp =  index;
    while(indexTmp.parent().isValid()){
        if(indexTmp.parent().data(Qt::UserRole+1) == 1)
            flagDelAtRecycleBin = true;
        indexTmp = indexTmp.parent();
    }

    if(!flagDelAtRecycleBin){
        // update database(move delete item to recycle bin)
        NoteAppGlobalInfo::getDB()->moveTreeViewItem(id,1,MoveFlag::OnItem);
    }
    else{
        NoteAppGlobalInfo::getDB()->deleteItemWithChild(id,[](int delId){
            QString pathDelNote = QApplication::applicationDirPath() + "/Data/%1";
            pathDelNote = pathDelNote.arg(delId);
            delDirRecursive(pathDelNote);
        });

    }

}

void NoteAppAction::actionSaveNote()
{
    QWebEnginePage* webViewPage = NoteAppGlobalInfo::instance()->getNotePage();

    webViewPage->runJavaScript("getNoteHtml()",
                               [webViewPage](QVariant val){

        QUrl url = webViewPage->url();
        QString htmlString = val.toString();

        if(!val.isValid()|| htmlString.isEmpty())
            return;


        QFile file(url.toLocalFile());
        if (!file.open(QFile::WriteOnly | QIODevice::Truncate)){
            qDebug() << "open file failed" << endl;
            return;
        }

        file.write(htmlString.toUtf8());
        file.close();

    });

    if(webViewPage->url().toLocalFile() == QApplication::applicationDirPath()+ "/Data/start.html"){
        return;
    }

    QString localFilePath = webViewPage->url().toLocalFile();
    int note_id = localFilePath
            .replace(QApplication::applicationDirPath()+"/Data/","")
            .replace("/note.html","").toInt();

    if(note_id < 0)
        return;


    webViewPage->runJavaScript("getNotePlainText()",
                               [note_id](QVariant val){
        QString pagePlainText = val.toString();
        QSqlQuery sql_update = NoteAppGlobalInfo::getDB()->sqlQuery;
        sql_update.exec(QString("UPDATE NOTE_INDEX "
                                "SET NOTE_TEXT = '%1' "
                                ).arg(pagePlainText)
                      + QString("WHERE ID = %1").arg(note_id)) ;
    });

}

void NoteAppAction::actionSearch()
{
    MainWindow *mainWin = NoteAppGlobalInfo::instance()->getNoteMainWindow();

    NoteLineEdit       *lineEditSearch  = mainWin->findChild<NoteLineEdit *>("lineEditSearch");
    NoteStackedWidget  *stackedWidget   = mainWin->findChild<NoteStackedWidget *>("stackedWidget");
    QWebEnginePage     *webViewPage     = NoteAppGlobalInfo::instance()->getNotePage();

    QString textSearch = lineEditSearch->text();

    if(textSearch.isEmpty()){
        return;
    }

    if(lineEditSearch->searchState()){
        lineEditSearch->setProperty("searchState",false);
        lineEditSearch->setText("");
        stackedWidget->setCurrentIndex(0);
        return;
    }

    stackedWidget->setCurrentIndex(1);
    lineEditSearch->setProperty("searchState",true);

    NoteListView *listView = mainWin->findChild<NoteListView *>("listView");

    QSqlQuery sql_query = NoteAppGlobalInfo::getDB()->sqlQuery;
    const QString sql_query_string = QString("SELECT ID,NAME,NOTE_TEXT FROM NOTE_INDEX "
                                     "WHERE NAME LIKE '%%1%' "
                                     "OR NOTE_TEXT LIKE '%%1%'/* case-insensitive; */")
                                    .arg(textSearch);

    if(sql_query.exec(sql_query_string)){
        QStandardItemModel * listViewModel = (QStandardItemModel *)listView->model();
        listViewModel->clear();
        while(sql_query.next()){
            qDebug() << sql_query.value(1).toString();
            QStandardItem *item = new QStandardItem(QIcon(":/icons/file.png")
                                                    ,sql_query.value(1).toString());
            item->setData(sql_query.value(0).toInt(),
                          Qt::UserRole+1);
            listViewModel->appendRow(item);

        }
    }

    webViewPage->findText(textSearch);

}

void NoteAppAction::actionSearchTextChg(QString searchText)
{

    MainWindow *mainWin = NoteAppGlobalInfo::instance()->getNoteMainWindow();
    NoteStackedWidget  *stackedWidget   = mainWin->findChild<NoteStackedWidget *>("stackedWidget");
    NoteLineEdit  *lineEditSearch  = mainWin->findChild<NoteLineEdit *>("lineEditSearch");

    if(searchText.isEmpty())
        stackedWidget->setCurrentIndex(0);

    lineEditSearch->setProperty("searchState",false);

}
