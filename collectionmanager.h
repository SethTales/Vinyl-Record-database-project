#ifndef COLLECTIONMANAGER_H
#define COLLECTIONMANAGER_H

#include "QDialog"
#include "QListWidget"
#include "button.h"
#include "line_edit.h"
#include "QDialog"
#include "QLabel"
#include "QGridLayout"
#include "QWidget"
#include "usercredentials.h"
#include "collectionselector.h"
#include "loginmanager.h"

class collectionSelector;
class loginDialog;

class collectionManager : public QDialog
{
    Q_OBJECT

public slots:
    void openClicked();
    void newClicked();
    void deleteClicked();
public:
    collectionManager (QDialog* parent = 0);
    ~collectionManager();

    collectionSelector selector;
    loginDialog *dialog;

    void getCreatorID(userCreds);

private:
    std::vector<std::string> libList;
    QList<QString> libNames;
    int creator_ID;

    Button *openButton, *newButton, *deleteButton;
    QListWidget *libraries;
    QDialog *newLibDialog, *deleteLibDialog;
    QLineEdit *newLibName;
    QGridLayout *collectionMgrLayout;

    Button *createButton(const QString& text);
    QLineEdit *createDisplay(const QString& text);
    QListWidget *createList(const QStringList& names);

    void getUserLibs();


};


#endif // COLLECTIONMANAGER_H
