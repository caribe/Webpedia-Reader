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

}

void PostsList::contextMenuEvent(QContextMenuEvent *event) {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() > 0) {
		contextMenu->exec(event->globalPos());
	}
}

