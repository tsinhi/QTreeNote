#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "TreeItemDelegate.h"

#include <QFileInfo>
#include <QStack>
#include <QDir>
#include <QBitmap>
#include <QWebEngineSettings>
#include <QWebChannel>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>

#include "NoteAppAction.h"
#include "NoteAppGlobalInfo.h"
#include "NoteButton.h"
#include "NoteDataBase.h"
#include "NoteScriptContext.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    NoteAppGlobalInfo::instance()->setNoteMainWindow(this);
    NoteAppGlobalInfo::instance()->setNotePage(ui->webView->page());
    NoteAppGlobalInfo::instance()->setTreeView(ui->treeView);
    NoteAppGlobalInfo::instance()->setDataBaseFilePath(
                QApplication::applicationDirPath() + "/Data/note.db");

    initNoteUi();
    initApp();
    initNote();

    initTreeViewMenu();
}


void MainWindow::initNoteUi()
{
    ui->webView->settings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);
    ui->webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
    ui->webView->settings()->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
    ui->webView->settings()->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);

    ui->lineEditSearch->setStyleSheet("border:1px ;border-radius:16px;");
}

void MainWindow::initApp()
{

    NoteScriptContext *scriptContext = new NoteScriptContext(this);
    QWebChannel *webChannel = new QWebChannel(ui->webView->page());
    webChannel->registerObject("noteScriptContext",scriptContext);
    ui->webView->page()->setWebChannel(webChannel);

    NoteAppAction *appAction = new NoteAppAction();
    NoteAppGlobalInfo::setNoteAppAction(appAction);

    QFile configJsonFile(QApplication::applicationDirPath()+"/Actions/actions.json");
    if(!configJsonFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "open actions.json file failed";
        return;
    }

    QByteArray configData = configJsonFile.readAll();
    configJsonFile.close();

    QJsonParseError jsonPareErr;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(configData, &jsonPareErr));
    if(jsonPareErr.error != QJsonParseError::NoError){
        qDebug() << "parse action json data error!";
        return;
    }

    QJsonArray rootArrayJson = jsonDoc.array();
    for(int i = 0; i< rootArrayJson.size() ;++i){
        QJsonObject jsonObj =  rootArrayJson.at(i).toObject();
        int size  = jsonObj.take("size").toInt();
        QJsonArray jsonGeometry = jsonObj.take("geometry").toArray();

        NoteButton *appButton =  new NoteButton(ui->appWidget);
        appButton->setStyleSheet("border:none");
        appButton->setText(jsonObj.take("actionName").toString());
        appButton->setIcon(QIcon(jsonObj.take("icon").toString()));
        appButton->setIconSize(QSize(size,size));
        appButton->setMinimumSize(QSize(size, size));
        appButton->setGeometry( QRect( jsonGeometry[0].toInt(),
                                       jsonGeometry[1].toInt(),
                                       jsonGeometry[2].toInt(),
                                       jsonGeometry[3].toInt()));

        QString strAction = "1"+jsonObj.take("action").toString();//construct SLOT
        connect(appButton,SIGNAL(clicked()),
                appAction,strAction.toLatin1().data());

    }

    connect(ui->lineEditSearch->getSearchAction(),SIGNAL(triggered()),
            appAction,SLOT(actionSearch()));

    connect(ui->lineEditSearch,SIGNAL(returnPressed()),
            appAction,SLOT(actionSearch()));

    connect(ui->lineEditSearch,SIGNAL(textChanged(QString)),
            appAction,SLOT(actionSearchTextChg(QString)));



}

void MainWindow::initNote()
{

    NoteDataBase *noteDataBase= new NoteDataBase("GlobalNoteConnection",
                        NoteAppGlobalInfo::getDataBaseFilePath());
    if(noteDataBase->open()){
        noteDataBase->createNoteTable();
        NoteAppGlobalInfo::setDB(noteDataBase);
    }


    QStandardItemModel *model   =   new QStandardItemModel ( ui -> treeView );
    ui->treeView->setModel(model);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral(""));

    connect(ui->treeView->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            ui->treeView,SLOT(on_selectionChanged(QItemSelection,QItemSelection)));

    connect(ui->treeView,SIGNAL(slectionChanged(int,QString)),
            ui->webView,SLOT(loadPage(int,QString)));

    connect(ui->listView,SIGNAL(clicked(int,QString)),
            ui->webView,SLOT(loadPage(int,QString)));

    QStandardItem* itemRoot = model->invisibleRootItem();
    QStandardItem* parent_item = itemRoot;

    // change it
    QStack< QPair<QStandardItem *,int> > queryStack;
    queryStack.push_back(QPair<QStandardItem *,int>(itemRoot,0));
    int parent_id = 0;

    QSqlQuery sql_query = noteDataBase->sqlQuery;


    while(queryStack.size()> 0){
        parent_item = queryStack.first().first;
        parent_id = queryStack.first().second;
        queryStack.pop_front();
        if(sql_query.exec(QString("select * from note_index "
                                   "where PARENT_ID = %1 "
                                   "ORDER BY ORDER_INDEX DESC").arg(parent_id)))
        {
            int countQuery = 0;
            while(sql_query.next())
            {
                ++countQuery;
                int id = sql_query.value(0).toInt();

                QString fileName = sql_query.value(1).toString();
                QStandardItem* item = new QStandardItem(QIcon(":/icons/file.png"),fileName);
                //set id to item
                item->setData(id,Qt::UserRole+1);

                if(id == 1){
                    NoteAppGlobalInfo::instance()->setRecycleBinItem(item);
                }

                parent_item->appendRow(item);
                queryStack.push_back(QPair<QStandardItem *,int>(item,id));

            }

            if(countQuery > 0){
                parent_item->setData(QIcon(":/icons/fold.png"),Qt::DecorationRole);
            }
        }

    }

    auto editDelegate = new TreeItemDelegate(this);

    ui->treeView->setItemDelegate(editDelegate);
    connect(editDelegate,
            &TreeItemDelegate::editingFinished,
            [this](const QModelIndex &index,const QString &lastFileName){
                qDebug() << lastFileName <<  "editDelegate Edit finished!";
                QAbstractItemModel *model = ui->treeView->model();
                QString text = model->data(index,Qt::DisplayRole).toString();
                int id = model->data(index,Qt::UserRole+1).toInt();

                if(lastFileName == text){
                    qDebug() << "data not changed,return ";
                    return;
                };

                QString loadFilePath = QApplication::applicationDirPath() +
                        "/Data/%1/note.html";
                loadFilePath = loadFilePath.arg(id);
                QFileInfo file(loadFilePath);
                QString strOldFile =  file.absoluteDir().absolutePath()+"/"
                        +QString::number(id)+"_"+lastFileName+".html";
                qDebug() << strOldFile << "rename to " << loadFilePath << endl;

                //TODO: change <title></title> element
                QSqlQuery sql_update = NoteAppGlobalInfo::getDB()->sqlQuery;
                sql_update.exec(QString("UPDATE note_index SET NAME=\"%1\" WHERE ID=%2")
                                    .arg(text).arg(id));

    });

    QStandardItemModel *model_listview   =   new QStandardItemModel ( ui->listView );
    ui->listView->setModel(model_listview);
    ui->webView->page()->load(QUrl(QApplication::applicationDirPath() + "/Data/start.html"));
}


void MainWindow::initTreeViewMenu()
{

}



MainWindow::~MainWindow()
{
    delete ui;
}
