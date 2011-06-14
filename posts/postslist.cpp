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
	setStyleSheet("alternate-background-color: #E8F5C7; gridline-color: red");

	contextMenu = new QMenu(this);

	contextMenu->addAction(mainWindow->actionPostRead);
	contextMenu->addAction(mainWindow->actionPostUnread);
	contextMenu->addAction(mainWindow->actionPostFlag);
	contextMenu->addAction(mainWindow->actionPostLike);
	contextMenu->addAction(mainWindow->actionPostDelete);

	contextMenu->addSeparator();

	QAction *menuLinkCopy = new QAction(QIcon(":/resources/clipboard-paste.png"), tr("&Copy link address"), this);
	connect(menuLinkCopy, SIGNAL(triggered()), SLOT(actionLinkCopy()));
	contextMenu->addAction(menuLinkCopy);

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

void PostsList::actionLinkCopy()
{
	QModelIndexList indexList = selectionModel()->selectedRows();
	if (indexList.length() == 1) {
		QClipboard *cb = QApplication::clipboard();
		Post *post = (Post *)indexList[0].internalPointer();
		cb->setText(post->link);
	}
}
