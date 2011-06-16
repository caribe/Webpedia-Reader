#ifndef SOURCE_H
#define SOURCE_H

#include <QObject>
#include <QHash>
#include <QDateTime>

class Source;

#include "post.h"

typedef QList<Post *> PostsArray;

class Source : public QObject
{
    Q_OBJECT

public:

	enum Filters {
		NewPosts,
		ImportantPosts,
		AllPosts
	};

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

	Filters filter;

	void updateDateTime();
	bool refreshNeeded();
	void setFilter(Filters);

private:
	QDateTime dateTime;

signals:

public slots:

};

#endif // SOURCE_H
