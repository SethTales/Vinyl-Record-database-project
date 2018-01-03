
#include <QApplication>

#include "recordmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    recordManager recManager;
    recManager.show();
    return app.exec();
}
