
QT+= \
    widgets \
    sql \

DEFINES += QT_NO_VERSION_TAGGING

LIBS += -lGL
LIBS += -L/usr/lib/x86_64-linux-gnu -L/usr/local/mysql-connector-c++-1.1.9-linux-ubuntu16.04-x86-64bit/lib -lmysqlcppconn
INCLUDEPATH += -I/usr/local/include
               -I/usr/include/mysql
               -I/usr/include/boost
HEADERS+= \
    tablewidget.h \
    recordmanager.h \
    recordentryfordb.h \
    line_edit.h \
    dropdown.h \
    dbmanager.h \
    button.h \
    loginmanager.h \
    clickablebutton.h \
    userauthmanager.h \
    usercredentials.h \
    sha256.h


SOURCES += \
    tablewidget.cpp \
    recordmanager.cpp \
    main.cpp \
    line_edit.cpp \
    dropdown.cpp \
    dbmanager.cpp \
    button.cpp \
    loginmanager.cpp \
    clickablebutton.cpp \
    userauthmanager.cpp \
    sha256.cpp




