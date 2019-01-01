#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QDragEnterEvent>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardItemModel>
#include "NoteTreeView.h"

#include "NoteAppGlobalInfo.h"
#include "NoteDataBase.h"

QModelIndex g_index;

NoteTreeView::NoteTreeView(QWidget *parent):QTreeView(parent)
{
    connect(this,SIGNAL(clicked(QModelIndex)),
            this,SLOT(on_treeView_clicked(QModelIndex)));

    connect(this, SIGNAL(customContextMenuRequested(const QPoint& )),
            this, SLOT(showContextMenu(const QPoint&)));

    connect(this,SIGNAL(updateIndex(QModelIndex&)),
            this,SLOT(on_updateIndex(QModelIndex&)));

    menu = new QMenu(this);

    QAction* actionAddTopFile = new QAction("Add top File",menu);
    actionAddTopFile ->setObjectName("actionAddTopFile");
    menu->addAction(actionAddTopFile );
    connect(actionAddTopFile,SIGNAL(triggered(bool)),
            this,SLOT(addTopFile(bool)));



}


void NoteTreeView::on_treeView_clicked(const QModelIndex &qmIndex)
{
}


void NoteTreeView::showContextMenu(const QPoint& pos)
{
    QModelIndex index = indexAt(pos);
    menu->exec(QCursor::pos());

}

void NoteTreeView::addTopFile(bool clicked)
{
    setCurrentIndex(rootIndex());
    NoteAppGlobalInfo::instance()->getNoteAppAction()->actionAddNote();

}


void NoteTreeView::dragEnterEvent( QDragEnterEvent* event )
{
    event->setAccepted(true);
}

void NoteTreeView::dragMoveEvent(QDragMoveEvent *event)
{
    QTreeView::dragMoveEvent(event);
}


void NoteTreeView::dropEvent( QDropEvent* event )
{
    NoteDataBase  noteDataBase("NoteTreeView_DragDrop_Connect",
                         NoteAppGlobalInfo::instance()->getDataBaseFilePath());
    noteDataBase.open();

    QIcon qiconFold = QIcon(QStringLiteral(":/icons/fold.png"));
    QIcon qiconFile = QIcon(QStringLiteral(":/icons/file.png"));

    QStandardItemModel *model = static_cast<QStandardItemModel *>(this->model());

    QPoint pos = event->pos();

    QModelIndex dragIndex  = currentIndex();
    qDebug() << dragIndex.data(Qt::EditRole).toString();
    int dragID = dragIndex.data(Qt::UserRole+1).toInt();


    QModelIndex dropIndex =  indexAt(pos);
    qDebug() << dropIndex.data(Qt::EditRole).toString();
    int dropID = dropIndex.data(Qt::UserRole+1).toInt();

    if(!dropIndex.isValid()){
        return;
    }

    QModelIndex dragParentIndex = dragIndex.parent();
    if(dragParentIndex.isValid()){
        int dragRow = dragIndex.row();
        if(!dragIndex.sibling(dragRow+1,0).isValid()
                && !dragIndex.sibling(dragRow-1,0).isValid() ){
            qDebug() << "drag item has no sibling";
            model->setData(dragParentIndex,qiconFile,Qt::DecorationRole);
        }
    }

    int dropParentID = 0;
    if(dropIndex.parent().isValid())
    {
        qDebug() << "dropIndex parent is valid";
        dropParentID = dropIndex.parent().data(Qt::UserRole+1).toInt();

    }

    DropIndicatorPosition dropIndicator = dropIndicatorPosition();
    switch (dropIndicator)
    {
        case QAbstractItemView::AboveItem:{
            noteDataBase.moveTreeViewItem(dragID,dropID,MoveFlag::AboveItem);
        }
        break;

        case QAbstractItemView::BelowItem:{
            noteDataBase.moveTreeViewItem(dragID,dropID,MoveFlag::BelowItem);
        }
        break;

        case QAbstractItemView::OnItem:{
            model->setData(dropIndex,qiconFold,Qt::DecorationRole);
            noteDataBase.moveTreeViewItem(dragID,dropID,MoveFlag::OnItem);
        }
        break;

        case QAbstractItemView::OnViewport:
        break;     
    }

    QTreeView::dropEvent(event);
}


void NoteTreeView::on_updateIndex(QModelIndex &index)
{
}

