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
#include "button.h"
#include "line_edit.h"
#include "clickablebutton.h"
#include "usercredentials.h"
#include "sha256.h"
#include "dropdown.h"
#include "collectionmanager.h"
#include "dbmanager.h"

class loginDialog : public QDialog
{
    Q_OBJECT

public slots:
    void loginClicked();
    void quitClicked();
    void forgotPsswdClicked();
    void rgstrAsNewUserClicked();
    void registerClicked();
    void cancelRegisterClicked();
    void cancelSubmitRecoveryEmailClicked();
    void enterRecoveryEmailClicked();
    void cancelSubmitSecretQuestionAnswerClicked();
    void submitSecretQuestionAnswerClicked();
    void submitNewPasswordClicked();
    void cancelResetPasswordClicked();

public:
    loginDialog (databaseService&, QDialog* parent = 0);
    ~loginDialog();

    databaseService& _refToDBServeInLogin;
    bool loggedIn = false;

    int sendUserID();

private:

    std::string username, password, reEnterPassword, secretQAnswer;
    int secretQIndex;
    userCreds userCredentials;
    bool newUser;
    QStringList secretQuestionListTexts;
    QString secretQuestion;

    Button *loginButton, *quitButton, *registerButton, *cancelButton, *enterButton;
    QLineEdit *usernameInput, *passwordInput, *reEnterPsswdInput, *secretQuestionAnswer;
    clickablePushButton *forgotPsswdBttn, *rgstrAsNewUserBttn;
    dropDownMenu *secretQuestionList;
    QLabel *userSecretQuestion = nullptr;
    QGridLayout *dialogLayout;

    Button* createButton(const QString& text);
    QLineEdit *createDisplay(const QString& text);
    clickablePushButton *createLabel(const QString& text);
    dropDownMenu* createDropDown();

    std::string getPassword();
    std::string getUsername();
    std::string getReEnterPsswd();
    int getSecretQuestionIndex();
    std::string getSecretQuestionAnswer();
    int getUserID();
    bool checkIfPsswdsMatch();
    bool checkIfEmail();
    bool checkIfQuestionAnswer();
    void createSchema();

    void buildLoginDialog();
    void destroyLoginDialog();
    void buildRegisterDialog();
    void destroyRegisterDialog();
    void buildEnterRecoveryEmailDialog();
    void destroyEnterRecoveryEmailDialog();
    void buildSecretQuestionAnswerDialog();
    void destroySecretQuestionAnswerDialog();
    void buildResetPasswordDialog();
    void destroyResetPasswordDialog();

};

#endif // USERAUTH_H


