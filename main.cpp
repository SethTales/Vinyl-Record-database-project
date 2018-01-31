
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"
#include "collectionmanager.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loginDialog dialog;
    dialog.show();
    //collectionManager manager;
    //manager.hide()
    return app.exec();

}

