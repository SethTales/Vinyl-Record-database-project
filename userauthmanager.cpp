#include "userauthmanager.h"

userAuthService::userAuthService()
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

bool userAuthService::checkNewUserCredentials(userCreds userCredentials)
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

bool userAuthService::checkExistingUserCredentials(userCreds userCredentials)
{

}

bool userAuthService::addNewUser(userCreds userCredentials)
{
    sql::PreparedStatement *pstmt = connection->prepareStatement
            ("INSERT INTO userCredentials (username, password)"
             "VALUES (?, ?)");
    try{
        pstmt->setString(1, userCredentials.username);
        std::cout << userCredentials.username << std::endl;
        pstmt->setString(2, userCredentials.password);
        std::cout << userCredentials.password << std::endl;
        pstmt->execute();
        connection->commit();
    } catch (sql::SQLException &ex) {
        std::cout << "Add new user exception occured " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    delete pstmt;
}

bool userAuthService::login(userCreds userCredentials)
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
        connection->commit();
        resultCount = resultSet->rowsCount();

    }catch(sql::SQLException &ex){
        std::cout << "login exception occurred " << ex.getErrorCode() << std::endl;
    }

    pstmt->close();
    resultSet->close();
    delete pstmt;
    delete resultSet;

    if (resultCount == 1)
    {
        return true;
    }

    else
    {
        return false;
    }
}