#ifndef NOTEBUTTON_H
#define NOTEBUTTON_H

#include <QWidget>
#include <QToolButton>
#include <QObject>

class NoteButton : public QToolButton
{
    Q_OBJECT
public:
    NoteButton(QWidget *parent = nullptr);
};

#endif // NOTEBUTTON_H
