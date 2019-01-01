#ifndef NOTEWEBENGINEVIEW_H
#define NOTEWEBENGINEVIEW_H

#include <QObject>
#include <QWidget>
#include <QWebEngineView>


class NoteWebEngineView : public QWebEngineView
{
    Q_OBJECT
public:
    NoteWebEngineView(QWidget *parent = nullptr);
protected:

    virtual void keyPressEvent(QKeyEvent* event);
    virtual bool event(QEvent *viewEvent);
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void childEvent(QChildEvent* event);

public slots:
    void loadPage(int id,QString fileName);


};

#endif // NOTEWEBENGINEVIEW_H
