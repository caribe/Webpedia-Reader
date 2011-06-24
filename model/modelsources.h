#ifndef MODELSOURCES_H
#define MODELSOURCES_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QTextStream>
#include <QMimeData>
#include <QtDebug>

#include "data/source.h"

class ModelSources : public QAbstractItemModel
{
    Q_OBJECT
public:
	explicit ModelSources(Source *database, QObject *parent = 0);

	// Properties
	Source *rootItem();
	QHash<int, Source *> sourcesIndex;

	// Subclassing
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	QModelIndex index (int row, int column, const QModelIndex & parent = QModelIndex()) const;
	QModelIndex parent (const QModelIndex & index = QModelIndex()) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	// Drag & Drop
	Qt::ItemFlags flags(const QModelIndex &index) const;
	bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	QMimeData *mimeData(const QModelIndexList &indexes) const;
	QStringList mimeTypes() const;
	Qt::DropActions supportedDropActions() const;

	// Utils
	void clearBranch(Source *source);
	QModelIndex indexBySource(Source *source, Source *parent = 0);
	void updateUnreadCount(Source *source, int value);
	int sumUnreadPosts(Source *source = 0) const;

	void updatePost(int source_id, int post_id, Post::Status status);

	void clear();
	void flush();

private:
	Source *database;
	QIcon sourceIcon;
	QIcon sourceSpecialIcon;
	QIcon sourceFolderIcon;

signals:
	void sourceMoved(Source *source, Source *parent, int row);
	void sourceRenamed(Source *source);

public slots:
	void folderCollapsed(const QModelIndex & index);
	void folderExpanded(const QModelIndex & index);
};

#endif // MODELSOURCES_H
