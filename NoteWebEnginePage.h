#ifndef NOTEWEBENGINEPAGE_H
#define NOTEWEBENGINEPAGE_H

#include <QObject>
#include <QWebEnginePage>
#include <QApplication>

class NoteWebEnginePage : public QWebEnginePage
{
    Q_OBJECT
public:
    NoteWebEnginePage(QObject* parent = 0);


public slots:
    void on_finishedLoad(bool);
protected:
    virtual void triggerAction(WebAction action, bool checked = false);

};

#endif // NOTEWEBENGINEPAGE_H
