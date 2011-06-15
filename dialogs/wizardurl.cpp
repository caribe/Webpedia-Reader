#include "wizardurl.h"

WizardUrl::WizardUrl(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("Add source by URL"));
	setMinimumSize(320, 160);

	// Form

	QFormLayout *formLayout = new QFormLayout();

	urlLineEdit = new QLineEdit("http://");
	formLayout->addRow(tr("URL:"), urlLineEdit);

	// Button Box

	QPushButton *acceptButton = new QPushButton(tr("Add source"));
	acceptButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(acceptButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	// Layout

	QVBoxLayout *layout = new QVBoxLayout();

	layout->addLayout(formLayout, 1);
	layout->addWidget(buttonBox);

	setLayout(layout);
}


QString WizardUrl::getUrl() {
	return urlLineEdit->text();
}
