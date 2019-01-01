#include "NoteAppGlobalInfo.h"
#include "NoteDataBase.h"

#include <QApplication>
#include <QDir>

NoteDataBase::NoteDataBase(const QString& connectionName,
                           const QString& databasePath)
{
    this->connectionName = connectionName;
    this->databasePath   = databasePath;
}

NoteDataBase::~NoteDataBase()
{
    if(noteDB.isOpen()){
        noteDB.close();
    }
}


bool NoteDataBase::open()
{
    noteDB = QSqlDatabase::addDatabase("QSQLITE",connectionName);
    noteDB.setDatabaseName(databasePath);
    if(!noteDB.open()){
        qFatal("unable to open database");
        return false;
    }

    sqlQuery =  QSqlQuery(noteDB);

    return true;
}

bool NoteDataBase::createNoteTable()
{
    QSqlQuery sql_query(noteDB);
    if(!noteDB.tables().contains("note_index")){
        QString create_sql = "CREATE TABLE note_index ("
                    "ID          INTEGER       PRIMARY KEY AUTOINCREMENT,"
                    "NAME        VARCHAR (256),"
                    "PARENT_ID   INT,"
                    "ORDER_INDEX INTEGER,"
                    "CREATE_TIME DATETIME,"
                    "MODIFY_TIME DATETIME,"
                    "NOTE_TEXT   TEXT)";

        if(!sql_query.exec(create_sql)){
            qDebug() << "Error: Fail to create table." << sql_query.lastError();
        }
        else{
            qDebug() << "Table created success!";

            // insert default item(Recycle Bin,id=1) to table;
            QString sql_insert_default = QString("insert into note_index  values"
                    "(1,\"Recycle Bin\",0,0,NULL,NULL,NULL)");
            sql_query.exec(sql_insert_default);
            sql_query.clear();

            QString strDir = QApplication::applicationDirPath() + "/Data/1";

            QDir dir;
            if (!dir.exists(strDir)){
                bool res = dir.mkpath(strDir);
                qDebug() << "create dir (1)" << res;
                QFile file( strDir + "/note.html" );
                if ( !file.exists()){
                    file.open( QIODevice::WriteOnly );
                    file.close();
                }
            }
        }
    }
    else{
        qDebug() << "Table existed!";
    }

    return true;
}

void NoteDataBase::deleteItemWithChild(int ID,std::function<void(int ID)> callbackFun)
{
    //remove all item from sqlite database
    QVector<int> removeIDs;
    removeIDs.push_back(ID);
    while(removeIDs.size()>0)
    {
        int delId = removeIDs.first();
        QString sql_string = "SELECT ID,PARENT_ID FROM note_index "
                             "WHERE PARENT_ID=%1";
        sql_string = sql_string.arg(delId);
        removeIDs.pop_front();

        sqlQuery.prepare(sql_string);
        if(!sqlQuery.exec()){
            qDebug() << "Error: Fail to select table." << sqlQuery.lastError();
        }
        else{
            while (sqlQuery.next()) {
                removeIDs.push_back(sqlQuery.value(0).toInt());
            }
        }

        //now,it's ok to delete id
        QSqlQuery sql_delete = NoteAppGlobalInfo::getDB()->sqlQuery;
        sql_delete.exec(QString("DELETE FROM note_index "
                                "WHERE ID=%1").arg(delId));
        if(callbackFun != nullptr)
            callbackFun(delId);
    }
}

bool NoteDataBase::addItem(int parentID,std::function<void(int ID)> callbackFun )
{

    if(!sqlQuery.exec(QString("SELECT ID,PARENT_ID,max(ORDER_INDEX)  "
                               "FROM note_index WHERE PARENT_ID=%1").arg(parentID))){
        qDebug() << "Error: Fail to select table." << sqlQuery.lastError();
    }
    else{

        int added_id = 100;
        if(sqlQuery.next()){
            added_id += sqlQuery.value(2).toInt();
            qDebug() << "sql query max order_id = " << added_id - 100;
        }

        if(!sqlQuery.exec(QString("insert into note_index  values"
                                    "(NULL,\"[New File]\",%1,%2,NULL,NULL,NULL)")
                                    .arg(parentID)
                                    .arg(added_id))){
            //todo:rollback
            return false;
        }
        else{
            int insertId = sqlQuery.lastInsertId().toInt();
            //model->setData(child, insertId,Qt::UserRole+1);
            if( callbackFun != nullptr)
                callbackFun(insertId);
        }
    }
    return true;
}


