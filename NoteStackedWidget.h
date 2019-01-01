#ifndef NOTESTACKEDWIDGET_H
#define NOTESTACKEDWIDGET_H

#include <QObject>
#include <QStackedWidget>

class NoteStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    NoteStackedWidget(QWidget *parent = nullptr);
};

#endif // NOTESTACKEDWIDGET_H