void NoteTreeView::on_selectionChanged(QItemSelection select,QItemSelection deseclect)
{
    int id = -1;
    QString text = "";
    if(!select.indexes().isEmpty()){
        qDebug() << "select" << select.indexes()[0].data(Qt::EditRole).toString();
        id = select.indexes()[0].data(Qt::UserRole+1).toInt();
        text = select.indexes()[0].data(Qt::EditRole).toString();
    }

    qDebug() << "selectionChanged id " << id << "text =" << text;
    //todo:

    if(!deseclect.indexes().isEmpty()){

        qDebug() << "deselect" << deseclect.indexes()[0].data(Qt::EditRole).toString();
    }
    emit slectionChanged(id,text);

}

void NoteTreeView::removeItem()
{
    QModelIndex index = currentIndex();
    if(!index.isValid()){
        qDebug() << "index is not valid" ;
        return;
    }

    QStandardItemModel *noteModel = static_cast<QStandardItemModel *>(model());
    int id = index.data(Qt::UserRole+1).toInt();

    if( id == 1 ){ // Recycle Bin Item can not be deleted!
        return;
    }

    bool flagDelAtRecycleBin = false;
    QModelIndex indexTmp =  index;
    while(indexTmp.parent().isValid()){
        if(indexTmp.parent().data(Qt::UserRole+1) == 1)
            flagDelAtRecycleBin = true;
        indexTmp = indexTmp.parent();
    }

    if(!flagDelAtRecycleBin){
        // send index to recycle bin (id = 1)

        QString fileName = index.data().toString();
        QIcon delIcon =  index.data(Qt::DecorationRole).value<QIcon>();

        QStandardItem* item = new QStandardItem(delIcon,fileName);
        item->setData(index.data(Qt::UserRole+1),Qt::UserRole+1);

        noteModel->removeRow(index.row(), index.parent());

        QList<QStandardItem *> recycleBinItems;
        recycleBinItems = noteModel->findItems("Recycle Bin");
        QStandardItem *varTmp;

        for(int i = 0; i< recycleBinItems.size();i++){
            varTmp = recycleBinItems.at(i);
            if( varTmp->data(Qt::UserRole+1) == 1)
                break;
        }

        varTmp->insertRow(0,item);

        // update database(move delete item to recycle bin)
        //NoteAppGlobalInfo::getDB()->moveTreeViewItem(id,1,MoveFlag::OnItem);
    }
    else{

        QString delWarningInfo = "Are you sure delete "+index.data(Qt::EditRole).toString() + " ?";
        int returnVal = QMessageBox::question(nullptr,"",
                                              delWarningInfo,QMessageBox::Yes | QMessageBox::No);
        if (returnVal == QMessageBox::No) {
              return;
        }

        noteModel->removeRow(index.row(), index.parent());

        QModelIndex parentIndex = index.parent();
        if(parentIndex.isValid()&& !parentIndex.child(0,0).isValid()){
            noteModel->setData(parentIndex,QIcon(":/icons/file.png"),Qt::DecorationRole);
        }
    }
}

/**
 * @brief NoteTreeView::insertChild
 * @return QModelIndex,inserted child item
 */
QModelIndex NoteTreeView::insertChild(int insertID)
{
    QIcon qiconFold = QIcon(QStringLiteral(":/icons/fold.png"));
    QIcon qiconFile = QIcon(QStringLiteral(":/icons/file.png"));
    QModelIndex index = currentIndex();

    QStandardItemModel *noteModel = static_cast<QStandardItemModel *>(model());

    if (noteModel->columnCount(index) == 0) {
        if (!noteModel->insertColumn(0, index))
            return QModelIndex();
    }

    if (!noteModel->insertRow(0, index))
        return QModelIndex();

    int id = index.data(Qt::UserRole+1).toInt();
    noteModel->setData(index,qiconFold,Qt::DecorationRole);

    QModelIndex child = noteModel->index(0, 0, index);

    noteModel->setData(child, QVariant("[New File]"), Qt::EditRole);
    noteModel->setData(child, qiconFile,Qt::DecorationRole);
    noteModel->setData(child, insertID,Qt::UserRole+1);

    selectionModel()->setCurrentIndex(noteModel->index(0, 0, index),
                                                QItemSelectionModel::ClearAndSelect);

    return child;
}
