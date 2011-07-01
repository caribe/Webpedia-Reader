#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QStackedLayout>
#include <QDebug>

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
	void registerAction();

public slots:

};

#endif // LOGIN_H
