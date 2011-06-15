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

}

void PostsList::contextMenuEvent(QContextMenuEvent *event) {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() > 0) {
		/*
		Post *post = (Post *)indexList.at(0).internalPointer();

		if (post->status == Post::read || post->status == Post::unread) {
			menuRead->setVisible(true);
			menuFlag->setVisible(true);
			menuLike->setVisible(true);
		} else if (post->status == Post::flagged) {
			menuRead->setVisible(false);
			menuFlag->setVisible(false);
			menuLike->setVisible(true);
		} else if (post->status == Post::liked) {
			menuRead->setVisible(false);
			menuFlag->setVisible(true);
			menuLike->setVisible(false);
		}
*/
		contextMenu->exec(event->globalPos());
	}
}

