
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    loginDialog dialog;

    if (dialog.exec() == 1)
    {
        dialog.close();
        recordManager recManager;
        recManager.show();
        return app.exec();
    }



}

