#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QStackedLayout>
#include <QRadioButton>
#include <QDebug>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
	QLineEdit *loginUsername, *loginPassword;
	QLineEdit *registerUsername, *registerEmail;
	QCheckBox *registerCheckBox;
	QStackedLayout *stack;
	enum Forms { ChooseForm, LoginForm, RegisterForm };
	void setForm(Login::Forms form);

private:
	QRadioButton *chooseLogin;
	QRadioButton *chooseRegister;


signals:
	void loginRequest(const QString &username, const QString &password);
	void registerRequest(const QString &username, const QString &email);

private slots:
	void chooseAction();
	void loginAction();
	void registerAction();

public slots:

};

#endif // LOGIN_H
