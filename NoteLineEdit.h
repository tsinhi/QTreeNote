#ifndef NOTESEARCHWIDGET_H
#define NOTESEARCHWIDGET_H

#include <QLineEdit>
#include <QWidget>
#include <QObject>
#include <QAction>


class NoteLineEdit : public QLineEdit
{
    Q_OBJECT
    Q_PROPERTY(bool searchState READ searchState WRITE setState NOTIFY StateChanged)
public:
    NoteLineEdit(QWidget *parent = nullptr );
    QAction *getSearchAction();
public:
    QAction *actionSearch;
    bool searchState() const;

public slots:
    void setState(bool searchState);

signals:
    void StateChanged(bool searchState);

private:


    bool m_searchState;
};

#endif // NOTESEARCHWIDGET_H
