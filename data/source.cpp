#include "source.h"

Source::Source(QObject *parent) : QObject(parent)
{
	this->parent = 0;
	this->id = 0;
	this->feed = 0;
	filter == NewPosts;
}


void Source::updateDateTime()
{
	dateTime = QDateTime::currentDateTime();
}

bool Source::refreshNeeded()
{
	if (this->feed == 0) return false;

	if (dateTime.isValid() && dateTime.secsTo(QDateTime::currentDateTime()) >= 60 * 15) {
		return true;
	} else {
		return false;
	}
}


void Source::setFilter(Filters filter)
{
	if (filter != this->filter) {
		this->filter = filter;
		dateTime = QDateTime;
	}
}
