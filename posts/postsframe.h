#ifndef POSTSFRAME_H
#define POSTSFRAME_H

#include <QSplitter>
#include <QTableView>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QTextBrowser>

class PostsFrame;

#include "postslist.h"
#include "model/modelposts.h"

class PostsFrame : public QSplitter
{
    Q_OBJECT
public:
	explicit PostsFrame(ModelPosts *postsModel, QWidget *parent = 0);
	PostsList *list;

private:
	QLabel *viewerTitle;
	QTextBrowser *viewer;
	void keyPressEvent(QKeyEvent *event);

signals:
	void action(Post *post, int code);
	void linkClicked(QUrl link);

public slots:
	void postSelected(const QModelIndex & current, const QModelIndex & previous);
	void actionDelete();
	void actionLike();
	void actionUnlike();
	void actionFlag();
	void actionUnflag();
	void actionRead();
	void actionUnread();

private slots:
	void actionLink(QUrl link);
	void openPost();
};

#endif // POSTSFRAME_H
