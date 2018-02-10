#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "recordentryfordb.h"
#include "recordmanager.h"
#include "usercredentials.h"

#include <QList>
#include <QString>
#include <vector>
#include <string>
#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

class recordManager;

class databaseService
{
public:
    databaseService();

    //functions for login manager
    bool checkNewUserCredentials(userCreds);
    bool addNewUser(userCreds);
    int getUserID(userCreds);
    void createSchema(userCreds);
    bool login(userCreds);
    void storeUserID(userCreds);

    //functions for collection manager
    std::vector <std::string> getLibNames();
    bool addNewLib(std::string);
    void storeTableName(std::string);
    void deleteTable(std::string);
    void removeTableFromUserLibs(std::string);

    //functions for record manager
    void addNewRecordToDB(struct record);
    QList<record> readRecordsFromDB();
    void updateRecordInDB(struct record);
    void deleteRecordFromDB(int);
    bool isTableEmpty();



private:
    sql::Driver *driver;
    sql::Connection *connection;
    sql::ConnectOptionsMap connection_properties;

    //member variables for login manager
    userCreds sessionUserCredentials;
    std::string schemaName;

    //member variables for collection manager
    //int ID;
    std::vector <std::string> libNames;
    std::string tableName;

    //member variables for record manager
    QList <record> list;
    record recordEntry;

};

#endif // DBMANAGER_H
