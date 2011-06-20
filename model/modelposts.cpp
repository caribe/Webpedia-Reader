#include "model/modelposts.h"

ModelPosts::ModelPosts(Source *database, QObject *parent) : QAbstractTableModel(parent)
{
	this->database = database;
	currentSource = 0;

	authorIcon = QIcon(":/resources/user.png");
	statusIcons[Post::unread] = QIcon(":/resources/mail.png");
	statusIcons[Post::read] = QIcon(":/resources/mail-open.png");
	statusIcons[Post::liked] = QIcon(":/resources/heart.png");
	statusIcons[Post::flagged] = QIcon(":/resources/flag.png");
}


void ModelPosts::clear() {
	beginResetModel();
	if (currentSource) {
		foreach (Post *post, currentSource->posts) {
			delete post;
		}
		currentSource->posts.clear();
	}
}


void ModelPosts::changed() {
	beginResetModel();
}


void ModelPosts::flush() {
	endResetModel();
}


void ModelPosts::setSource(Source *source) {
	if (source != currentSource) {
		currentSource = source;
		reset();
	}
}


int ModelPosts::rowCount(const QModelIndex &parent) const {
	if (currentSource) {
		return currentSource->posts.length();
	} else {
		return 0;
	}
}

int ModelPosts::columnCount(const QModelIndex & parent) const {
	return 5;
}

Post *ModelPosts::getPost(const QModelIndex &index) {
	if (currentSource) {
		return currentSource->posts[index.row()];
	} else {
		return 0;
	}
}

QVariant ModelPosts::data(const QModelIndex & index, int role) const {

	Post *post = currentSource->posts[index.row()];

	if (!post) return QVariant();

	if (role == Qt::DisplayRole) {
		switch (index.column()) {
		case 0:
			return QVariant();
		case 1:
			return post->title;
		case 2:
			return post->pubdate.toString("hh:mm dd/MM/yyyy");
		case 3:
			return post->author;
		case 4:
			return post->source->title;
		}
	} else if (role == Qt::DecorationRole) {
		switch (index.column()) {
		case 0:
			return statusIcons[post->status];
		}
	} else if (role == Qt::UserRole + 1) {
		return post->id;
	}

	return QVariant();
}


QVariant ModelPosts::headerData(int section, Qt::Orientation orientation, int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QVariant();
		case 1:
			return tr("Title");
		case 2:
			return tr("Date");
		case 3:
			return tr("Author");
		case 4:
			return tr("Source");
		}
	}

	return QVariant();
}


QModelIndex ModelPosts::index(int row, int column, const QModelIndex & parent) const {
	if (row < currentSource->posts.length()) {
		Post *post = currentSource->posts[row];
		return createIndex(row, column, post);
	} else {
		return QModelIndex();
	}
}


void ModelPosts::sort(int column, Qt::SortOrder order) {
	beginResetModel();
	if (order == Qt::AscendingOrder) {
		switch (column) {
		case 0:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByStatusAsc);
		break;
		case 1:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByTitleAsc);
		break;
		case 2:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByDateAsc);
		break;
		case 3:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByAuthorAsc);
		break;
		case 4:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortBySourceAsc);
		break;
		}
	} else {
		switch (column) {
		case 0:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByStatusDesc);
		break;
		case 1:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByTitleDesc);
		break;
		case 2:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByDateDesc);
		break;
		case 3:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortByAuthorDesc);
		break;
		case 4:
			qSort(currentSource->posts.begin(), currentSource->posts.end(), ModelPosts::sortBySourceDesc);
		break;
		}
	}
	this->endResetModel();
}


bool ModelPosts::sortByStatusAsc(Post *p1, Post *p2)
{
	return (p1->status > p2->status) ? true : false;
}


bool ModelPosts::sortByStatusDesc(Post *p1, Post *p2)
{
	return (p1->status <= p2->status) ? true : false;
}


bool ModelPosts::sortByTitleAsc(Post *p1, Post *p2)
{
	return (p1->title.compare(p2->title) > 0) ? true : false;
}


bool ModelPosts::sortByTitleDesc(Post *p1, Post *p2)
{
	return (p1->title.compare(p2->title) <= 0) ? true : false;
}


bool ModelPosts::sortByAuthorAsc(Post *p1, Post *p2)
{
	return (p1->author.compare(p2->author) > 0) ? true : false;
}


bool ModelPosts::sortByAuthorDesc(Post *p1, Post *p2)
{
	return (p1->author.compare(p2->author) <= 0) ? true : false;
}


bool ModelPosts::sortBySourceAsc(Post *p1, Post *p2)
{
	return (p1->source->title.compare(p2->source->title) > 0) ? true : false;
}


bool ModelPosts::sortBySourceDesc(Post *p1, Post *p2)
{
	return (p1->source->title.compare(p2->source->title) <= 0) ? true : false;
}


bool ModelPosts::sortByDateAsc(Post *p1, Post *p2)
{
	return (p1->pubdate > p2->pubdate) ? true : false;
}


bool ModelPosts::sortByDateDesc(Post *p1, Post *p2)
{
	return (p1->pubdate <= p2->pubdate) ? true : false;
}
