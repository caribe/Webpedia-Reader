#include "wizardlist.h"

WizardList::WizardList(QWidget *parent, QStandardItemModel *model) : QDialog(parent)
{
	setWindowTitle(tr("Add source"));
	setMinimumSize(320, 160);

	// Table

	listTable = new QTableView();
	listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	listTable->setSelectionMode(QAbstractItemView::SingleSelection);
	listTable->setModel(model);

	listTable->verticalHeader()->hide();
	listTable->verticalHeader()->setDefaultSectionSize(listTable->verticalHeader()->fontMetrics().height() + 3);
	listTable->setSortingEnabled(true);
	listTable->horizontalHeader()->setStretchLastSection(true);

	listTable->setAlternatingRowColors(true);
	listTable->setShowGrid(false);
	listTable->setStyleSheet("alternate-background-color: #ECF5D3");

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

	layout->addWidget(listTable);
	layout->addWidget(buttonBox);

	setLayout(layout);

}


int WizardList::getSource() {
	QModelIndexList indexList = listTable->selectionModel()->selectedRows();
	if (indexList.length() == 1) {
		return indexList.at(0).data(Qt::UserRole + 1).toInt();
	}

	return 0;
}
