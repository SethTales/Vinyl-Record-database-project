#include "collectionmanager.h"

collectionManager::collectionManager(databaseService& _refToDBServInConst, QDialog *parent)
    : _refToDBServInCllctn(_refToDBServInConst), QDialog(parent)
{
    _refToDBServInCllctn = _refToDBServInConst;

    this->setFixedSize(950, 500);
    this->setWindowTitle("Select or Create Libraries");

    getUserLibs();

    collectionMgrLayout = new QGridLayout;

    newButton = createButton("Create");
    openButton = createButton("Open");
    deleteButton = createButton("Delete");

    newLibName = createDisplay("Library Name");

    libraries = createList(libNames);

    collectionMgrLayout->addWidget(newButton, 6, 26, 1, 5);
    collectionMgrLayout->addWidget(openButton, 1, 26, 1, 5);
    collectionMgrLayout->addWidget(deleteButton, 48, 26, 1, 5);

    QLabel *nameLabel = new QLabel;
    nameLabel->setText("Your Collections");
    collectionMgrLayout->addWidget(nameLabel, 0, 1, 1, 4);

    collectionMgrLayout->addWidget(newLibName, 6, 32, 1, 16);

    collectionMgrLayout->addWidget(libraries, 1, 0, 48, 48);

    setLayout(collectionMgrLayout);

    QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(newClicked()));
    QObject::connect(openButton, SIGNAL(clicked()), this, SLOT(openClicked()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));

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
            std::cout << "Error creating new lib" << std::endl;
        }
        else if (newLibCreated == true)
        {
            libNames.append(QString::fromStdString(name));
            libraries->addItem(QString::fromStdString(name));
        }
    }
}

void collectionManager::deleteClicked()
{

}

void collectionManager::getUserLibs()
{
    //creator_ID = dialog->sendUserID();
    //std::cout << "creator_ID = " << creator_ID << std::endl;
    libList = _refToDBServInCllctn.getLibNames();

    for (int i = 0; i < libList.size(); ++i)
    {
        (libNames.append(QString::fromStdString(libList.at(i))));
    }

}
