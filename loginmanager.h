#ifndef USERAUTH_H
#define USERAUTH_H

#include <QCoreApplication>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <QMessageBox>
#include <QComboBox>
#include "userauthmanager.h"
#include "button.h"
#include "line_edit.h"
#include "clickablebutton.h"
#include "usercredentials.h"
#include "sha256.h"
#include "dropdown.h"
#include "collectionmanager.h"

class userAuthService;
class dropDownMenu;
class collectionManager;

class loginDialog : public QDialog
{
    Q_OBJECT

public slots:
    void loginClicked();
    void quitClicked();
    void forgotPsswdClicked();
    void rgstrAsNewUserClicked();
    void registerClicked();
    void cancelClicked();

public:
    loginDialog (QDialog* parent = 0);
    ~loginDialog();

    userAuthService *authService;
    //collectionManager *mgr;
    bool loggedIn = false;

    int sendUserID();

private:

    std::string username, password, reEnterPassword, secretQAnswer;
    userCreds userCredentials;
    bool newUser;

    Button *loginButton, *quitButton, *registerButton, *cancelButton;
    QLineEdit *usernameInput, *passwordInput, *reEnterPsswdInput, *secretQuestionAnswer;
    clickablePushButton *forgotPsswdBttn, *rgstrAsNewUserBttn;
    dropDownMenu *secretQuestionList;
    QGridLayout *dialogLayout;

    Button* createButton(const QString& text);
    QLineEdit *createDisplay(const QString& text);
    clickablePushButton *createLabel(const QString& text);
    dropDownMenu* createDropDown();

    std::string getPassword();
    std::string getUsername();
    std::string getReEnterPsswd();
    std::string getSecretQuestionAnswer();
    bool checkIfPsswdsMatch();
    bool checkIfEmail();
    bool checkIfQuestionAnswer();

};

#endif // USERAUTH_H


