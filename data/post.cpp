#include "post.h"

Post::Post(QObject *parent) :
    QObject(parent)
{
	id = feedId = sourceId = 0;
}
