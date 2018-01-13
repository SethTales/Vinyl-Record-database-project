
#include "dbmanager.h"

#include <QString>
#include <vector>
#include <iostream>

databaseService::databaseService()
{
    try
    {
        driver = get_driver_instance();
        connection = driver->connect("localhost", "root", "PimpFarmer99&");
        connection->setAutoCommit(false);
        connection->setSchema("sethRecLib");
    }catch(sql::SQLException &ex){
        std::cout << "Exception occured " << ex.getErrorCode() << std::endl;
    }

}

void databaseService::addNewRecordToDB(struct record)
{

}

QList <record> databaseService::readRecordsFromDB()
{
    sql::PreparedStatement *getAllRecords=connection->prepareStatement
            ("SELECT * FROM recLib");
    sql::ResultSet *recordsFromDB = NULL;

    try
    {
        recordsFromDB = getAllRecords->executeQuery();
        while (recordsFromDB->next())
        {
            recordEntry.bName = (QString::fromStdString(recordsFromDB->getString("bName")));
            std::cout << recordEntry.bName.toStdString() << std::endl;
            recordEntry.aTitle = (QString::fromStdString(recordsFromDB->getString("aTitle")));
            std::cout << recordEntry.aTitle.toStdString() << std::endl;
            recordEntry.genre = (QString::fromStdString(recordsFromDB->getString("genre")));
            std::cout << recordEntry.genre.toStdString() << std::endl;
            recordEntry.year = (QString::fromStdString(recordsFromDB->getString("year")));
            std::cout << recordEntry.year.toStdString() << std::endl;
            recordEntry.recLabel = (QString::fromStdString(recordsFromDB->getString("label")));
            std::cout << recordEntry.recLabel.toStdString() << std::endl;
            list.append(recordEntry);
        }
    }catch(sql::SQLException &ex){
        std::cout << "Exception occured " << ex.getErrorCode() << std::endl;
    }

    recordsFromDB->close();
    getAllRecords->close();
    delete getAllRecords;
    delete recordsFromDB;

    return list;
}

void databaseService::updateRecordInDB(struct record)
{

}

void databaseService::deleteRecordsFromDB(QList<record> recordsToDelete)
{

}
