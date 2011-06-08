#include "wizard.h"

Wizard::Wizard(MainWindow *parent) : QDialog(parent)
{
	mainWindow = parent;

	setWindowTitle("Aggiungi una fonte");

	layout = new QStackedLayout();

	// 0) Scelta URL/Lista

	QWidget *modeWidget = new QWidget();
	QVBoxLayout *modeLayout = new QVBoxLayout();

	QPushButton *modeFromUrlButton = new QPushButton(tr("From URL"));
	this->connect(modeFromUrlButton, SIGNAL(clicked()), SLOT(step1a()));
	modeLayout->addWidget(modeFromUrlButton);

	QPushButton *modeFromListButton = new QPushButton(tr("From List"));
	this->connect(modeFromListButton, SIGNAL(clicked()), SLOT(stepListMode()));
	modeLayout->addWidget(modeFromListButton);

	modeWidget->setLayout(modeLayout);
	layout->addWidget(modeWidget);

	// 1) Inserimento da URL

	QWidget *urlWidget = new QWidget();
	QVBoxLayout *urlLayout = new QVBoxLayout();

	w10 = new QLineEdit("http://");
	urlLayout->addWidget(w10);

	QHBoxLayout *l11 = new QHBoxLayout();

	l11->addStretch(1);

	QPushButton *w13 = new QPushButton(tr("Back"));
	this->connect(w13, SIGNAL(clicked()), SLOT(step0()));
	l11->addWidget(w13);

	QPushButton *w12 = new QPushButton(tr("Next >"));
	this->connect(w12, SIGNAL(clicked()), SLOT(step1b()));
	l11->addWidget(w12);

	QPushButton *w14 = new QPushButton(QIcon(":/resources/cross.png"), "Cancel");
	this->connect(w14, SIGNAL(clicked()), SLOT(reject()));

	urlLayout->addLayout(l11);

	urlWidget->setLayout(urlLayout);
	layout->addWidget(urlWidget);

	// 2) Inserimento da Lista

	QVBoxLayout *listLayout = new QVBoxLayout();

	listTable = new QTableView();
	listTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	listTable->setSelectionMode(QAbstractItemView::SingleSelection);
	listTable->setModel(mainWindow->sourcesListModel);

	listLayout->addWidget(listTable);

	QDialogButtonBox *listButtonBox = new QDialogButtonBox();

	QPushButton *listOkButton = new QPushButton(tr("&Ok"));
	listOkButton->setDefault(true);
	listButtonBox->addButton(listOkButton, QDialogButtonBox::AcceptRole);
	connect(listButtonBox, SIGNAL(accepted()), SLOT(stepListAccept()));

	listLayout->addWidget(listButtonBox);

	QWidget *listWidget = new QWidget();
	listWidget->setLayout(listLayout);
	layout->addWidget(listWidget);

	// Finish

	setLayout(layout);

}


void Wizard::step0()
{
	layout->setCurrentIndex(0);
	if (exec() == QDialog::Accepted) {
		if (layout->currentIndex() == 1) {
			mainWindow->connection->addSource(w10->text());
		} else {
			// TODO
		}
	}
}


void Wizard::step1a()
{
	layout->setCurrentIndex(1);
}


void Wizard::step1b()
{
	accept();
}


void Wizard::stepListMode()
{
	mainWindow->connection->sourceList();
	layout->setCurrentIndex(2);
}


void Wizard::stepListAccept()
{

	QModelIndexList indexList = listTable->selectionModel()->selectedRows();

	if (indexList.length() == 1) {
		QModelIndex index = indexList[0];

		QStandardItem *item = mainWindow->sourcesListModel->itemFromIndex(index);

		mainWindow->connection->sourceAdd(item->data(Qt::UserRole + 1).toInt());

		accept();
	}

}
