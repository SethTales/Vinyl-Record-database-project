
#include "dbmanager.h"

#include <QString>
#include <vector>
#include <iostream>
#include <string>

using namespace Aws::SNS::Model;

databaseService::databaseService()
{
    try
    {
        driver = get_driver_instance();
        connection = driver->connect("tcp://mysql-instance1.cysndijadlug.us-west-2.rds.amazonaws.com:3306",
                                     "SethTales1015", "PimpFarmer99&");
        connection->setAutoCommit(false);
    }catch(sql::SQLException &ex){
        std::cout << "Connection exception occured " << ex.getErrorCode() << std::endl;
    }

}

void databaseService::clearUserCredentials()
{
    sessionUserCredentials.username = "";
    sessionUserCredentials.password = "";
    sessionUserCredentials.sqAnswer = "";
    sessionUserCredentials.ID = 0;
    sessionUserCredentials.sqIndex = 0;
}

//functions for login manager
bool databaseService::checkNewUserCredentials(userCreds userCredentials)
{
    connection->setSchema("recLib");
    int resultCount = 0;
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT * FROM userCredentials WHERE username = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        pstmt->setString(1, userCredentials.username);
        resultSet = pstmt->executeQuery();
        connection->commit();
        resultCount = resultSet->rowsCount();

    }catch(sql::SQLException &ex){
        std::cout << "checkNewUserCredentials exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    if (resultCount == 0)
    {
        return true;
    }

    else
    {
        return false;
    }

}

