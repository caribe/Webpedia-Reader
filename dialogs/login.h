#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QStackedLayout>

class Wizard;

#include "mainwindow.h"

class Login : public QDialog
{
    Q_OBJECT
public:
    explicit Login(QWidget *parent = 0);
	QLineEdit *username;
	QLineEdit *password;
	QStackedLayout *stack;

private:

signals:

private slots:
	void loginAction();

public slots:

};

#endif // LOGIN_H
