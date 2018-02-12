#include "collectionmanager.h"

collectionManager::collectionManager(databaseService& _refToDBServInConst, QDialog *parent)
    : _refToDBServInCllctn(_refToDBServInConst), QDialog(parent)
{
    _refToDBServInCllctn = _refToDBServInConst;
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    this->setFixedSize(950, 500);
    this->setWindowTitle("Select or Create Libraries");

    getUserLibs();

    collectionMgrLayout = new QGridLayout;

    newButton = createButton("Create");
    openButton = createButton("Open");
    deleteButton = createButton("Delete");
    logoutButton = createButton("Logout");
    logoutButton->setFixedSize(60, 30);
    quitButton = createButton("Quit");
    quitButton->setFixedSize(60, 30);

    newLibName = createDisplay("Library Name");

    libraries = createList(libNames);
    libraries->setSelectionMode(QAbstractItemView::SingleSelection);

    collectionMgrLayout->addWidget(newButton, 6, 26, 1, 5);
    collectionMgrLayout->addWidget(openButton, 1, 26, 1, 5);
    collectionMgrLayout->addWidget(deleteButton, 48, 26, 1, 5);
    collectionMgrLayout->addWidget(logoutButton, 48, 36, 1, 5);
    collectionMgrLayout->addWidget(quitButton, 48, 42, 1, 5);

    QLabel *nameLabel = new QLabel;
    nameLabel->setText("Your Collections");
    collectionMgrLayout->addWidget(nameLabel, 0, 1, 1, 4);

    collectionMgrLayout->addWidget(newLibName, 6, 32, 1, 16);

    collectionMgrLayout->addWidget(libraries, 1, 0, 48, 48);

    setLayout(collectionMgrLayout);

    QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(newClicked()));
    QObject::connect(openButton, SIGNAL(clicked()), this, SLOT(openClicked()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
    QObject::connect(logoutButton, SIGNAL(clicked()), this, SLOT(logoutClicked()));

}

Button *collectionManager::createButton(const QString& text)
{
    Button *button = new Button(text);
    button->setFixedSize(80, 40);
    return button;
}

QLineEdit *collectionManager::createDisplay(const QString& text)
{
    QLineEdit *newLine = new QLineEdit();
    newLine->setPlaceholderText(text);
    QFont font = newLine->font();
    font.setPointSize(font.pointSize() + 4);
    newLine->setFont(font);
    return newLine;
}

QListWidget *collectionManager::createList(const QStringList& names)
{
    QListWidget *newList = new QListWidget;
    newList->addItems(names);
    newList->setFixedSize(500, 450);
    return newList;

}

collectionManager::~collectionManager()
{
    //delete selector;
}

void collectionManager::openClicked()
{

    std::string name = libraries->selectedItems().at(0)->text().toStdString();
    _refToDBServInCllctn.storeTableName(name);

    this->done(1);
}
void collectionManager::newClicked()
{
    bool newLibCreated = false;
    if (newLibName->text() == "")
    {
        QMessageBox messageBox;
        messageBox.setText("You must enter a name to create a new library!");
        messageBox.setWindowTitle("Error");
        messageBox.exec();
    }

    else
    {
        std::string name = newLibName->text().toStdString();
        newLibCreated = _refToDBServInCllctn.addNewLib(name);
        if (newLibCreated == false)
        {
            QMessageBox messageBox;
            messageBox.setText("Libary name is already in use.");
            messageBox.setInformativeText("Please choose another name for your collection.");
            messageBox.setWindowTitle("Error");
            messageBox.exec();
            newLibName->clear();
        }
        else if (newLibCreated == true)
        {
            libList.push_back(name);
            libNames.append(QString::fromStdString(name));
            libraries->addItem(QString::fromStdString(name));
            newLibName->clear();
        }
    }
}

void collectionManager::deleteClicked()
{
    std::string libNameToDelete = libraries->selectedItems().at(0)->text().toStdString();

    if (libNameToDelete == "")
    {
        QMessageBox messageBox;
        messageBox.setText("Select a collection to delete");
        messageBox.setWindowTitle("Error");
        messageBox.exec();
    }
    else
    {
        QMessageBox messageBox;
        messageBox.setIcon(QMessageBox::Warning);
        messageBox.setText("You are about permanently delete a record collection and all associated data.");
        messageBox.setInformativeText("Do you wish to proceed?");
        messageBox.setWindowTitle("WARNING");
        messageBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        int deleteBoxReturn = messageBox.exec();

        switch (deleteBoxReturn)
        {
        case QMessageBox::Cancel:
           {
               return;
           }
        case QMessageBox::Ok:
           {
               _refToDBServInCllctn.deleteTable(libNameToDelete);
               _refToDBServInCllctn.removeTableFromUserLibs(libNameToDelete);

               for (int i = 0; i < libList.size(); ++i)
               {
                   std::cout << "libList size = " << libList.size() << std::endl;
                   std::cout << "line 150" << std::endl;
                   if (libNameToDelete == libList.at(i))
                   {
                       std::cout << "libNameToDelete is: " << libNameToDelete << "and libList.at(i) is: " << libList.at(i) << std::endl;
                       std::vector<std::string>::iterator result = std::find(libList.begin(), libList.end(), libNameToDelete);
                       libList.erase(result);
                       std::cout << "libNames.at(i) is: " << libNames.at(i).toStdString() << std::endl;
                       libNames.removeAt(i);
                       std::cout << "selected item is: " << libraries->selectedItems().at(i)->text().toStdString() << std::endl;
                       qDeleteAll(libraries->selectedItems());
                   }
               }
           }
        }
    }
}

void collectionManager::logoutClicked()
{
    _refToDBServInCllctn.clearUserCredentials();
    this->done(2);
}

void collectionManager::quitClicked()
{
    this->done(0);
}

void collectionManager::getUserLibs()
{
    libList = _refToDBServInCllctn.getLibNames();

    for (int i = 0; i < libList.size(); ++i)
    {
        (libNames.append(QString::fromStdString(libList.at(i))));
    }

}
