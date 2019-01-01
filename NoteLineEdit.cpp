#include <QCompleter>
#include <QPushButton>
#include <QVBoxLayout>

#include "NoteLineEdit.h"


NoteLineEdit::NoteLineEdit(QWidget *parent)
    :QLineEdit(parent)
{

     setPlaceholderText(QStringLiteral("search..."));

     actionSearch = new QAction(QIcon(":/icons/search.png"),"search",this);
     addAction(actionSearch,QLineEdit::TrailingPosition);
     setTextMargins(8,0,0,0);

     //TODO: search history
     QCompleter *completer = new QCompleter(this);
     setCompleter(completer);


}

QAction *NoteLineEdit::getSearchAction()
{
    return actionSearch;
}

bool NoteLineEdit::searchState() const
{
    return m_searchState;
}


void NoteLineEdit::setState(bool searchState)
{
    if (m_searchState == searchState)
        return;

    m_searchState = searchState;

    if(searchState){
        actionSearch->setIcon(QIcon(":/icons/X.png"));
    }
    else{
        actionSearch->setIcon(QIcon(":/icons/search.png"));
    }

    emit StateChanged(m_searchState);
}
