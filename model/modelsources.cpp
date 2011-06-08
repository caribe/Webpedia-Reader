#include "modelsources.h"

ModelSources::ModelSources(Source *database, QObject *parent) : QAbstractItemModel(parent)
{
	sourceIcon = QIcon(":/resources/feed.png");
	sourceFolderIcon = QIcon(":/resources/folder-open-feed.png");
	this->database = database;
}


void ModelSources::clear()
{
	beginResetModel();
	sourcesIndex.clear();
	clearBranch(database);
}


void ModelSources::clearBranch(Source *source)
{
	foreach (Source *child, source->childs) {
		clearBranch(child);
		delete child;
	}
	source->childs.clear();

	foreach (Post *post, source->posts) {
		delete post;
	}
	source->posts.clear();
}


void ModelSources::flush()
{
	endResetModel();
}


int ModelSources::rowCount(const QModelIndex & parent) const
{
	Source *source;

	if (parent.isValid()) { // Righe in root
		source = (Source *)parent.internalPointer();
	} else {
		source = database;
	}

	int rows = 0;

	if (source) {
		rows = source->childs.length();
	}

	return rows;
}


int ModelSources::columnCount(const QModelIndex & parent) const
{
	return 2;
}


QVariant ModelSources::data(const QModelIndex & index, int role) const
{
	if (index.isValid()) {
		Source *source = static_cast<Source *>(index.internalPointer());

		switch(role) {
		case Qt::DisplayRole:
		case Qt::EditRole:
			if (index.column() == 0) {
				return source->title;
			} else {
				if (source->feed == 0) {
					return sumUnreadPosts(source);
				} else {
					return source->unread;
				}
			}
			break;
		case Qt::DecorationRole:
			if (index.column() == 0) {
				if (source->feed) {
					return sourceIcon;
				} else {
					return sourceFolderIcon;
				}
			} else {
				return QVariant();
			}
			break;
		case Qt::TextAlignmentRole:
			if (index.column() == 0) {
				return Qt::AlignLeft;
			} else {
				return Qt::AlignRight;
			}
			break;
		case Qt::UserRole + 1:
			return source->feed;
		}
	}

	return QVariant();
}


QModelIndex ModelSources::index(int row, int column, const QModelIndex & parent) const {

	Source *source;

	if (parent.isValid()) {
		source = (Source *)parent.internalPointer();
	} else {
		source = database;
	}

	return createIndex(row, column, source->childs[row]);
}

QModelIndex ModelSources::parent(const QModelIndex &index) const {
	if (index.isValid()) {
		Source *source = (Source *)index.internalPointer();
		Source *parent = source->parent;
		Source *grandParent = 0;

		if (parent) grandParent = parent->parent;

		if (grandParent) {
			int row = 0;
			foreach (Source *s, grandParent->childs) {
				if (s == parent) break;
				row++;
			}
			return createIndex(row, 0, parent);
		} else {
			return QModelIndex();
		}
	} else {
		return QModelIndex();
	}
}

QVariant ModelSources::headerData(int section, Qt::Orientation orientation, int role) const {
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
			case 0:
				return tr("Sources");
			case 1:
				return tr("Unread");
		}
	}

	return QVariant();
}


Source *ModelSources::rootItem()
{
	return database;
}


Qt::DropActions ModelSources::supportedDropActions() const
{
	return Qt::MoveAction;
}


Qt::ItemFlags ModelSources::flags(const QModelIndex &index) const
{
	Qt::ItemFlags defaultFlags = QAbstractItemModel::flags(index);

	if (index.isValid()) {
		if (index.column() == 0) {
			return Qt::ItemIsDragEnabled | Qt::ItemIsEditable | Qt::ItemIsDropEnabled | defaultFlags;
		} else {
			return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
		}
	} else {
		return Qt::ItemIsDropEnabled | defaultFlags;
	}
}


QStringList ModelSources::mimeTypes() const
{
	QStringList types;
	types << "text/plain";
	return types;
}


QMimeData *ModelSources::mimeData(const QModelIndexList &indexes) const
{
	QMimeData *mimeData = new QMimeData();
	QByteArray encodedData;

	QDataStream stream(&encodedData, QIODevice::WriteOnly);

	foreach (QModelIndex index, indexes) {
		if (index.isValid()) {
			Source *source = static_cast<Source *>(index.internalPointer());
			stream << source->id;
		}
	}

	mimeData->setData("text/plain", encodedData);
	return mimeData;
}

bool ModelSources::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
	if (action == Qt::IgnoreAction)
		return true;

	if (!data->hasFormat("text/plain"))
		return false;

	int beginRow;

	if (row != -1)
		beginRow = row;
	else if (parent.isValid())
		beginRow = 0;
	else
		beginRow = rowCount(QModelIndex());

	QByteArray encodedData = data->data("text/plain");
	QDataStream stream(&encodedData, QIODevice::ReadOnly);

	Source *parentSource = static_cast<Source *>(parent.internalPointer());

	if (parentSource) {
		if (row == -1) { // Esattamente su un elemento
			if (parentSource->feed != 0) { // Su una fonte
				row = parent.row();
				parentSource = static_cast<Source *>(parent.parent().internalPointer());
			}
		}
	}

	while (!stream.atEnd()) {
		int id;
		stream >> id;
		Source *source = sourcesIndex[id];
		emit sourceMoved(source, parentSource, row);
	}

	return true;
}


void ModelSources::folderCollapsed(const QModelIndex & index)
{
	Source *source = static_cast<Source *>(index.internalPointer());
	source->expanded = false;
}

void ModelSources::folderExpanded(const QModelIndex & index)
{
	Source *source = static_cast<Source *>(index.internalPointer());
	source->expanded = true;
}


bool ModelSources::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if (role == Qt::EditRole && index.isValid()) {
		Source *source = static_cast<Source *>(index.internalPointer());
		source->title = value.toString();
		emit dataChanged(index, index);
		emit sourceRenamed(source);
	}
}


int ModelSources::sumUnreadPosts(Source *source) const
{
	int sum = 0;

	if (source == 0) source = database;

	foreach (Source *item, source->childs) {
		if (item->feed == 0) {
			sum += sumUnreadPosts(item);
		} else {
			sum += item->unread;
		}
	}

	return sum;
}


QModelIndex ModelSources::indexBySource(Source *source, Source *parent) {

	if (parent == 0) parent = database;

	int i = 0;
	foreach (Source *child, parent->childs) {
		if (child->id == source->id) return createIndex(i, 0, child);
		QModelIndex res = indexBySource(source, child);
		if (res.isValid()) return res;
		i++;
	}

	return QModelIndex();
}


void ModelSources::updateUnreadCount(Source *source, int value) {
	source->unread += value;
	QModelIndex index = indexBySource(source);
	emit dataChanged(index, index.sibling(index.row(), 1));
}
