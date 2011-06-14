#ifndef UPDATER_H
#define UPDATER_H

#include <iostream>

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>

#include <QStandardItemModel>
#include <QSortFilterProxyModel>

#include <QTextStream>
#include <QDomDocument>
#include <QSettings>
#include <QtDebug>

class Connection;

#include "mainwindow.h"
#include "data/source.h"
#include "model/modelsources.h"

class Connection : public QNetworkAccessManager
{
	Q_OBJECT

public:
	Connection(MainWindow *parent = 0);
	void login(QString username, QString password);
	void sourceAdd(int source);
	void sourceDel(Source *source);

	typedef QHash<QString, QString> ParamHash;

private:
	MainWindow *mainWindow;
	void descendSourceTree(Source *current, QDomElement node, ModelSources *sourcesModel);
	QByteArray postDataEncoding(ParamHash);
	Post::Status stringToStatus(QString status);

	void sendRequest(QString action, ParamHash &data, bool noAuth = false);
	void sendRequest(QString action, bool noAuth = false);

public slots:
	void update();
	void finish(QNetworkReply *reply);
	void sourceSelected(const QModelIndex & current, const QModelIndex & previous);
	void postSelected(const QModelIndex & current, const QModelIndex & previous);
	void addSource(QString url);
	void sourceList();
	void sendAction(PostsArray &postsArray, Post::Status code);
	void addFolder(Source *);
	void moveFolder(Source *, Source *, int);
	void renameFolder(Source *);
	void folderExpand(Source *);
	void folderCollapse(Source *);
};

#endif // UPDATER_H
