
#include <QApplication>

#include "recordmanager.h"
#include "loginmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //recordManager recManager;
    loginDialog dialog;
    dialog.show();
    //recManager.show();
    return app.exec();
}
