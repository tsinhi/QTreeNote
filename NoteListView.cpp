#include "NoteListView.h"

#include "NoteAppGlobalInfo.h"

NoteListView::NoteListView(QWidget *parent):QListView(parent)
{
    connect(this,SIGNAL(clicked(QModelIndex)),
            this,SLOT(on_clicked(QModelIndex)));

}


void NoteListView::on_clicked(QModelIndex index)
{
    int id = index.data(Qt::UserRole+1).toInt();
    QString fileName =  index.data(Qt::EditRole).toString();
    emit clicked(id,fileName);

}
