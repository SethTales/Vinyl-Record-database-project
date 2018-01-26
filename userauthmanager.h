#ifndef USERAUTHMANAGER_H
#define USERAUTHMANAGER_H

#include <cppconn/connection.h>
#include <cppconn/driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <string>
#include "loginmanager.h"
#include "usercredentials.h"

class loginDialog;

class userAuthService
{
public:
    userAuthService();
    bool checkNewUserCredentials(userCreds);
    bool checkExistingUserCredentials(userCreds);
    bool addNewUser(userCreds);
private:
    userCreds userCredentials;
    sql::Driver *driver;
    sql::Connection *connection;
};

#endif // USERAUTHMANAGER_H
