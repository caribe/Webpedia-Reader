#ifndef POSTSLIST_H
#define POSTSLIST_H

#include <QApplication>
#include <QClipboard>
#include <QTableView>
#include <QHeaderView>
#include <QContextMenuEvent>
#include <QMenu>

class PostsList;

#include "mainwindow.h"
#include "data/post.h"
#include "postsframe.h"

class PostsList : public QTableView
{
    Q_OBJECT

private:
	PostsFrame *parent;
	MainWindow *mainWindow;

	QMenu *contextMenu;
	QAction *menuLike;
	QAction *menuFlag;
	QAction *menuRead;
	QAction *menuUnread;
	void contextMenuEvent(QContextMenuEvent *event);

public:
	explicit PostsList(PostsFrame *parent = 0);

private slots:
	void actionLinkCopy();

};

#endif // POSTSLIST_H
