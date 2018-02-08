
#include "dbmanager.h"

#include <QString>
#include <vector>
#include <iostream>

databaseService::databaseService()
{
    try
    {
        driver = get_driver_instance();
        connection = driver->connect("host", "user", "password");
        connection->setSchema("recLib");
        connection->setAutoCommit(false);
    }catch(sql::SQLException &ex){
        std::cout << "Connection exception occured " << ex.getErrorCode() << std::endl;
    }

}

//functions for login manager
bool databaseService::checkNewUserCredentials(userCreds userCredentials)
{
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
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("INSERT INTO userCredentials (username, password, sqAnswer)"
             "VALUES (?, ?, ?)");
    try{
        pstmt->setString(1, userCredentials.username);
        std::cout << userCredentials.username << std::endl;
        pstmt->setString(2, userCredentials.password);
        std::cout << userCredentials.password << std::endl;
        pstmt->setString(3, userCredentials.sqAnswer);
        pstmt->execute();
        connection->commit();
    } catch (sql::SQLException &ex) {
        std::cout << "Add new user exception occured " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    delete pstmt;
}

bool databaseService::login(userCreds userCredentials)
{
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

//functions for collection manager
std::vector <std::string> databaseService::getLibNames()
{
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
    }

    stmt->close();
    delete stmt;


    int count;
    std::string query = "SELECT count(*) FROM information_schema.TABLES "
                        "WHERE (table_schema = 'recLib') "
                        "AND (TABLE_NAME = '" + libName + "')";
    sql::Statement *stmt2 = connection->createStatement();

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

//functions for record manager
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
                std::cout << "Add new record exception occured" << ex.getErrorCode();
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
