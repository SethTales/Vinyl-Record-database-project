
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"
#include "collectionmanager.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    int loginDialogCode, collectMgrCode, recManagerCode;
    databaseService dbService;
    databaseService& _refToDBService = dbService;

label1:
    loginDialog *dialog = new loginDialog(_refToDBService);
    dialog->show();
    loginDialogCode = dialog->exec();

    if (loginDialogCode == 1) //if user succesfully logs in
    {
        label2:
        collectionManager *collectMgr = new collectionManager(_refToDBService);
        collectMgr->show();
        collectMgrCode = collectMgr->exec();

        if (collectMgrCode == 1) //if user succesfully opens a library
        {
            recordManager *recManager = new recordManager(_refToDBService);
            recManager->show();
            recManagerCode = recManager->exec();
        }

        else if (collectMgrCode == 2) //if user logs out from collection manager
        {
            goto label1;
        }

        else if (collectMgrCode == 0) //if user quits from colelction manager
        {
            return 0;
        }

        if (recManagerCode == 1) //if user logs out from record manager
        {
            goto label1;
        }

        else if (recManagerCode == 2) //if user changes tables from record manager
        {
            goto label2;
        }
    }

    return 0;
}

