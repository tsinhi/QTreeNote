#ifndef TREEITEMDELEGATE_H
#define TREEITEMDELEGATE_H

#include <QObject>
#include <QWidget>
#include <QItemDelegate>

class TreeItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TreeItemDelegate(QObject *parent = nullptr){}
    virtual void setModelData(QWidget *editor,
                                  QAbstractItemModel *model,
                                  const QModelIndex &index) const
        {
            QString lastFileName = model->data(index,Qt::EditRole).toString();
            QItemDelegate::setModelData(editor, model, index);
            if (index.column() == 0) {
                emit editingFinished(index,lastFileName);
            }
        }

    signals:
    void editingFinished(const QModelIndex &,const QString&) const;

};

#endif // TREEITEMDELEGATE_H
