#ifndef NOTEDATABASE_H
#define NOTEDATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>


enum MoveFlag{
    AboveItem,
    BelowItem,
    OnItem
};

/**
 * @brief Just for SQLite3
 */
class NoteDataBase
{
public:
    explicit NoteDataBase(const QString& connectionName,
                          const QString& databasePath);
    bool moveTreeViewItem(int sourceID,int destID,MoveFlag moveFlag);
    bool open();
    bool createNoteTable();
    void deleteItemWithChild(int ID,std::function<void(int ID)> callbackFun = NULL);
    bool addItem(int parentID,std::function<void(int ID)> callbackFun = NULL);
    void travelNoteRecursive(int rootID,std::function<void(int ID,int parent_ID)> callbackFun = NULL);

    virtual ~NoteDataBase();
public:
    QSqlQuery sqlQuery;
private:
    QString databasePath;
    QString connectionName;
    QSqlDatabase noteDB;

};

#endif // NOTEDATABASE_H
