#ifndef COLLECTIONSELECTOR_H
#define COLLECTIONSELECTOR_H

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include <vector>
#include "usercredentials.h"
#include "dbmanager.h"

class collectionSelector
{
public:
    collectionSelector();
    std::vector <std::string> getLibNames(int);

private:
    int ID;
    std::vector <std::string> libNames;
    sql::Driver *driver;
    sql::Connection *connection;
};

#endif // COLLECTIONSELECTOR_H
