
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"
#include "collectionmanager.h"
#include "dbmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    databaseService dbService;
    databaseService& _refToDBService = dbService;
    loginDialog *dialog = new loginDialog(_refToDBService);
    dialog->show();

    if (dialog->exec() == 1)
    {
        //delete dialog;
        collectionManager *collectMgr = new collectionManager(_refToDBService);
        collectMgr->show();

    //    if (collectMgr.exec() == 1)
    //    {
    //        recordManager recMgr(_refToDBService);
    //    }

    }

    return app.exec();

}

