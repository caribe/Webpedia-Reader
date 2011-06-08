#include "postslist.h"

PostsList::PostsList(PostsFrame *parent) : QTableView(parent) {

	this->parent = parent;

	setContextMenuPolicy(Qt::DefaultContextMenu);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);

	verticalHeader()->hide();
	verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height() + 3);
	setSortingEnabled(true);
	horizontalHeader()->setStretchLastSection(true);

	setAlternatingRowColors(true);
	setShowGrid(false);
	setStyleSheet("alternate-background-color: #E8F5C7; gridline-color: red");

	contextMenu = new QMenu(this);

	menuRead = new QAction(QIcon(":/resources/mail-open.png"), tr("&Read"), this);
	menuRead->setShortcut(Qt::Key_R);
	parent->connect(menuRead, SIGNAL(triggered()), SLOT(actionRead()));
	contextMenu->addAction(menuRead);

	menuLike = new QAction(QIcon(":/resources/star.png"), tr("&Like"), this);
	menuLike->setShortcut(Qt::Key_L);
	parent->connect(menuLike, SIGNAL(triggered()), SLOT(actionLike()));
	contextMenu->addAction(menuLike);

	menuUnlike = new QAction(QIcon(":/resources/star-empty.png"), tr("&Unlike"), this);
	parent->connect(menuUnlike, SIGNAL(triggered()), SLOT(actionUnlike()));
	contextMenu->addAction(menuUnlike);

	menuFlag = new QAction(QIcon(":/resources/flag.png"), tr("&Flag"), this);
	menuLike->setShortcut(Qt::Key_F);
	parent->connect(menuFlag, SIGNAL(triggered()), SLOT(actionFlag()));
	contextMenu->addAction(menuFlag);

	menuUnflag = new QAction(QIcon(":/resources/flag-white.png"), tr("&Unflag"), this);
	parent->connect(menuUnflag, SIGNAL(triggered()), SLOT(actionUnflag()));
	contextMenu->addAction(menuUnflag);

	menuUnread = new QAction(QIcon(":/resources/mail.png"), tr("&Unread"), this);
	parent->connect(menuUnread, SIGNAL(triggered()), SLOT(actionUnread()));
	contextMenu->addAction(menuUnread);

	QAction *menuDelete = new QAction(QIcon(":/resources/cross.png"), tr("&Delete"), this);
	parent->connect(menuDelete, SIGNAL(triggered()), SLOT(actionDelete()));
	contextMenu->addAction(menuDelete);

	contextMenu->addSeparator();

	QAction *menuLinkCopy = new QAction(QIcon(":/resources/clipboard-paste.png"), tr("&Copy link address"), this);
	connect(menuLinkCopy, SIGNAL(triggered()), SLOT(actionLinkCopy()));
	contextMenu->addAction(menuLinkCopy);

}

void PostsList::contextMenuEvent(QContextMenuEvent *event) {
	QModelIndexList indexList = selectionModel()->selectedRows();

	if (indexList.length() == 1) {
		Post *post = (Post *)indexList.at(0).internalPointer();

		if (post->status == "read" || post->status == "new") {
			menuRead->setVisible(true);
			menuFlag->setVisible(true);
			menuUnflag->setVisible(false);
			menuLike->setVisible(true);
			menuUnlike->setVisible(false);
		} else if (post->status == "flag") {
			menuRead->setVisible(false);
			menuFlag->setVisible(false);
			menuUnflag->setVisible(true);
			menuLike->setVisible(true);
			menuUnlike->setVisible(false);
		} else if (post->status == "like") {
			menuRead->setVisible(false);
			menuFlag->setVisible(true);
			menuUnflag->setVisible(false);
			menuLike->setVisible(false);
			menuUnlike->setVisible(true);
		}

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
