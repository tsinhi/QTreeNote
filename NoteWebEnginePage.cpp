#include "NoteWebEnginePage.h"

#include <QClipboard>
#include <QDir>
#include <QFileInfo>

NoteWebEnginePage::NoteWebEnginePage(QObject* parent):QWebEnginePage(parent)
{
    connect(this,SIGNAL(loadFinished(bool)),this,SLOT(on_finishedLoad(bool)));

}


void NoteWebEnginePage::on_finishedLoad(bool finised)
{
    QString runInitScript =
            "var noteScript = document.createElement('script');"
            "noteScript.type = 'text/javascript';"
            "noteScript.charset = 'utf-8';"
            "noteScript.src='../../Script/note.js';"
            "noteScript.onload=function(){initNoteEditor()};"
            "document.head.appendChild(noteScript);";
    runJavaScript(runInitScript);

}


void NoteWebEnginePage::triggerAction(NoteWebEnginePage::WebAction action, bool checked /*= false*/)
{
    if(action == QWebEnginePage::Paste){
        QClipboard* clipboard = QApplication::clipboard();
        if (!clipboard)
            return ;

        QImage image = clipboard->image();
        if (!image.isNull()){
            qDebug() << __FUNCTION__ << "clipboard is an image" ;
            QString pageFilePath = url().toLocalFile();
            qDebug() << __FUNCTION__ << pageFilePath;
            QFileInfo fileInfo(pageFilePath);

            QTime time = QTime::currentTime();
            QDate date = QDate::currentDate();
            QString imageFileName = date.toString(Qt::DateFormat::ISODate)
                    + "_" + QString::number( time.msecsSinceStartOfDay())
                    + ".png";
            QString imageFileStr = fileInfo.absoluteDir().absolutePath() + "/"
                    + imageFileName;

            qDebug() << imageFileStr;

            image.save(imageFileStr);

            QString scriptStr = QString("noteInsertHtml('<img src=\"%1\" />')")
                    .arg(imageFileName);

            runJavaScript(scriptStr);

            return ;
        }
    }




    //QWebEnginePage::triggerAction(action, checked);
}
