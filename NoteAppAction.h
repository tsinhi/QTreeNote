#ifndef NOTEAPPACTION_H
#define NOTEAPPACTION_H

#include <QObject>


class NoteAppAction : public QObject
{
    Q_OBJECT
public:
    NoteAppAction();

signals:
    void updateCurrentNote(int id,QString fileName);

public slots:
    void actionHomePage();
    void actionEnableEdit();
    void actionAddNote();
    void actionDelNote();
    void actionSaveNote();
    void actionSearch();
    void actionSearchTextChg(QString searchText);


};

#endif // NOTEAPPACTION_H
