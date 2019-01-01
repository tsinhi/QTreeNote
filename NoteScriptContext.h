#ifndef NOTESCRIPTCONTEXT_H
#define NOTESCRIPTCONTEXT_H

#include <QObject>
#include <QDebug>

class NoteScriptContext : public QObject
{
    Q_OBJECT
public:
    explicit NoteScriptContext(QObject *parent = nullptr);

signals:

public slots:
    void runByScript();

};

#endif // NOTESCRIPTCONTEXT_H
