
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"

bool getLoggedInValue();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loginDialog dialog;
    dialog.exec();
    recordManager recManager;
    recManager.show();


    return app.exec();
}

bool getLoggedInValue()
{
    //loginDialog dialog;
    //if (dialog.getLoggedInValue() == true)
    //{
    //    recordManager recManager;
    //    recManager.show();
    //}
}
