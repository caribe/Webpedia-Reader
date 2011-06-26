#include "postslist.h"

PostsList::PostsList(PostsFrame *parent) : QTableView(parent) {

	this->parent = parent;
	mainWindow = parent->mainWindow;

	setContextMenuPolicy(Qt::DefaultContextMenu);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::ExtendedSelection);

	verticalHeader()->hide();
	verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height() + 3);
	setSortingEnabled(true);
	horizontalHeader()->setStretchLastSection(true);

	setAlternatingRowColors(true);
	setShowGrid(false);
	setStyleSheet("alternate-background-color: #ECF5D3");

	contextMenu = new QMenu(this);

	contextMenu->addAction(mainWindow->actionPostRead);
	contextMenu->addAction(mainWindow->actionPostUnread);
	contextMenu->addAction(mainWindow->actionPostFlag);
	contextMenu->addAction(mainWindow->actionPostLike);
	contextMenu->addAction(mainWindow->actionPostDelete);
	contextMenu->addSeparator();
	contextMenu->addAction(mainWindow->actionPostView);
	contextMenu->addAction(mainWindow->actionPostExternal);
	contextMenu->addSeparator();
	contextMenu->addAction(mainWindow->actionAddSourceById);

	connect(mainWindow->actionAddSourceById, SIGNAL(triggered()), SLOT(slotAddSourceById()));

}

void PostsList::contextMenuEvent(QContextMenuEvent *event) {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() > 0) {
		Post *post = static_cast<Post *>(indexList.last().internalPointer());
		mainWindow->actionAddSourceById->setEnabled(post->feedId != 0 && post->sourceId == 0 ? true : false);
		contextMenu->exec(event->globalPos());
	}
}


void PostsList::slotAddSourceById()
{
	QModelIndexList indexList = selectionModel()->selectedRows();
	if (indexList.length() == 1) {

		Post *post = static_cast<Post *>(indexList.at(0).internalPointer());

		if (post->feedId > 0 && post->sourceId == 0) {
			emit signalAddSourceById(post->feedId);
		}
	}

}
