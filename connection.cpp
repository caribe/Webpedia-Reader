#include "connection.h"

Connection::Connection(MainWindow *parent) : QNetworkAccessManager(parent)
{
	this->connect(this, SIGNAL(finished(QNetworkReply *)), SLOT(finish(QNetworkReply *)));
	mainWindow = parent;
}



void Connection::sendAuthRequest(QString action, ParamHash &data)
{
	qDebug() << "AUTH: "+mainWindow->baseUrl+"reader."+action+"/";
	mainWindow->setStatus(tr("Connection"), true);

	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	post(request, postDataEncoding(data));
}



void Connection::sendPostRequest(QString action, ParamHash &data)
{
	qDebug() << "POST: "+mainWindow->baseUrl+"reader."+action+"/";

	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	request.setRawHeader("Authorization", "Basic "+QString(mainWindow->user+":"+mainWindow->token).toLocal8Bit().toBase64());

	mainWindow->setStatus(tr("Connection"), true);

	post(request, postDataEncoding(data));
}


void Connection::sendGetRequest(QString action)
{
	qDebug() << "GET: "+mainWindow->baseUrl+"reader."+action+"/";

	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	request.setRawHeader("Authorization", "Basic "+QString(mainWindow->user+":"+mainWindow->token).toLocal8Bit().toBase64());

	mainWindow->setStatus(tr("Connection"), true);

	get(request);
}


void Connection::sendDeleteRequest(QString action)
{
	qDebug() << "DELETE: "+mainWindow->baseUrl+"reader."+action+"/";

	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	request.setRawHeader("Authorization", "Basic "+QString(mainWindow->user+":"+mainWindow->token).toLocal8Bit().toBase64());

	mainWindow->setStatus(tr("Connection"), true);

	deleteResource(request);
}


void Connection::update()
{
	if (mainWindow->logged == true) sendGetRequest("sources.show");
}


void Connection::sourceSelected(const QModelIndex & index, const QModelIndex & previous)
{
	sourceSelected(index);
}


void Connection::sourceSelected(const QModelIndex & index)
{
	Source *source = static_cast<Source *>(index.internalPointer());
	mainWindow->splitter2->setCurrentIndex(1);
	mainWindow->postsModel->setSource(source);
	if (source->refreshNeeded()) {
		QStringList filters;
		filters << "news" << "important" << "all";
		sendGetRequest(QString("source.posts/%1,%2").arg(source->id).arg(filters[source->filter]));
	}
}


void Connection::addFolder(Source *parentSource)
{
	int source_id = 0;
	if (parentSource) source_id = parentSource->id;

	ParamHash data;
	data["parent"] = source_id;

	sendPostRequest(QString("folder.create"), data);
}


void Connection::moveFolder(Source *source, Source *parentSource, int row)
{
	int parent_id = 0;
	if (parentSource) parent_id = parentSource->id;

	ParamHash data;
	data["source"] = QString::number(source->id);
	data["parent"] = QString::number(parent_id);
	data["position"] = QString::number(row);

	if (source->feed == 0) {
		sendPostRequest(QString("folder.move"), data);
	} else {
		sendPostRequest(QString("source.move"), data);
	}
}


void Connection::renameFolder(Source *source)
{
	ParamHash data;
	data["source"] = QString::number(source->id);
	data["name"] = source->title;

	qDebug() << source->id;

	if (source->feed == 0) {
		sendPostRequest(QString("folder.rename"), data);
	} else {
		sendPostRequest(QString("source.rename"), data);
	}
}


void Connection::sendAction(PostsArray &postsArray, Post::Status code)
{
	QStringList modes;
	modes << "unread" << "read" << "flag" << "like" << "delete";

	QStringList sources;
	foreach (Post *post, postsArray) {
		sources << QString::number(post->id);
	}

	ParamHash data;
	data["posts"] = sources.join(",");
	data["state"] = modes[code];

	sendPostRequest(QString("post.modify"), data);
}


void Connection::sourceList()
{
	sendGetRequest("source.list");
}


void Connection::addSource(QString url)
{
	QHash<QString, QString> data;
	data.insert("url", url);

	sendPostRequest("source.add", data);
}


void Connection::sourceAdd(int source)
{
	ParamHash data;
	data["source"] = QString::number(source);

	sendPostRequest(QString("source.follow"), data);
}


