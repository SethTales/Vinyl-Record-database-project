#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "recordentryfordb.h"
#include "recordmanager.h"

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
    void addNewRecordToDB(struct record);
    QList<record> readRecordsFromDB();
    void updateRecordInDB(struct record);
    void deleteRecordsFromDB(QList <record>);

private:

    QList <record> list;
    record recordEntry;
    sql::Driver *driver;
    sql::Connection *connection;
};

#endif // DBMANAGER_H
