#include "login.h"

Login::Login(QWidget *parent) : QDialog(parent)
{
	// *** Choose widget

	QVBoxLayout *chooseLayout = new QVBoxLayout();

	QPushButton *chooseLogin = new QPushButton(tr("Use existing account"));
	connect(chooseLogin, SIGNAL(clicked()), SLOT(loginAction()));
	chooseLayout->addWidget(chooseLogin);

	QPushButton *chooseRegister = new QPushButton(tr("Register new account"));
	chooseLayout->addWidget(chooseRegister);

	QWidget *chooseWidget = new QWidget(this);
	chooseWidget->setLayout(chooseLayout);

	// *** Login widget

	QVBoxLayout *loginLayout = new QVBoxLayout();

	// Form

	QFormLayout *loginFormLayout = new QFormLayout();

	username = new QLineEdit();
	password = new QLineEdit();
	password->setEchoMode(QLineEdit::Password);

	loginFormLayout->addRow(tr("&Username:"), username);
	loginFormLayout->addRow(tr("&Password:"), password);

	loginLayout->addLayout(loginFormLayout);

	// Buttons

	QDialogButtonBox *loginGroup = new QDialogButtonBox();

	loginGroup->addButton(tr("OK"), QDialogButtonBox::AcceptRole);
	loginGroup->addButton(tr("Cancel"), QDialogButtonBox::RejectRole);

	this->connect(loginGroup, SIGNAL(accepted()), SLOT(accept()));
	this->connect(loginGroup, SIGNAL(rejected()), SLOT(reject()));

	loginLayout->addWidget(loginGroup);

	QWidget *loginWidget = new QWidget(this);
	loginWidget->setLayout(loginLayout);

	// *** Stack

	stack = new QStackedLayout();

	stack->addWidget(chooseWidget);
	stack->addWidget(loginWidget);

	stack->setCurrentIndex(0);

	setLayout(stack);
	resize(320, 240);
}


void Login::loginAction() {
	stack->setCurrentIndex(1);
}
