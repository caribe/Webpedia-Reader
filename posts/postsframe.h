#ifndef POSTSFRAME_H
#define POSTSFRAME_H

#include <QSplitter>
#include <QTableView>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QWebView>
#include <QStackedWidget>
#include <QWidgetAction>
#include <QDesktopServices>

class PostsFrame;

#include "mainwindow.h"
#include "postslist.h"
#include "model/modelposts.h"

class PostsFrame : public QSplitter
{
    Q_OBJECT
public:
	explicit PostsFrame(ModelPosts *postsModel, MainWindow *parent = 0);
	PostsList *list;
	MainWindow *mainWindow;

private:
	QLabel *viewerTitle;
	QWebView *viewer;

signals:
	void action(PostsArray &postslist, Post::Status code);
	void linkClicked(QUrl link, QString title);

public slots:
	void postSelected(const QModelIndex & current, const QModelIndex & previous);
	void actionPost();
	void actionLinkCopy();

private slots:
	void actionLink(QUrl link);
	void openPost();
};

#endif // POSTSFRAME_H
