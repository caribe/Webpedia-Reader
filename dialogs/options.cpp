#include "options.h"

Options::Options(QWidget *parent) : QDialog(parent) {

	setWindowIcon(QIcon(":resources/webpedia.png"));
	setWindowTitle(tr("Options"));

	QSettings settings;

	QVBoxLayout *layout = new QVBoxLayout();

	// TABS

	// General TAB

	QWidget *generalWidget = new QWidget();

	QFormLayout *generalLayout = new QFormLayout();

	generalUpdFreq = new QComboBox();
	generalUpdFreq->addItem(tr("15 Minutes"), 15);
	generalUpdFreq->addItem(tr("30 Minutes"), 30);
	generalUpdFreq->addItem(tr("1 Hour"), 60);
	generalUpdFreq->addItem(tr("2 Hours"), 120);
	generalUpdFreq->addItem(tr("4 Hours"), 240);
	generalUpdFreq->addItem(tr("Never"), -1);
	generalUpdFreq->setCurrentIndex(generalUpdFreq->findData(settings.value("settings/updateFrequency", 60)));
	generalLayout->addRow(tr("Update frequency:"), generalUpdFreq);

	generalNumber = new QComboBox();
	generalNumber->addItem(tr("10"), 10);
	generalNumber->addItem(tr("20"), 20);
	generalNumber->addItem(tr("30"), 30);
	generalNumber->addItem(tr("40"), 40);
	generalNumber->addItem(tr("50"), 50);
	generalNumber->addItem(tr("All"), -1);
	generalNumber->setCurrentIndex(generalNumber->findData(settings.value("settings/postsToDownload", 20)));
	generalLayout->addRow(tr("Posts to download:"), generalNumber);

	generalOrder = new QComboBox();
	generalOrder->addItem(tr("More recent first"), 1);
	generalOrder->addItem(tr("Less recent first"), -1);
	generalOrder->setCurrentIndex(generalOrder->findData(settings.value("settings/downloadOrder", -1)));
	generalLayout->addRow(tr("Download order:"), generalOrder);

	generalWidget->setLayout(generalLayout);

	// Putting things together

	QTabWidget *tab = new QTabWidget(this);
	tab->addTab(generalWidget, tr("General"));

	layout->addWidget(tab);

	// Buttons

	QPushButton *saveButton = new QPushButton(tr("&Save"));
	saveButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(tr("&Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(saveButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	layout->addWidget(buttonBox);

	connect(buttonBox, SIGNAL(accepted()), SLOT(acceptAction()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	// Finish

	setLayout(layout);

}



void Options::acceptAction()
{
	QSettings settings;

	settings.setValue("settings/updateFrequency", generalUpdFreq->itemData(generalUpdFreq->currentIndex()));
	settings.setValue("settings/postsToDownload", generalNumber->itemData(generalNumber->currentIndex()));
	settings.setValue("settings/downloadOrder", generalOrder->itemData(generalOrder->currentIndex()));

	accept();
}
