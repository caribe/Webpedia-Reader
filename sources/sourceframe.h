#ifndef SOURCEFRAME_H
#define SOURCEFRAME_H

#include <QTreeView>
#include <QContextMenuEvent>
#include <QDebug>

class SourceFrame;

#include "mainwindow.h"
#include "data/source.h"

class SourceFrame : public QTreeView
{
    Q_OBJECT
public:
	explicit SourceFrame(MainWindow *parent = 0);
	void contextMenuEvent(QContextMenuEvent *event);
	void autoExpandItems();

private:
	MainWindow *mainWindow;
	QMenu *contextMenu;
	QAction *menuAdd;
	QAction *menuAddFolder;
	QAction *menuAddSeparator;
	bool autoExpandFlag;
	void autoExpandItemsFunc(QAbstractItemModel *theModel, const QModelIndex &index = QModelIndex());

signals:
	void addFolderSignal(Source *);
	void expandFolderSignal(Source *);
	void collapseFolderSignal(Source *);

private slots:
	void renameAction();
	void addFolderAction();
	void folderExpand(const QModelIndex &index);
	void folderCollapse(const QModelIndex &index);

public slots:
	void menuDeleteAction();
};

#endif // SOURCEFRAME_H
