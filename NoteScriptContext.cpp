#include "NoteScriptContext.h"

NoteScriptContext::NoteScriptContext(QObject *parent) : QObject(parent)
{

}

void NoteScriptContext::runByScript()
{

    qDebug() << "run by Script";

}