void Connection::sourceDel(Source *source)
{
	if (source->feed == 0) {
		sendDeleteRequest(QString("folder.destroy/%1").arg(source->id));
	} else {
		sendDeleteRequest(QString("source.ignore/%1").arg(source->id));
	}
}


void Connection::folderExpand(Source *source) {
	ParamHash data;
	data["source"] = QString::number(source->id);
	sendPostRequest(QString("folder.expand"), data);
}

void Connection::folderCollapse(Source *source) {
	ParamHash data;
	data["source"] = QString::number(source->id);
	sendPostRequest(QString("folder.collapse"), data);
}


// *** Elaborazione XML

void Connection::finish(QNetworkReply *reply)
{
	if (reply->error() != QNetworkReply::NoError) {
		QMessageBox::critical(mainWindow, "Errore", reply->errorString());
		return;
	}

	QDomDocument *xml = new QDomDocument();
	xml->setContent(reply->readAll());

	QDomElement root = xml->documentElement();

	QDomNodeList rootC = root.childNodes();

	ModelSources *sourcesModel = mainWindow->sourcesModel;
	ModelPosts *postsModel = mainWindow->postsModel;
	QStandardItemModel *sourcesListModel = mainWindow->sourcesListModel;

	for (unsigned int rootI = 0; rootI < rootC.length(); rootI++) {
		QDomNode rootN = rootC.item(rootI);
		if (rootN.isElement()) {
			if (rootN.toElement().tagName() == "message") {

				QMessageBox::information(mainWindow, "Informazione", rootN.toElement().text());

			} else if (rootN.toElement().tagName() == "error") {

				QMessageBox::critical(mainWindow, "Errore", rootN.toElement().text());

			} else if (rootN.toElement().tagName() == "sources") {

				int preUnreadPosts = mainWindow->sourcesModel->sumUnreadPosts();

				sourcesModel->clear();

				Source *current = sourcesModel->rootItem();

				descendSourceTree(current, rootN.toElement(), sourcesModel);

				sourcesModel->flush();

				mainWindow->tree->autoExpandItems();

				int postUnreadPosts = mainWindow->sourcesModel->sumUnreadPosts();
				if (postUnreadPosts > preUnreadPosts) {
					if (postUnreadPosts - preUnreadPosts == 1) {
						mainWindow->trayIcon->showMessage(tr("Webpedia Reader"), tr("There is a new post"));
					} else {
						mainWindow->trayIcon->showMessage(tr("Webpedia Reader"), tr("There are %1 new posts").arg(postUnreadPosts - preUnreadPosts));
					}
				}

			} else if (rootN.toElement().tagName() == "sources-list") {

				QStringList headerLabels;
				headerLabels << tr("Source") << tr("Followers") << tr("Posts");

				sourcesListModel->clear();
				sourcesListModel->setHorizontalHeaderLabels(headerLabels);

				for (QDomElement sourceE = rootN.firstChildElement("source"); !sourceE.isNull(); sourceE = sourceE.nextSiblingElement("source")) {

					QStandardItem *item = new QStandardItem(QIcon(":/resources/feed.png"), sourceE.text());
					item->setEditable(false);
					item->setData(sourceE.attribute("id").toInt(), Qt::UserRole + 1);

					QStandardItem *followers = new QStandardItem();
					followers->setData(sourceE.attribute("followers").toInt(), Qt::DisplayRole);
					followers->setEditable(false);
					followers->setTextAlignment(Qt::AlignRight);

					QStandardItem *posts = new QStandardItem();
					posts->setData(sourceE.attribute("posts").toInt(), Qt::DisplayRole);
					posts->setEditable(false);
					posts->setTextAlignment(Qt::AlignRight);

					QList<QStandardItem *> row;
					row << item << followers << posts;

					sourcesListModel->appendRow(row);
				}

			} else if (rootN.toElement().tagName() == "posts") {

				// Source ID
				QDomElement rootE = rootN.toElement();
				int source_id = rootE.attribute("source").toInt();

				Source *source = sourcesModel->sourcesIndex[source_id];

				if (source) {
					QModelIndexList sel = mainWindow->postsFrame->list->selectionModel()->selectedRows();
					postsModel->clear();

					for (QDomElement el = rootN.firstChildElement("post"); !el.isNull(); el = el.nextSiblingElement("post")) {
						Post *post = new Post();
						post->id = el.attribute("id").toInt();
						post->status = stringToStatus(el.attribute("status"));
						post->pubdate = QDateTime::fromString(el.attribute("date"), "yyyy/MM/dd hh:mm");
						post->title = el.firstChildElement("title").text();
						post->author = el.firstChildElement("author").text();
						post->body = el.firstChildElement("body").text();
						post->link = el.firstChildElement("link").text();
						post->source = source;

						source->posts.append(post);
						source->postsIndex.insert(post->id, post);
					}

					postsModel->flush();

					foreach (QModelIndex row, sel) {
						if (row.row() < postsModel->rowCount()) {
							mainWindow->postsFrame->list->selectRow(row.row());
						} else {
							mainWindow->postsFrame->list->selectRow(postsModel->rowCount() - 1);
						}
					}

					source->updateDateTime();
				}

			} else if (rootN.toElement().tagName() == "posts-update") {

				QModelIndexList selection = mainWindow->postsFrame->list->selectionModel()->selectedRows();
				postsModel->changed();

				for (QDomElement el = rootN.firstChildElement("post"); !el.isNull(); el = el.nextSiblingElement("post")) {
					int post_id = el.attribute("id").toInt();
					sourcesModel->updatePost(el.attribute("source").toInt(), post_id, stringToStatus(el.attribute("status")));
				}

				postsModel->flush();

				if (selection.length() > 0) {
					foreach (QModelIndex row, selection) {
						if (row.row() < postsModel->rowCount()) {
							mainWindow->postsFrame->list->selectRow(row.row());
						} else {
							mainWindow->postsFrame->list->selectRow(postsModel->rowCount() - 1);
						}
					}
				}

			} else if (rootN.toElement().tagName() == "login") {

				mainWindow->token = rootN.firstChildElement("token").text();
				mainWindow->username = rootN.firstChildElement("username").text();
				mainWindow->user = rootN.firstChildElement("user").text();

				QSettings settings;

				settings.setValue("user/user", mainWindow->user);
				settings.setValue("user/username", mainWindow->username);
				settings.setValue("user/token", mainWindow->token);

				settings.sync();

				mainWindow->menuConnect->setVisible(false);
				mainWindow->menuDisconnect->setVisible(true);

			}
		}
	}

	mainWindow->setStatus(tr("Ready"), false);
}


