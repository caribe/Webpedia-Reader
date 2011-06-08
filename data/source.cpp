#include "source.h"

Source::Source(QObject *parent) : QObject(parent)
{
	this->parent = 0;
	this->id = 0;
	this->feed = 0;
}
