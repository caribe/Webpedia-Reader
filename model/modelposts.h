#ifndef MODELPOSTS_H
#define MODELPOSTS_H

#include <QAbstractTableModel>
#include <QIcon>
#include <QDebug>

#include "data/source.h"

class ModelPosts : public QAbstractTableModel
{
    Q_OBJECT
public:
	explicit ModelPosts(Source *database, QObject *parent = 0);

	// Subclassing
	int rowCount(const QModelIndex & parent = QModelIndex()) const;
	int columnCount(const QModelIndex & parent = QModelIndex()) const;
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
	QVariant headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	QModelIndex index(int row, int column, const QModelIndex & parent) const;

	Post *getPost(const QModelIndex &index);
	void setSource(Source *source);

	void clear();
	void flush();
	void updatePost(int post_id);

	void sort( int column, Qt::SortOrder order = Qt::AscendingOrder);
	static bool sortByStatusAsc(Post *p1, Post *p2);
	static bool sortByStatusDesc(Post *p1, Post *p2);
	static bool sortByTitleAsc(Post *p1, Post *p2);
	static bool sortByTitleDesc(Post *p1, Post *p2);
	static bool sortByAuthorAsc(Post *p1, Post *p2);
	static bool sortByAuthorDesc(Post *p1, Post *p2);
	static bool sortByDateAsc(Post *p1, Post *p2);
	static bool sortByDateDesc(Post *p1, Post *p2);
	static bool sortBySourceAsc(Post *p1, Post *p2);
	static bool sortBySourceDesc(Post *p1, Post *p2);

	Source *currentSource;

private:
	Source *database;
	QHash<Post::Status, QIcon> statusIcons;
	QIcon authorIcon;

signals:

public slots:

};

#endif // MODELPOSTS_H
