#ifndef POST_H
#define POST_H

#include <QObject>
#include <QDateTime>

class Post;

#include "source.h"

class Post : public QObject
{
    Q_OBJECT

public:
    explicit Post(QObject *parent = 0);

	enum Status {
		unread,
		read,
		flagged,
		liked,
		deleted
	};

	int id;
	QString title, author, body, link;
	Status status;
	QDateTime pubdate;
	Source *source;

	int feedId, sourceId;
	QString feedName;

signals:

public slots:

};

#endif // POST_H
