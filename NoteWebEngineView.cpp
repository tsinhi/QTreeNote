#include "NoteAppGlobalInfo.h"
#include "NoteWebEnginePage.h"
#include "NoteWebEngineView.h"

#include <QApplication>
#include <QKeyEvent>

NoteWebEngineView::NoteWebEngineView(QWidget *parent):QWebEngineView(parent)
{

    NoteWebEnginePage *webPage = new NoteWebEnginePage(this);
    webPage->setObjectName("webPage");
    setPage(webPage);
    setAcceptDrops(true);


}


void NoteWebEngineView::loadPage(int id,QString fileName)
{
    QString loadFilePath = QApplication::applicationDirPath() + "/Data/start.html";

    if(-1 != id){

        loadFilePath = QApplication::applicationDirPath() + QString("/Data/%1/note.html")
                        .arg(id);
    }

    if( page()->url().toString() != loadFilePath){
        // should save current page,then load new page,
        NoteAppGlobalInfo::instance()->getNoteAppAction()->actionSaveNote();

        page()->load(QUrl(loadFilePath));
    }


}

bool NoteWebEngineView::eventFilter(QObject* watched, QEvent* event)
{

    switch (event->type()) {
//    case QEvent::FocusIn:
//    {
//        QFocusEvent* focusEvent = dynamic_cast<QFocusEvent*>(event);
//        if (focusEvent)
//        {
//            focusInEvent(focusEvent);
//        }
//        break;
//    }
//    case QEvent::FocusOut:
//    {
//        QFocusEvent* focusEvent = dynamic_cast<QFocusEvent*>(event);
//        if (focusEvent)
//        {
//            focusOutEvent(focusEvent);
//        }
//        break;
//    }
    case QEvent::KeyPress:
    {
        QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
        if (keyEvent)
        {
            keyPressEvent(keyEvent);
        }
        break;
    }
    default:
        break;
    }
    return QWebEngineView::eventFilter(watched, event);
}

void NoteWebEngineView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_V && event->modifiers() == Qt::ControlModifier){

        qDebug() << __FUNCTION__ << "CTRL+V pressed" << this;
        triggerPageAction(QWebEnginePage::WebAction::Paste,false);
        return;
    }

    QWebEngineView::keyPressEvent(event);
}

bool NoteWebEngineView::event(QEvent *viewEvent)
{
    return QWebEngineView::event(viewEvent);
}

void NoteWebEngineView::childEvent(QChildEvent* event)
{
    if (event->type() == QEvent::ChildAdded){
        qDebug() << __FUNCTION__ << "ChildAdded";
        event->child()->installEventFilter(this);

    }
    else if (event->type() == QEvent::ChildRemoved){
        qDebug() << __FUNCTION__ << "ChildRemoved";
        event->child()->removeEventFilter(this);
    }

    QWebEngineView::childEvent(event);
}
