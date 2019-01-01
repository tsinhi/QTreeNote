#ifndef NOTELISTVIEW_H
#define NOTELISTVIEW_H

#include <QListView>
#include <QObject>
#include <QDebug>

class NoteListView : public QListView
{
    Q_OBJECT
public:
    NoteListView(QWidget *parent = nullptr);

signals:
    void clicked(int id,QString fileName);
    void slectionChanged();
public slots:
    void on_clicked(QModelIndex index);
};

#endif // NOTELISTVIEW_H
