#include "collectionmanager.h"

collectionManager::collectionManager(QDialog *parent)
    : QDialog(parent)
{
    this->setFixedSize(950, 800);
    this->setWindowTitle("Select or Create Libraries");

    getUserLibs();

    collectionMgrLayout = new QGridLayout;

    newButton = createButton("Create");
    openButton = createButton("Open");
    deleteButton = createButton("Delete");

    libraries = createList(libNames);

    collectionMgrLayout->addWidget(newButton, 1, 42, 1, 5);
    collectionMgrLayout->addWidget(openButton, 3, 42, 1, 5);
    collectionMgrLayout->addWidget(deleteButton, 16, 42, 1, 5);

    QLabel *nameLabel = new QLabel;
    nameLabel->setText("Your Collections");
    collectionMgrLayout->addWidget(nameLabel, 0, 1, 1, 4);

    collectionMgrLayout->addWidget(libraries, 1, 0, 48, 48);

    setLayout(collectionMgrLayout);

    QObject::connect(newButton, SIGNAL(clicked()), this, SLOT(newClicked()));
    QObject::connect(openButton, SIGNAL(clicked()), this, SLOT(openClicked()));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(deleteClicked()));

    dialog->done(1);
    this->exec();


}

Button *collectionManager::createButton(const QString& text)
{
    Button *button = new Button(text);
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
    newList->setFixedSize(800, 750);
    return newList;

}

collectionManager::~collectionManager()
{
    //delete selector;
}

void collectionManager::openClicked()
{

}
void collectionManager::newClicked()
{

}

void collectionManager::deleteClicked()
{

}

void collectionManager::getUserLibs()
{
    creator_ID = dialog->sendUserID();
    std::cout << "creator_ID = " << creator_ID << std::endl;
    libList = selector.getLibNames(creator_ID);

    for (int i = 0; i < libList.size(); ++i)
    {
        (libNames.append(QString::fromStdString(libList.at(i))));
    }

}
