#ifndef USERAUTH_H
#define USERAUTH_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <QMessageBox>
#include "userauthmanager.h"
#include "button.h"
#include "line_edit.h"
#include "clickablebutton.h"
#include "usercredentials.h"

class userAuthService;

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

private:
    //QLineEdit *usernamePtr, *passwordPtr;
    std::string username, password, reEnterPassword;
    userCreds userCredentials;
    bool newUser;

    Button *loginButton, *quitButton, *registerButton, *cancelButton;
    QLineEdit *usernameInput, *passwordInput, *reEnterPsswdInput;
    clickablePushButton *forgotPsswdBttn, *rgstrAsNewUserBttn;
    QGridLayout *dialogLayout;

    Button* createButton(const QString& text);
    QLineEdit *createDisplay(const QString& text);
    clickablePushButton *createLabel(const QString& text);

    std::string getPassword();
    std::string getUsername();
    std::string getReEnterPsswd();
    bool checkIfPsswdsMatch();

};

#endif // USERAUTH_H


