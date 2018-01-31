
#include "collectionselector.h"

collectionSelector::collectionSelector()
{
    std::cout << "collectionSelector constructor called" << std::endl;
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

std::vector <std::string> collectionSelector::getLibNames(int ID)
{
    std::string listEntry;
    std::vector <std::string> list;

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

