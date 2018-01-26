
#include "loginmanager.h"

loginDialog::loginDialog(QDialog *parent)
    : QDialog(parent)
{
    int width = 280;
    int height = 200;
    resize(width, height);

    setWindowTitle("Returning User - AudioFile");

    dialogLayout = new QGridLayout;

    loginButton = createButton("Login");
    quitButton = createButton("Quit");
    registerButton = createButton("Register");
    cancelButton = createButton("Cancel");
    quitButton->setFixedSize(120, 40);
    loginButton->setFixedSize(120, 40);
    registerButton->setFixedSize(120, 40);
    cancelButton->setFixedSize(120, 40);

    usernameInput = createDisplay("Username");
    passwordInput = createDisplay("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    reEnterPsswdInput = createDisplay("Re-Enter Password");

    forgotPsswdBttn = createLabel("Forgot password?");
    rgstrAsNewUserBttn = createLabel("Register as new user");

    dialogLayout->addWidget(loginButton, 12, 1, 1, 3);
    dialogLayout->addWidget(quitButton, 12, 4, 1, 3);
    dialogLayout->setAlignment(quitButton, Qt::AlignRight);
    dialogLayout->addWidget(usernameInput, 1, 1, 1, 9);
    dialogLayout->addWidget(passwordInput, 5, 1, 1, 9);
    dialogLayout->addWidget(forgotPsswdBttn, 6, 1, 1, 2);
    dialogLayout->addWidget(rgstrAsNewUserBttn, 6, 5, 1, 2);

    setLayout(dialogLayout);

    QObject::connect(loginButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
    QObject::connect(forgotPsswdBttn, SIGNAL(clicked()), this, SLOT(forgotPsswdClicked()));
    QObject::connect(rgstrAsNewUserBttn, SIGNAL(clicked()), this, SLOT(rgstrAsNewUserClicked()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(registerButton, SIGNAL(clicked()), this, SLOT(registerClicked()));

    authService = new userAuthService;

}

loginDialog::~loginDialog()
{
    delete authService;
}

QLineEdit *loginDialog::createDisplay(const QString &text)
{
    QLineEdit *newLine = new QLineEdit();
    newLine->setPlaceholderText(text);
    QFont font = newLine->font();
    font.setPointSize(font.pointSize() + 4);
    newLine->setFont(font);
    return newLine;
}

Button *loginDialog::createButton(const QString &text)
{
    Button *button = new Button(text);
    return button;
}

clickablePushButton *loginDialog::createLabel(const QString &text)
{
    clickablePushButton *label = new clickablePushButton(text);
    return label;
}

std::string loginDialog::getPassword()
{
    password = passwordInput->text().toStdString();
    std::cout << "setPassword = " << password << std::endl;
    return password;
}

std::string loginDialog::getUsername()
{
    username = usernameInput->text().toStdString();
    std::cout << "setUsername = " << username << std::endl;
    return username;
}

std::string loginDialog::getReEnterPsswd()
{
    reEnterPassword = reEnterPsswdInput->text().toStdString();
    return reEnterPassword;
}

bool loginDialog::checkIfPsswdsMatch()
{
    if (getPassword() == getReEnterPsswd())
    {
        return true;
    }

    else
    {
        return false;
    }
}

void loginDialog::loginClicked()
{
    //string uName, pWord;
    //uName = getPassword().toStdString();
    //pWord = getUsername().toStdString();
}

void loginDialog::quitClicked()
{

}

void loginDialog::forgotPsswdClicked()
{

}

void loginDialog::rgstrAsNewUserClicked()
{
    usernameInput->clear();
    passwordInput->clear();

    std::cout << "rgstrAsNewUserClicked entered" << std::endl;
    dialogLayout->removeWidget(loginButton);
    delete loginButton;
    dialogLayout->removeWidget(quitButton);
    delete quitButton;
    dialogLayout->removeWidget(forgotPsswdBttn);
    delete forgotPsswdBttn;
    dialogLayout->removeWidget(rgstrAsNewUserBttn);
    delete rgstrAsNewUserBttn;

    registerButton = createButton("Register");
    cancelButton = createButton("Cancel");
    registerButton->setFixedSize(120, 40);
    cancelButton->setFixedSize(120, 40);
    reEnterPsswdInput = createDisplay("Re-Enter Password");
    reEnterPsswdInput->setEchoMode(QLineEdit::Password);

    dialogLayout->addWidget(registerButton, 12, 1, 1, 3);
    dialogLayout->addWidget(cancelButton, 12, 7, 1, 3);
    dialogLayout->setAlignment(cancelButton, Qt::AlignRight);
    dialogLayout->addWidget(reEnterPsswdInput, 9, 1, 1, 9);

    setWindowTitle("New User - AudioFile");

    setLayout(dialogLayout);

    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(registerButton, SIGNAL(clicked()), this, SLOT(registerClicked()));
}

void loginDialog::registerClicked()
{
    bool passwordsMatch = checkIfPsswdsMatch();

    if (passwordsMatch == true)
    {
        userCredentials.username = getUsername();
        userCredentials.password = getPassword();
        newUser = authService->checkNewUserCredentials(userCredentials);

        if (newUser == true)
        {
            authService->addNewUser(userCredentials);
            QMessageBox messageBox;
            messageBox.setWindowTitle("Success");
            messageBox.setText("Thank you for registering. Please login.");
            messageBox.exec();
            usernameInput->clear();
            passwordInput->clear();
            cancelClicked();
        }

        else if (newUser == false)
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle("Error");
            messageBox.setText("Username is already taken.");
            messageBox.exec();

        }
    }

    else if (passwordsMatch == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Error");
        messageBox.setText("Passwords do not match, please try again");
        messageBox.exec();
    }
}

void loginDialog::cancelClicked()
{
    usernameInput->clear();
    passwordInput->clear();

    std::cout << "cancelClicked entered " << std::endl;
    dialogLayout->removeWidget(registerButton);
    delete registerButton;
    dialogLayout->removeWidget(cancelButton);
    delete cancelButton;
    dialogLayout->removeWidget(reEnterPsswdInput);
    delete reEnterPsswdInput;

    loginButton = createButton("Login");
    quitButton = createButton("Quit");
    quitButton->setFixedSize(120, 40);
    loginButton->setFixedSize(120, 40);

    forgotPsswdBttn = createLabel("Forgot password?");
    rgstrAsNewUserBttn = createLabel("Register as new user");

    dialogLayout->addWidget(loginButton, 12, 1, 1, 3);
    dialogLayout->addWidget(quitButton, 12, 4, 1, 3);
    dialogLayout->setAlignment(quitButton, Qt::AlignRight);
    dialogLayout->addWidget(forgotPsswdBttn, 6, 1, 1, 2);
    dialogLayout->addWidget(rgstrAsNewUserBttn, 6, 5, 1, 2);

    setWindowTitle("Returning User - AudioFile");
    setLayout(dialogLayout);

    QObject::connect(rgstrAsNewUserBttn, SIGNAL(clicked()), this, SLOT(rgstrAsNewUserClicked()));

}


