#include "login.h"

Login::Login(QWidget *parent) : QDialog(parent)
{
	// *** Choose widget

	QIcon icon = parent->windowIcon();
	QSize size = icon.actualSize(QSize(64, 64));

	QLabel *iconLabel = new QLabel();
	iconLabel->setPixmap(icon.pixmap(size));

	QPushButton *chooseLogin = new QPushButton(tr("Use existing account"));
	connect(chooseLogin, SIGNAL(clicked()), SLOT(loginAction()));

	QPushButton *chooseRegister = new QPushButton(tr("Register new account"));
	connect(chooseRegister, SIGNAL(clicked()), SLOT(registerAction()));

	QVBoxLayout *chooseVLayout = new QVBoxLayout();
	chooseVLayout->addWidget(chooseLogin);
	chooseVLayout->addWidget(chooseRegister);

	QHBoxLayout *chooseHLayout = new QHBoxLayout();
	chooseHLayout->addWidget(iconLabel, 0, Qt::AlignTop);
	chooseHLayout->addLayout(chooseVLayout, 1);

	QWidget *chooseWidget = new QWidget(this);
	chooseWidget->setLayout(chooseHLayout);

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


void Login::registerAction()
{
	QDesktopServices::openUrl(QUrl("http://webpedia.altervista.org/registrazione.html"));
	stack->setCurrentIndex(1);
}
