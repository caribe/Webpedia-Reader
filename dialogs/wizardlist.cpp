#include "wizardlist.h"

WizardList::WizardList(QWidget *parent, QStandardItemModel *model) : QDialog(parent)
{
	setWindowTitle(tr("Add source"));
	setMinimumSize(640, 480);

	// Table

	listTable = new QTableView(this);
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

	QPushButton *acceptButton = new QPushButton(QIcon(":/resources/plus.png"), tr("Add source"));
	acceptButton->setDefault(true);

	QPushButton *cancelButton = new QPushButton(tr("Cancel"));

	QDialogButtonBox *buttonBox = new QDialogButtonBox(Qt::Horizontal);
	buttonBox->addButton(acceptButton, QDialogButtonBox::AcceptRole);
	buttonBox->addButton(cancelButton, QDialogButtonBox::RejectRole);

	connect(buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), SLOT(reject()));

	// Tab bar

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	QButtonGroup *buttonsGroup = new QButtonGroup;
	buttonsGroup->setExclusive(true);

	QPushButton *buttonFollowed = new QPushButton(QIcon(":/resources/user.png"), tr("Most followed"));
	buttonFollowed->setCheckable(true);
	buttonsLayout->addWidget(buttonFollowed);
	buttonsGroup->addButton(buttonFollowed, 1);

	QPushButton *buttonLiked = new QPushButton(QIcon(":/resources/heart.png"), tr("Most liked"));
	buttonLiked->setCheckable(true);
	buttonsLayout->addWidget(buttonLiked);
	buttonsGroup->addButton(buttonLiked, 2);

	QPushButton *buttonRecent = new QPushButton(QIcon(":/resources/clock.png"), tr("Recent"));
	buttonRecent->setCheckable(true);
	buttonsLayout->addWidget(buttonRecent);
	buttonsGroup->addButton(buttonRecent, 3);

	QPushButton *buttonUpdated = new QPushButton(QIcon(":/resources/feed.png"), tr("Updated"));
	buttonUpdated->setCheckable(true);
	buttonsLayout->addWidget(buttonUpdated);
	buttonsGroup->addButton(buttonUpdated, 4);

	QPushButton *buttonSpecials = new QPushButton(QIcon(":/resources/star.png"), tr("Specials"));
	buttonSpecials->setCheckable(true);
	buttonsLayout->addWidget(buttonSpecials);
	buttonsGroup->addButton(buttonSpecials, 5);

	QPushButton *buttonSearch = new QPushButton(QIcon(":/resources/wrench.png"), tr("Search"));
	buttonSearch->setCheckable(true);
	buttonsLayout->addWidget(buttonSearch);
	buttonsGroup->addButton(buttonSearch, 6);

	buttonsLayout->addStretch(1);

	connect(buttonsGroup, SIGNAL(buttonClicked(int)), SLOT(buttonClicked(int)));
	buttonFollowed->click();

	QWidget *tabBar = new QWidget(this);
	tabBar->setLayout(buttonsLayout);

	// Layout

	QVBoxLayout *layout = new QVBoxLayout();

	layout->addWidget(tabBar);
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


void WizardList::buttonClicked(int id) {

	switch (id) {
	case 1:
		emit listRequest("list");
		break;
	case 2:
		emit listRequest("list/liked");
		break;
	case 3:
		emit listRequest("list/recent");
		break;
	case 4:
		emit listRequest("list/updated");
		break;
	case 5:
		emit listRequest("list/specials");
		break;
	}
}