bool NoteDataBase::moveTreeViewItem(int sourceID,int destID,MoveFlag moveFlag)
{
    QString strQuery = QString("SELECT PARENT_ID,ORDER_INDEX FROM note_index WHERE ID=%1")
            .arg(destID);

    int destOrderIndex = -1;
    int destParentID   = 0;

    if(sqlQuery.exec(strQuery)){
        if(sqlQuery.first()){
            destParentID   = sqlQuery.value(0).toInt();
            destOrderIndex = sqlQuery.value(1).toInt();
        }else{
            return false;
        }
    }

    int     newOrderID          = 0;
    int     newSourceParentID   = 0;
    QString strUpdateOrderID    = "";
    QString strUpdateSourceItem = "";

    switch (moveFlag) {
    case MoveFlag::AboveItem:
    {
        int destPreOrderIndex = -1;
        if(sqlQuery.exec( QString("select min(order_index) from note_index "
                                  "where order_index > %1 and parent_id = %2")
                               .arg(destOrderIndex)
                               .arg(destParentID)) ){
            if(sqlQuery.next()){
                destPreOrderIndex = sqlQuery.value(0).toInt();
                newOrderID = (destPreOrderIndex + destOrderIndex)/2+1;
            }
            else{
                newOrderID = destOrderIndex + 100;
            }
        }

        if(newOrderID == destPreOrderIndex){
            strUpdateOrderID = QString("UPDATE note_index "
                                       "SET ORDER_INDEX = ORDER_INDEX + 200 "
                                       "WHERE ORDER_INDEX >= %1 AND PARENT_ID = %2")
                                  .arg(newOrderID)
                                  .arg(destParentID);
            newOrderID += 100;
        }

        newSourceParentID = destParentID;

        break;;
    }


    case MoveFlag::BelowItem:
    {
        int destNextOrderIndex = -1;
        strQuery = QString("select max(order_index) from note_index "
                           "where order_index < %1 and parent_id = %2")
                        .arg(destOrderIndex)
                        .arg(destParentID);

        if(sqlQuery.exec(strQuery)){
            if(sqlQuery.first()){
                destNextOrderIndex = sqlQuery.value(0).toInt();
                newOrderID = (destNextOrderIndex + destOrderIndex)/2+1;
            }else{
                newOrderID = destOrderIndex/2;
            }
        }

        if(newOrderID == destID){
            strUpdateOrderID = QString("UPDATE note_index "
                                       "SET ORDER_INDEX = ORDER_INDEX + 200 "
                                       "WHERE ORDER_INDEX >= %1 AND PARENT_ID = %2")
                                  .arg(newOrderID)
                                  .arg(destParentID);
            newOrderID += 100;
        }

        newSourceParentID = destParentID;

        break;
    }

    case MoveFlag::OnItem:
    {
        int min_oreder_id = 200;
        if(sqlQuery.exec( QString("SELECT min(order_id) FROM note_index WHERE PARENT_ID=%1")
                                    .arg(destID)) ){
            if(sqlQuery.next()){
                min_oreder_id = sqlQuery.value(0).toInt();
            }
        }

        newOrderID = min_oreder_id/2;

        if(newOrderID == 0 ){
            strUpdateOrderID = QString("UPDATE NOTE_INDEX "
                                             "SET ORDER_INDEX = ORDER_INDEX + 200 "
                                             "WHERE  PARENT_ID = %2")
                                        .arg(destID);
            newOrderID += 100;
        }

        newSourceParentID = destID;

        break;
    }

    default:
        break;

    }



    if(!strUpdateOrderID.isEmpty()){
        sqlQuery.exec(strUpdateOrderID);
    }

    strUpdateSourceItem = QString( "UPDATE note_index "
                                   "SET ORDER_INDEX = %1,PARENT_ID=%2 "
                                   "WHERE ID = %3")
                              .arg(newOrderID)
                              .arg(newSourceParentID)
                              .arg(sourceID);

    if(!strUpdateSourceItem.isEmpty()){
       sqlQuery.exec(strUpdateSourceItem);
    }

}
