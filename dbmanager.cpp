
#include "dbmanager.h"

#include <QString>
#include <vector>
#include <iostream>

databaseService::databaseService()
{
    try
    {
        driver = get_driver_instance();
        connection = driver->connect("tcp://mysql-instance1.cysndijadlug.us-west-2.rds.amazonaws.com:3306",
                                     "SethTales1015", "PimpFarmer99&");
        connection->setSchema("recLib");
        connection->setAutoCommit(false);
    }catch(sql::SQLException &ex){
        std::cout << "Connection exception occured " << ex.getErrorCode() << std::endl;
    }

}

void databaseService::addNewRecordToDB(struct record recordEntry)
{
    sql::PreparedStatement *addRecord = connection->prepareStatement
            ("INSERT INTO sethRecLib ( bandName, albumTitle, genre, yearReleased, recLabel ) "
             "VALUES (?, ?, ?, ?, ?)" );
    try{
        addRecord->setString(1, recordEntry.bName.toStdString());
        addRecord->setString(2, recordEntry.aTitle.toStdString());
        addRecord->setString(3, recordEntry.genre.toStdString());
        addRecord->setString(4, recordEntry.year.toStdString());
        addRecord->setString(5, recordEntry.recLabel.toStdString());
        addRecord->execute();
        connection->commit();
    }catch (sql::SQLException &ex) {
                std:: cout << "Add new record exception occured" << ex.getErrorCode();
    }
    addRecord->close();
    delete addRecord;
}

QList <record> databaseService::readRecordsFromDB()
{
    sql::PreparedStatement *getAllRecords = connection->prepareStatement
            ("SELECT * FROM sethRecLib");
    sql::ResultSet *recordsFromDB = NULL;

    try
    {
        recordsFromDB = getAllRecords->executeQuery();
        while (recordsFromDB->next())
        {
            recordEntry.ID = (recordsFromDB->getInt("ID"));
            recordEntry.bName = (QString::fromStdString(recordsFromDB->getString("bandName")));
            recordEntry.aTitle = (QString::fromStdString(recordsFromDB->getString("albumTitle")));
            recordEntry.genre = (QString::fromStdString(recordsFromDB->getString("genre")));
            recordEntry.year = (QString::fromStdString(recordsFromDB->getString("yearReleased")));
            recordEntry.recLabel = (QString::fromStdString(recordsFromDB->getString("recLabel")));
            list.append(recordEntry);
        }
    }catch(sql::SQLException &ex){
        std::cout << "Read exception occured " << ex.getErrorCode() << std::endl;
    }

    recordsFromDB->close();
    getAllRecords->close();
    delete getAllRecords;
    delete recordsFromDB;

    return list;
}

void databaseService::updateRecordInDB(struct record recordEntry)
{
    sql::PreparedStatement *updateRecord = connection->prepareStatement
            ("UPDATE sethRecLib SET bandName=?, albumTitle=?, genre=?, yearReleased=?, recLabel=? WHERE ID=?");
    try{
        updateRecord->setString(1, recordEntry.bName.toStdString());
        updateRecord->setString(2, recordEntry.aTitle.toStdString());
        updateRecord->setString(3, recordEntry.genre.toStdString());
        updateRecord->setString(4, recordEntry.year.toStdString());
        updateRecord->setString(5, recordEntry.recLabel.toStdString());
        updateRecord->setInt(6, recordEntry.ID);
        updateRecord->execute();
        connection->commit();
    }catch(sql::SQLException &ex){
        std::cout << "Update exception occured " << ex.getErrorCode() << std::endl;
    }

    updateRecord->close();
    delete updateRecord;
}

void databaseService::deleteRecordFromDB(int ID)
{
    sql::PreparedStatement *deleteRecord = connection->prepareStatement
            ("DELETE FROM sethRecLib WHERE ID=?");
    try{
        deleteRecord->setInt(1, ID);
        deleteRecord->execute();
        connection->commit();
    }catch(sql::SQLException &ex){
        std::cout << "Delete exception occured " << ex.getErrorCode() << std::endl;
    }

    deleteRecord->close();
    delete deleteRecord;
}

bool databaseService::isTableEmpty()
{
    int count;

    sql::Statement *stmt = connection->createStatement();
    sql::ResultSet *rs = stmt->executeQuery("SELECT ID FROM sethRecLib");

    //count = rs;

    while (rs->next())
    {
        count = rs->getInt("ID");
    }

    if (count == 0)
    {
        return true;
    }

    else
    {
        return false;
    }

    stmt->close();
    rs->close();
    delete stmt;
    delete rs;
}
