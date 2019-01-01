#ifndef NOTETREEVIEW_H
#define NOTETREEVIEW_H

#include <QTreeView>
#include <QObject>
#include <QMenu>
#include <QAction>


class NoteTreeView : public QTreeView
{
    Q_OBJECT
public:
    NoteTreeView(QWidget *parent = nullptr);
    void dragEnterEvent( QDragEnterEvent* event );
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent( QDropEvent* event );

    QModelIndex insertChild(int insertID = -1);
    void removeItem();


signals:
    void clicked(int id,QString fileName);
    void updateIndex(QModelIndex &index);
    void slectionChanged(int id,QString fileName);
    void slectionChanged();
public slots:
    void on_treeView_clicked(const QModelIndex &qmIndex);
    void showContextMenu(const QPoint& pos);
    void addTopFile(bool clicked);
    void on_updateIndex(QModelIndex &index);
    void on_selectionChanged(QItemSelection select,QItemSelection deseclect);
private:
    QMenu *menu;
};

#endif // NOTETREEVIEW_H
