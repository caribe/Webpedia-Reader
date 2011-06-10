#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QHash>

class Source;

#include "post.h"

typedef QList<Post *> PostsArray;

class Source : public QObject
{
    Q_OBJECT
public:
    explicit Source(QObject *parent = 0);
	int id, feed, row, unread;
	bool expanded;
	QString title;
	QString url;
	QString status;
	Source *parent;
	QList<Source *> childs;

	QList<Post *> posts;
	QHash<int, Post *> postsIndex;

signals:

public slots:

};

#endif // SOURCE_H
