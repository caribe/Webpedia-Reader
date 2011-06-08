#include "sourceframe.h"

SourceFrame::SourceFrame(MainWindow *parent) : QTreeView(parent)
{
	mainWindow = parent;

	setContextMenuPolicy(Qt::DefaultContextMenu);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);

	setDragEnabled(true);
	setAcceptDrops(true);
	setDropIndicatorShown(true);

	contextMenu = new QMenu(this);

	QAction *menuRename = new QAction(QIcon(":/resources/pencil.png"), tr("&Rename"), this);
	connect(menuRename, SIGNAL(triggered()), SLOT(renameAction()));
	contextMenu->addAction(menuRename);

	QAction *menuDelete = new QAction(QIcon(":/resources/cross.png"), tr("&Delete"), this);
	menuDelete->setShortcut(QKeySequence::Delete);
	connect(menuDelete, SIGNAL(triggered()), SLOT(menuDeleteAction()));
	contextMenu->addAction(menuDelete);

	contextMenu->addSeparator();

	QAction *menuUpdate = new QAction(QIcon(":/resources/arrow-circle.png"), tr("&Update all sources"), this);
	contextMenu->addAction(menuUpdate);

	menuAddSeparator = new QAction(this);
	menuAddSeparator->setSeparator(true);

	menuAdd = new QAction(QIcon(":/resources/feed-plus.png"), tr("&Add source"), this);
	contextMenu->addAction(menuAdd);

	menuAddFolder = new QAction(QIcon(":/resources/folder--plus.png"), tr("Add &folder"), this);
	connect(menuRename, SIGNAL(triggered()), SLOT(addFolderAction()));
	contextMenu->addAction(menuAddFolder);

	autoExpandFlag = false;
	connect(this, SIGNAL(expanded(const QModelIndex &)), SLOT(folderExpand(const QModelIndex &)));
	connect(this, SIGNAL(collapsed(const QModelIndex &)), SLOT(folderCollapse(const QModelIndex &)));

}


void SourceFrame::contextMenuEvent(QContextMenuEvent *event) {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() == 1) {
		Source *source = static_cast<Source *>(indexList.at(0).internalPointer());

		if (source->feed == 0) {
			menuAddSeparator->setVisible(true);
			menuAdd->setVisible(true);
			menuAddFolder->setVisible(true);
		} else {
			menuAddSeparator->setVisible(false);
			menuAdd->setVisible(false);
			menuAddFolder->setVisible(false);
		}

		contextMenu->exec(event->globalPos());
	}
}


void SourceFrame::menuDeleteAction() {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() == 1) {
		Source *source = static_cast<Source *>(indexList.at(0).internalPointer());
		mainWindow->connection->sourceDel(source);
	}
}

void SourceFrame::renameAction() {
	QModelIndexList indexList = selectionModel()->selectedRows();
	if (indexList.length() == 1) {
		openPersistentEditor(indexList.at(0));
	}
}


void SourceFrame::addFolderAction()
{
	QModelIndexList indexList = selectionModel()->selectedRows();
	if (indexList.length() == 1) {
		Source *source = static_cast<Source *>(indexList.at(0).internalPointer());
		if (source->feed == 0) {
			emit addFolderSignal(source);
		}
	}
}


void SourceFrame::autoExpandItems()
{
	autoExpandFlag = true;
	QAbstractItemModel *theModel = model();
	autoExpandItemsFunc(theModel);
	autoExpandFlag = false;
}


void SourceFrame::autoExpandItemsFunc(QAbstractItemModel *theModel, const QModelIndex &index)
{
	int rows = theModel->rowCount(index);

	for (int i = 0; i < rows; i++) {
		QModelIndex child = theModel->index(i, 0, index);
		Source *source = static_cast<Source *>(child.internalPointer());
		if (source->status == "expanded") {
			this->expand(child);
		}
		autoExpandItemsFunc(theModel, child);
	}
}


void SourceFrame::folderExpand(const QModelIndex &index)
{
	if (autoExpandFlag == false) emit expandFolderSignal(static_cast<Source *>(index.internalPointer()));
}


void SourceFrame::folderCollapse(const QModelIndex &index)
{
	if (autoExpandFlag == false) emit collapseFolderSignal(static_cast<Source *>(index.internalPointer()));
}