bool databaseService::addNewUser(userCreds userCredentials)
{
    connection->setSchema("recLib");
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("INSERT INTO userCredentials (username, password, sqQuestion, sqAnswer)"
             "VALUES (?, ?, ?, ?)");
    try{
        pstmt->setString(1, userCredentials.username);
        std::cout << userCredentials.username << std::endl;
        pstmt->setString(2, userCredentials.password);
        std::cout << userCredentials.password << std::endl;
        pstmt->setInt(3, userCredentials.sqIndex);
        std::cout << userCredentials.sqIndex << std::endl;
        pstmt->setString(4, userCredentials.sqAnswer);
        std::cout << userCredentials.sqAnswer << std::endl;
        pstmt->execute();
        connection->commit();
    } catch (sql::SQLException &ex) {
        std::cout << "Add new user exception occured " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    delete pstmt;
}

void databaseService::subscribeNewUser()
{
    //SubscribeRequest *subRequest = new SubscribeRequest;
    //subRequest->SetTopicArn("arn:aws:sns:us-west-2:944658851848:new-user-registered");

}

int databaseService::getUserID(userCreds userCredentials)
{
    connection->setSchema("recLib");
    //int ID;
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT user_ID from userCredentials WHERE username = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        std::cout << "username in getUserID = " << userCredentials.username << std::endl;
        pstmt->setString(1, userCredentials.username);
        resultSet = pstmt->executeQuery();
        while (resultSet->next())
        {
            userCredentials.ID = (resultSet->getInt("user_ID"));
            //sessionUserCredentials.ID = userCredentials.ID;
            std::cout << userCredentials.ID << std::endl;
        }

    }catch(sql::SQLException &ex){
        std::cout << "user ID Exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    return userCredentials.ID;
}

void databaseService::createSchema(userCreds userCredentials)
{
    std::cout << "userCreds.ID in createSchema = " << std::to_string(userCredentials.ID) << std::endl;
    std::string statement = "create schema user" + std::to_string(userCredentials.ID) + "database";
    sql::Statement *stmt = connection->createStatement();

    try {
        stmt->execute(statement);
        connection->commit();
        //schemaName = "user" + std::to_string(userCredentials.ID) + "database";
        //std::cout << "schemaName = " << schemaName << std::endl;
    } catch (sql::SQLException &ex) {
        std::cout << "create schema exception occured: " << ex.getErrorCode() << std::endl;
    }

    stmt->close();
    delete stmt;
}

bool databaseService::login(userCreds userCredentials)
{
    connection->setSchema("recLib");
    int resultCount = 0;
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT * FROM userCredentials WHERE username = ? AND password = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        pstmt->setString(1, userCredentials.username);
        pstmt->setString(2, userCredentials.password);
        resultSet = pstmt->executeQuery();
        //connection->commit();
        resultCount = resultSet->rowsCount();

    }catch(sql::SQLException &ex){
        std::cout << "login Exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    std::cout << "resultCount = " << resultCount << std::endl;

    if (resultCount == 1)
    {
        sql::PreparedStatement *pstmt = connection->prepareStatement
                ("SELECT user_ID from userCredentials WHERE username = ?");
        sql::ResultSet *resultSet = NULL;

        try
        {
            pstmt->setString(1, userCredentials.username);
            resultSet = pstmt->executeQuery();
            while (resultSet->next())
            {
                userCredentials.ID = (resultSet->getInt("user_ID"));
                sessionUserCredentials.username = userCredentials.username;
                std::cout << "session uname = " << sessionUserCredentials.username << std::endl;
                sessionUserCredentials.password = userCredentials.password;
                std::cout << "session pword = " << sessionUserCredentials.password << std::endl;
                sessionUserCredentials.ID = userCredentials.ID;
                std::cout << "session ID = " << sessionUserCredentials.ID << std::endl;
                schemaName = "user" + std::to_string(userCredentials.ID) + "database";
                std::cout << "schemaName = " << schemaName << std::endl;
            }

        }catch(sql::SQLException &ex){
            std::cout << "user ID Exception occurred " << ex.getErrorCode() << std::endl;
        }

        pstmt->close();
        resultSet->close();
        delete pstmt;
        delete resultSet;

        return true;
    }

    else
    {
        return false;
    }
}

void databaseService::storeUserID(userCreds userCredentials)
{
    connection->setSchema("recLib");
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT user_ID from userCredentials WHERE username = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        pstmt->setString(1, userCredentials.username);
        resultSet = pstmt->executeQuery();
        while (resultSet->next())
        {
            userCredentials.ID = (resultSet->getInt("user_ID"));
        }

    }catch(sql::SQLException &ex){
        std::cout << "user ID Exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    std::cout << "userID in storeUserID = " << userCredentials.ID << std::endl;
}

int databaseService::getSecretQuestionIndex(userCreds userCredentials)
{
    connection->setSchema("recLib");
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT sqQuestion from userCredentials WHERE username = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        pstmt->setString(1, userCredentials.username);
        resultSet = pstmt->executeQuery();
        while (resultSet->next())
        {
            userCredentials.sqIndex = (resultSet->getInt("sqQuestion"));
        }

    }catch(sql::SQLException &ex){
        std::cout << "user secret question Exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    std::cout << "user secret question index in getSecretQuestionAnswer = " << userCredentials.sqIndex << std::endl;

    return userCredentials.sqIndex;
}

//functions for collection manager
std::vector <std::string> databaseService::getLibNames()
{
    connection->setSchema("recLib");
    std::string listEntry;
    std::vector <std::string> list;
    int ID = sessionUserCredentials.ID;
    std::cout << "user ID in getLibNames = " << ID << std::endl;

    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("SELECT tableName FROM userLibs WHERE creator_ID = ?");
    sql::ResultSet *resultSet = NULL;

    try
    {
        pstmt->setInt(1, ID);
        resultSet = pstmt->executeQuery();
        while (resultSet->next())
        {
            listEntry = resultSet->getString("tableName");
            list.push_back(listEntry);
        }

    }catch(sql::SQLException &ex){
        std::cout << "getLibnames exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    return list;
}

bool databaseService::addNewLib(std::string libName)
{
    //create the new table
    connection->setSchema(schemaName);
    std::string tempLibName = "`" + libName + "`";
    sql::Statement *stmt = NULL;
    std::string statement = ("CREATE TABLE " + tempLibName +
             " (ID int NOT NULL PRIMARY KEY AUTO_INCREMENT, " +
             "bandName varChar(255), " +
             "albumTitle varChar(255), " +
             "genre varChar(255), " +
             "yearReleased varChar(255), " +
             "recLabel varChar(255))");
    try{

        stmt = connection->createStatement();
        stmt->executeUpdate(statement);
        connection->commit();

    }catch (sql::SQLException &ex) {
        std:: cout << "Add new table exception occured" << ex.getErrorCode();
        if (ex.getErrorCode() == 1050)
        {
            return false;
        }
    }

    stmt->close();
    delete stmt;

    //check if database already exists
    int count = 0;
    std::string query = "SELECT count(*) FROM information_schema.TABLES "
                        "WHERE (table_schema = '" + schemaName + "' )"
                        "AND (TABLE_NAME = \"" + libName + "\")";
    sql::Statement *stmt2 = connection->createStatement();
    std::cout << libName << std::endl;

    try {
        sql::ResultSet *rs = stmt2->executeQuery(query);
        while (rs->next())
        {
            count = rs->getInt("count(*)");
            std::cout << "Count = " << count << std::endl;
        }
    } catch (sql::SQLException &ex) {
        std::cout << "Check if new lib exception occured " << ex.getErrorCode();
    }

    if (count == 1)
    {
        connection->setSchema("recLib");
        sql::PreparedStatement *pstmt = connection->prepareStatement
                ("INSERT INTO userLibs ( creator_ID, tableName) "
                 "VALUES (?, ?)");
        try{
            pstmt->setInt(1, sessionUserCredentials.ID);
            pstmt->setString(2, libName);
            pstmt->execute();
            connection->commit();

        }catch (sql::SQLException &ex) {
            std::cout << "Add new lib exception occured" << ex.getErrorCode();
        }
        pstmt->close();
        delete pstmt;
    }

    if (count == 1)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void databaseService::storeTableName(std::string name)
{
    tableName = name;
}

void databaseService::deleteTable(std::string tableNameToDelete)
{
    connection->setSchema(schemaName);
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("DROP TABLE `" + tableNameToDelete + "`");
    pstmt->execute();
    connection->commit();
    pstmt->close();
    delete pstmt;
}

void databaseService::removeTableFromUserLibs(std::string tableNameToDelete)
{
    connection->setSchema("recLib");
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("DELETE FROM userLibs "
             "WHERE tableName = \"" + tableNameToDelete + "\" AND creator_ID = " + std::to_string(sessionUserCredentials.ID));
    pstmt->execute();
    connection->commit();
    pstmt->close();
    delete pstmt;
}

//functions for record manager
void databaseService::addNewRecordToDB(struct record recordEntry)
{
    connection->setSchema(schemaName);
    sql::PreparedStatement *addRecord = connection->prepareStatement
                ("INSERT INTO `" + tableName + "` ( bandName, albumTitle, genre, yearReleased, recLabel ) "
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
                    std::cout << "Add new record exception occured" << ex.getErrorCode();
        }
        addRecord->close();
    delete addRecord;
}

QList <record> databaseService::readRecordsFromDB()
{
    connection->setSchema(schemaName);
    sql::PreparedStatement *getAllRecords = connection->prepareStatement
            ("SELECT * FROM `" + tableName + "`");
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
    connection->setSchema(schemaName);
    sql::PreparedStatement *updateRecord = connection->prepareStatement
            ("UPDATE `" + tableName + "` SET bandName=?, albumTitle=?, genre=?, yearReleased=?, recLabel=? WHERE ID=?");
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
    connection->setSchema(schemaName);
    sql::PreparedStatement *deleteRecord = connection->prepareStatement
            ("DELETE FROM `" + tableName + "` WHERE ID=?");
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
    connection->setSchema(schemaName);
    int count = 0;
    std::string query = "SELECT ID FROM `" + tableName + "`";
    sql::Statement *stmt = connection->createStatement();
    sql::ResultSet *rs = stmt->executeQuery(query);

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

void databaseService::clearList()
{
    list.clear();
}
