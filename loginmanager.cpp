
#include "loginmanager.h"

loginDialog::loginDialog(databaseService& _refToDBServInConst, QDialog *parent)
    : _refToDBServeInLogin(_refToDBServInConst), QDialog(parent)
{
    _refToDBServeInLogin = _refToDBServInConst;
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    this->setModal(true);
    this->setFixedSize(450, 250);

    setWindowTitle("Returning User - AudioFile");

    dialogLayout = new QGridLayout;

    loginButton = createButton("Login");
    quitButton = createButton("Quit");
    registerButton = createButton("Register");
    cancelButton = createButton("Cancel");
    cancelPwordResetButton = createButton("Cancel");
    enterButton= createButton("Enter");
    quitButton->setFixedSize(120, 40);
    loginButton->setFixedSize(120, 40);
    registerButton->setFixedSize(120, 40);
    cancelButton->setFixedSize(120, 40);
    cancelPwordResetButton->setFixedSize(120, 40);
    enterButton->setFixedSize(120, 40);

    usernameInput = createDisplay("Email");
    passwordInput = createDisplay("Password");
    passwordInput->setEchoMode(QLineEdit::Password);
    reEnterPsswdInput = createDisplay("Re-Enter Password");

    forgotPsswdBttn = createLabel("Forgot password?");
    rgstrAsNewUserBttn = createLabel("Register as new user");

    dialogLayout->addWidget(loginButton, 12, 1, 1, 3);
    dialogLayout->addWidget(quitButton, 12, 6, 1, 3);
    dialogLayout->setAlignment(quitButton, Qt::AlignRight);
    dialogLayout->addWidget(usernameInput, 1, 1, 1, 9);
    dialogLayout->addWidget(passwordInput, 5, 1, 1, 9);
    dialogLayout->addWidget(forgotPsswdBttn, 6, 1, 1, 2);
    dialogLayout->addWidget(rgstrAsNewUserBttn, 6, 7, 1, 2);

    setLayout(dialogLayout);

    QObject::connect(loginButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
    QObject::connect(forgotPsswdBttn, SIGNAL(clicked()), this, SLOT(forgotPsswdClicked()));
    QObject::connect(rgstrAsNewUserBttn, SIGNAL(clicked()), this, SLOT(rgstrAsNewUserClicked()));
    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(registerButton, SIGNAL(clicked()), this, SLOT(registerClicked()));
    QObject::connect(cancelPwordResetButton, SIGNAL(clicked()), this, SLOT(cancelPwordResetClicked()));
    //QObject::connect(enterButton, SIGNAL(clicked()), this, SLOT(c))

    secretQuestionListTexts = (QStringList() << "What is your mother's maiden name?" << "What is the name of your first pet?" <<
                                   "What is the name of your first school?" << "What is your favorite book?" <<
                                   "What is the name of your childhood best friend?" << "What is your favorite record?");

}

loginDialog::~loginDialog()
{

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

dropDownMenu *loginDialog::createDropDown()
{
    //QStringList secretQuestions = (QStringList() << "What is your mother's maiden name?" << "What is the name of your first pet?" <<
                                // "What is the name of your first school?" << "What is your favorite book?" <<
                                // "What is the name of your childhood best friend?" << "What is your favorite record?");

    dropDownMenu *menu = new dropDownMenu(secretQuestionListTexts);
    return menu;
}

int loginDialog::sendUserID()
{
    return userCredentials.ID;
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

int loginDialog::getSecretQuestionIndex()
{
    secretQIndex = secretQuestionList->currentIndex();
    return secretQIndex;
}

std::string loginDialog::getSecretQuestionAnswer()
{
    secretQAnswer = secretQuestionAnswer->text().toStdString();
    return secretQAnswer;
}

int loginDialog::getUserID()
{
    userCredentials.ID = _refToDBServeInLogin.getUserID(userCredentials);
    std::cout << "userID in getUserId = " << userCredentials.ID << std::endl;
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

bool loginDialog::checkIfEmail()
{
    std::string uName = getUsername();
    std::string emailDomain;
    int atSignIndex;
    bool atSign = false;
    bool period = false;

    //check if there is an '@' in the username
    for (int i = 0; i < uName.length(); ++i)
    {
        if (uName[i] == '@')
        {
            atSign = true;
            atSignIndex = i;
            break;
        }
    }

    //extract email domain from username
    for (int i = atSignIndex; i < uName.length(); ++i)
    {
        emailDomain += uName[i];
    }

    //check if domain is real url
    for (int i = 0; i < emailDomain.length(); ++i)
    {
        if (emailDomain[i] == '.')
        {
            period = true;
            break;
        }
    }

    if (period == true && atSign == true)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool loginDialog::checkIfQuestionAnswer()
{
    if (getSecretQuestionAnswer() == "")
    {
        return false;
    }

    else
    {
        return true;
    }
}

void loginDialog::loginClicked()
{
    userCredentials.username = getUsername();
    userCredentials.password = sha256(getPassword());
    loggedIn = _refToDBServeInLogin.login(userCredentials);

    if (loggedIn == true)
    {
        _refToDBServeInLogin.storeUserID(userCredentials);
        this->done(1);
    }

    else if (loggedIn == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Login Attempt Failed");
        messageBox.setText("Cannot login. Please check username and password or register as a new user.");
        messageBox.exec();
    }
}

void loginDialog::quitClicked()
{
    this->close();
}

void loginDialog::forgotPsswdClicked()
{
    usernameInput->clear();
    passwordInput->clear();

    dialogLayout->removeWidget(loginButton);
    delete loginButton;
    dialogLayout->removeWidget(quitButton);
    delete quitButton;
    dialogLayout->removeWidget(forgotPsswdBttn);
    delete forgotPsswdBttn;
    dialogLayout->removeWidget(rgstrAsNewUserBttn);
    delete rgstrAsNewUserBttn;
    dialogLayout->removeWidget(passwordInput);
    delete passwordInput;
    passwordInput = nullptr;

    if (cancelPwordResetButton == NULL)
    {
        cancelPwordResetButton = createButton("Cancel");
        cancelPwordResetButton->setFixedSize(120, 40);
    }

    if (enterButton == NULL)
    {
        enterButton = createButton("Enter");
        enterButton->setFixedSize(120, 40);
    }

    //secretQuestionAnswer = createDisplay("Secret Question Answer");
    //userSecretQuestion = new QLabel;

    dialogLayout->addWidget(cancelPwordResetButton, 12, 7, 1, 3);
    dialogLayout->setAlignment(cancelPwordResetButton, Qt::AlignRight);
    dialogLayout->addWidget(enterButton, 12, 0, 1, 3);
    dialogLayout->setAlignment(enterButton, Qt::AlignLeft);
    //dialogLayout->addWidget(secretQuestionAnswer, 11, 1, 1, 9);
    //dialogLayout->addWidget(userSecretQuestion, 10, 1, 1, 9);

    setWindowTitle("Password Reset - AudioFile");

    setLayout(dialogLayout);

    QObject::connect(cancelPwordResetButton, SIGNAL(clicked()), this, SLOT(cancelPwordResetClicked()));
    QObject::connect(enterButton, SIGNAL(clicked()), this, SLOT(enterRecoveryEmailClicked()));

}

void loginDialog::enterRecoveryEmailClicked()
{

    QString secretQuestion;
    userCredentials.username = usernameInput->text().toStdString();
    userCredentials.sqIndex = _refToDBServeInLogin.getSecretQuestionIndex(userCredentials);
    secretQuestion = secretQuestionListTexts.at(userCredentials.sqIndex);
    userSecretQuestion = new QLabel;
    userSecretQuestion->setText(secretQuestion);
    dialogLayout->addWidget(userSecretQuestion, 6, 2, 1, 9);

    dialogLayout->removeWidget(usernameInput);
    delete usernameInput;
    usernameInput = NULL;

    secretQuestionAnswer = createDisplay("Secret Question Answer");
    dialogLayout->addWidget(secretQuestionAnswer, 9, 2, 1, 9);
}

void loginDialog::submitSecretQuestionAnswerClicked()
{

}

void loginDialog::rgstrAsNewUserClicked()
{
    usernameInput->clear();
    passwordInput->clear();

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
    secretQuestionAnswer = createDisplay("Secret Question Answer");
    secretQuestionList = createDropDown();

    dialogLayout->addWidget(registerButton, 12, 1, 1, 3);
    dialogLayout->addWidget(cancelButton, 12, 7, 1, 3);
    dialogLayout->setAlignment(cancelButton, Qt::AlignRight);
    dialogLayout->addWidget(reEnterPsswdInput, 9, 1, 1, 9);
    dialogLayout->addWidget(secretQuestionAnswer, 11, 1, 1, 9);
    dialogLayout->addWidget(secretQuestionList, 10, 1, 1, 9);

    setWindowTitle("New User - AudioFile");

    setLayout(dialogLayout);

    QObject::connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    QObject::connect(registerButton, SIGNAL(clicked()), this, SLOT(registerClicked()));
    //QObject::connect(forgotPsswdBttn, SIGNAL(clicked()), this, SLOT(forgotPsswdClicked()));
}

void loginDialog::registerClicked()
{
    bool isEmail = checkIfEmail();
    bool passwordsMatch = checkIfPsswdsMatch();
    bool isTextInQuestion = checkIfQuestionAnswer();

    if (isEmail == true && passwordsMatch == true && isTextInQuestion == true)
    {
        userCredentials.username = getUsername();
        userCredentials.password = sha256(getPassword());
        userCredentials.sqIndex = getSecretQuestionIndex();
        userCredentials.sqAnswer = sha256(getSecretQuestionAnswer());
        newUser = _refToDBServeInLogin.checkNewUserCredentials(userCredentials);

        if (newUser == true)
        {
            _refToDBServeInLogin.addNewUser(userCredentials);
            createSchema();
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

    else if (isEmail == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Error");
        messageBox.setText("Please enter a valid email");
        messageBox.exec();
    }

    else if (passwordsMatch == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Error");
        messageBox.setText("Passwords do not match, please try again");
        messageBox.exec();
    }

    else if (isTextInQuestion == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowTitle("Error");
        messageBox.setText("Please enter an answer to the secret question");
        messageBox.exec();
    }
}

void loginDialog::createSchema()
{
    userCredentials.ID = _refToDBServeInLogin.getUserID(userCredentials);
    std::cout << "userCreds.ID in createSchema loginDialog = " << userCredentials.ID << std::endl;
    _refToDBServeInLogin.createSchema(userCredentials);
}

void loginDialog::cancelClicked()
{
    usernameInput->clear();
    passwordInput->clear();

    dialogLayout->removeWidget(registerButton);
    delete registerButton;
    registerButton = NULL;
    dialogLayout->removeWidget(cancelButton);
    delete cancelButton;
    cancelButton = NULL;
    dialogLayout->removeWidget(reEnterPsswdInput);
    delete reEnterPsswdInput;
    reEnterPsswdInput = NULL;
    dialogLayout->removeWidget(secretQuestionAnswer);
    delete secretQuestionAnswer;
    secretQuestionAnswer = NULL;
    dialogLayout->removeWidget(secretQuestionList);
    delete secretQuestionList;
    secretQuestionList = NULL;

    loginButton = createButton("Login");
    quitButton = createButton("Quit");
    quitButton->setFixedSize(120, 40);
    loginButton->setFixedSize(120, 40);

    forgotPsswdBttn = createLabel("Forgot password?");
    rgstrAsNewUserBttn = createLabel("Register as new user");

    dialogLayout->addWidget(loginButton, 12, 1, 1, 3);
    dialogLayout->addWidget(quitButton, 12, 6, 1, 3);
    dialogLayout->setAlignment(quitButton, Qt::AlignRight);
    dialogLayout->addWidget(forgotPsswdBttn, 6, 1, 1, 2);
    dialogLayout->addWidget(rgstrAsNewUserBttn, 6, 7, 1, 2);

    setWindowTitle("Returning User - AudioFile");
    setLayout(dialogLayout);

    QObject::connect(rgstrAsNewUserBttn, SIGNAL(clicked()), this, SLOT(rgstrAsNewUserClicked()));
    QObject::connect(loginButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
    QObject::connect(forgotPsswdBttn, SIGNAL(clicked()), this, SLOT(forgotPsswdClicked()));
}

void loginDialog::cancelPwordResetClicked()
{
    if (userSecretQuestion != NULL)
    {
        dialogLayout->removeWidget(userSecretQuestion);
        delete userSecretQuestion;
        userSecretQuestion = NULL;
    }

    if (passwordInput != NULL)
    {
        dialogLayout->removeWidget(passwordInput);
        delete passwordInput;
        passwordInput = NULL;
    }

    if (reEnterPsswdInput != NULL)
    {
        dialogLayout->removeWidget(reEnterPsswdInput);
        delete reEnterPsswdInput;
        reEnterPsswdInput = NULL;
    }

    if (usernameInput != NULL)
    {
        dialogLayout->removeWidget(usernameInput);
        delete usernameInput;
        usernameInput = NULL;
    }

    dialogLayout->removeWidget(enterButton);
    delete enterButton;
    enterButton = NULL;
    dialogLayout->removeWidget(cancelPwordResetButton);
    delete cancelPwordResetButton;
    cancelPwordResetButton = NULL;

    loginButton = createButton("Login");
    quitButton = createButton("Quit");
    quitButton->setFixedSize(120, 40);
    loginButton->setFixedSize(120, 40);

    usernameInput = createDisplay("Email");
    passwordInput = createDisplay("Password");
    passwordInput->setEchoMode(QLineEdit::Password);

    forgotPsswdBttn = createLabel("Forgot password?");
    rgstrAsNewUserBttn = createLabel("Register as new user");

    dialogLayout->addWidget(usernameInput, 1, 1, 1, 9);
    dialogLayout->addWidget(passwordInput, 5, 1, 1, 9);
    dialogLayout->addWidget(loginButton, 12, 1, 1, 3);
    dialogLayout->addWidget(quitButton, 12, 6, 1, 3);
    dialogLayout->setAlignment(quitButton, Qt::AlignRight);
    dialogLayout->addWidget(forgotPsswdBttn, 6, 1, 1, 2);
    dialogLayout->addWidget(rgstrAsNewUserBttn, 6, 7, 1, 2);

    setWindowTitle("Returning User - AudioFile");
    setLayout(dialogLayout);

    QObject::connect(rgstrAsNewUserBttn, SIGNAL(clicked()), this, SLOT(rgstrAsNewUserClicked()));
    QObject::connect(loginButton, SIGNAL(clicked()), this, SLOT(loginClicked()));
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quitClicked()));
    QObject::connect(forgotPsswdBttn, SIGNAL(clicked()), this, SLOT(forgotPsswdClicked()));
}