void Connection::descendSourceTree(Source *parent, QDomElement node, ModelSources *model)
{

	if (node.tagName() == "sources") {
		for (QDomElement el = node.firstChildElement("source"); !el.isNull(); el = el.nextSiblingElement("source")) {
			descendSourceTree(parent, el, model);
		}
	} else {
		Source *source = new Source();
		source->id = node.attribute("id").toInt();
		source->title = node.firstChildElement("name").text();
		source->parent = parent;
		source->status = node.attribute("status");
		source->unread = node.attribute("unread").toInt();

		if (node.attribute("type") == "special") {
			source->type = Source::SpecialFeed;
		} else {
			source->type = Source::Feed;
		}

		QString feed = node.attribute("feed");
		if (feed.isEmpty() == false) source->feed = feed.toInt();

		model->sourcesIndex.insert(source->id, source);
		parent->childs.append(source);

		for (QDomElement el = node.firstChildElement("source"); !el.isNull(); el = el.nextSiblingElement("source")) {
			descendSourceTree(source, el, model);
		}
	}
}


// *** Utility


QByteArray Connection::postDataEncoding(ParamHash hash)
{
	QStringList list;
	QHashIterator<QString, QString> i(hash);

	while (i.hasNext()) {
		i.next();
		list << i.key()+"="+QUrl::toPercentEncoding(i.value());
	}

	return list.join("&").toUtf8();
}


Post::Status Connection::stringToStatus(QString status) {
	if (status == "read") {
		return Post::read;
	} else if (status == "new") {
		return Post::unread;
	} else if (status == "like") {
		return Post::liked;
	} else if (status == "flag") {
		return Post::flagged;
	} else if (status == "delete") {
		return Post::deleted;
	}

	return Post::unread;
}
