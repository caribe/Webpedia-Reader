#include "login.h"

Login::Login(QWidget *parent) : QDialog(parent)
{
	// *** Webpedia icon

	QIcon icon = parent->windowIcon();
	QSize size = icon.actualSize(QSize(128, 128));

	// *** Choose widget

	QLabel *chooseLabel = new QLabel();
	chooseLabel->setPixmap(icon.pixmap(size));
	chooseLabel->setAlignment(Qt::AlignCenter);

	chooseLogin = new QRadioButton(tr("Use &existing account"), this);
	chooseRegister = new QRadioButton(tr("Register &new account"), this);

	QDialogButtonBox *buttonBox = new QDialogButtonBox();
	buttonBox->addButton(tr("&Next >"), QDialogButtonBox::AcceptRole);
	buttonBox->addButton(QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), SLOT(chooseAction()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	QVBoxLayout *chooseLayout = new QVBoxLayout();
	chooseLayout->addWidget(chooseLabel, 1);
	chooseLayout->addWidget(chooseLogin);
	chooseLayout->addWidget(chooseRegister);
	chooseLayout->addWidget(buttonBox);

	QWidget *chooseWidget = new QWidget();
	chooseWidget->setLayout(chooseLayout);


	// *** Login widget

	QLabel *loginLabel = new QLabel();
	loginLabel->setPixmap(icon.pixmap(size));
	loginLabel->setAlignment(Qt::AlignCenter);

	// Form

	QVBoxLayout *loginLayout = new QVBoxLayout();
	loginLayout->addWidget(loginLabel);

	QFormLayout *loginFormLayout = new QFormLayout();
	loginUsername = new QLineEdit();
	loginPassword = new QLineEdit();
	loginPassword->setEchoMode(QLineEdit::Password);

	loginFormLayout->addRow(tr("&Username:"), loginUsername);
	loginFormLayout->addRow(tr("&Password:"), loginPassword);

	loginLayout->addLayout(loginFormLayout);

	// Buttons

	QDialogButtonBox *loginButtonBox = new QDialogButtonBox();

	loginButtonBox->addButton(QDialogButtonBox::Ok);
	loginButtonBox->addButton(QDialogButtonBox::Cancel);
	connect(loginButtonBox, SIGNAL(accepted()), SLOT(loginAction()));
	connect(loginButtonBox, SIGNAL(rejected()), SLOT(reject()));

	loginLayout->addWidget(loginButtonBox);

	QWidget *loginWidget = new QWidget(this);
	loginWidget->setLayout(loginLayout);

	// *** Register widget

	// Form

	QFormLayout *registerForm = new QFormLayout();

	registerUsername = new QLineEdit();
	registerEmail = new QLineEdit();
	registerCheckBox = new QCheckBox(tr("I read and accepted the TOS"));

	registerForm->addRow(tr("Username"), registerUsername);
	registerForm->addRow(tr("eMail"), registerEmail);
	registerForm->addRow(registerCheckBox);

	// Buttons

	QDialogButtonBox *registerButtonBox = new QDialogButtonBox();

	registerButtonBox->addButton(QDialogButtonBox::Ok);
	registerButtonBox->addButton(QDialogButtonBox::Cancel);
	connect(registerButtonBox, SIGNAL(accepted()), SLOT(registerAction()));
	connect(registerButtonBox, SIGNAL(rejected()), SLOT(reject()));

	QVBoxLayout *registerLayout = new QVBoxLayout();
	registerLayout->addLayout(registerForm);
	registerLayout->addWidget(registerButtonBox);

	QWidget *registerWidget = new QWidget(this);
	registerWidget->setLayout(registerLayout);

	// *** Stack

	stack = new QStackedLayout();

	stack->addWidget(chooseWidget);
	stack->addWidget(loginWidget);
	stack->addWidget(registerWidget);

	stack->setCurrentIndex(0);

	resize(320, 240);
	setLayout(stack);
}


void Login::setForm(Login::Forms form)
{
	stack->setCurrentIndex(form);
}


void Login::chooseAction()
{
	if (chooseLogin->isChecked()) {
		setForm(LoginForm);
	} else if (chooseRegister->isChecked()) {
		setForm(RegisterForm);
	}
}


void Login::loginAction()
{
	QString username = loginUsername->text();
	QString password = loginPassword->text();

	if (username.isEmpty()) {
		QMessageBox::critical(this, tr("Login"), tr("You give no username"));
	} else if (password.isEmpty()) {
		QMessageBox::critical(this, tr("Login"), tr("You give no password"));
	} else {
		emit loginRequest(username, password);
		accept();
	}
}


void Login::registerAction()
{
	QString username = registerUsername->text();
	QString email = registerEmail->text();

	if (username.isEmpty()) {
		QMessageBox::critical(this, tr("Register"), tr("You give no username"));
	} else if (email.isEmpty()) {
		QMessageBox::critical(this, tr("Register"), tr("You give no email"));
	} else if (registerCheckBox->isChecked() == false) {
		QMessageBox::critical(this, tr("Register"), tr("You don't accepted the TOS"));
	} else {
		emit registerRequest(username, email);
		accept();
	}
}
