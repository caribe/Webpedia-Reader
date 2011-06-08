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
	int id;
        QString title, author, body, status, link;
        QDateTime pubdate;
	Source *source;

signals:

public slots:

};

#endif // POST_H
