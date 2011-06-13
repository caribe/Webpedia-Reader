#include "connection.h"

Connection::Connection(MainWindow *parent) : QNetworkAccessManager(parent)
{
	this->connect(this, SIGNAL(finished(QNetworkReply *)), SLOT(finish(QNetworkReply *)));
	mainWindow = parent;
}


void Connection::login(QString username, QString password)
{
	ParamHash data;
	data["username"] = username;
	data["password"] = password;
	sendRequest("login", data);
}


void Connection::sendRequest(QString action, ParamHash &data, bool noAuth)
{
	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	if (noAuth == false) {
		request.setRawHeader("Authorization", "Basic "+QString(mainWindow->user+":"+mainWindow->token).toLocal8Bit().toBase64());
	}

	mainWindow->setStatus(tr("Connection"), true);

	post(request, postDataEncoding(data));
}


void Connection::sendRequest(QString action, bool noAuth)
{
	QNetworkRequest request(QUrl(mainWindow->baseUrl+"reader."+action+"/"));
	if (noAuth == false) {
		request.setRawHeader("Authorization", "Basic "+QString(mainWindow->user+":"+mainWindow->token).toLocal8Bit().toBase64());
	}

	mainWindow->setStatus(tr("Connection"), true);

	get(request);
}


void Connection::update()
{
	if (mainWindow->logged == true) sendRequest("sources");
}


void Connection::sourceSelected(const QModelIndex & current, const QModelIndex & previous)
{
	Source *source = static_cast<Source *>(current.internalPointer());
	if (source->feed == 0) {
		mainWindow->postsModel->setSource(source);
	} else {
		mainWindow->postsModel->currentSource = source;
		mainWindow->splitter2->setCurrentIndex(1);
		sendRequest(QString("source/%1").arg(source->id));
	}
}


void Connection::postSelected(const QModelIndex & current, const QModelIndex & previous)
{
	ModelPosts *model = mainWindow->postsModel;

	Post *item = model->getPost(current);

	if (item) {
		// mainWindow->post->setHtml("<html><body>"+item->body+"</body></html>");

		if (item->status == Post::unread) {
			sendRequest(QString("postread/%1").arg(item->id));
		}
	}
}


void Connection::addFolder(Source *parentSource)
{
	int source_id = 0;
	if (parentSource) source_id = parentSource->id;

	sendRequest(QString("folderadd/%1").arg(source_id));
}


void Connection::moveFolder(Source *source, Source *parentSource, int row)
{
	int parent_id = 0;
	if (parentSource) parent_id = parentSource->id;

	sendRequest(QString("sourcemove/%1,%2,%3").arg(source->id).arg(parent_id).arg(row));
}


void Connection::renameFolder(Source *source)
{
	sendRequest(QString("sourcerename/%1,%2").arg(source->id).arg(source->title));
}


void Connection::sendAction(PostsArray &postsArray, int code)
{
	QStringList modes;
	modes << "delete" << "like" << "unlike" << "flag" << "unflag" << "read" << "unread";

	QStringList sources;
	foreach (Post *post, postsArray) {
		sources << QString::number(post->id);
	}

	sendRequest(QString("post%1/%2").arg(modes[code]).arg(sources.join(",")));
}


void Connection::sourceList()
{
	sendRequest("list");
}


void Connection::addSource(QString url)
{
	QHash<QString, QString> data;
	data.insert("url", url);

	sendRequest("sourceadd", data);
}


void Connection::sourceAdd(int source)
{
	sendRequest(QString("sourceadd/%1").arg(source));
}


void Connection::sourceDel(Source *source)
{
	sendRequest(QString("sourcedel/%1").arg(source->id));
}


void Connection::folderExpand(Source *source) {
	sendRequest(QString("sourceexpand/%1").arg(source->id));
}

void Connection::folderCollapse(Source *source) {
	sendRequest(QString("sourcecollapse/%1").arg(source->id));
}


// *** Elaborazione XML

void Connection::finish(QNetworkReply *reply)
{
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

				sourcesListModel->clear();

				for (QDomElement sourceE = rootN.firstChildElement("source"); !sourceE.isNull(); sourceE = sourceE.nextSiblingElement("source")) {

					QStandardItem *item = new QStandardItem(QIcon(":/resources/feed.png"), sourceE.text());
					item->setData(sourceE.attribute("id").toInt(), Qt::UserRole + 1);

					QStandardItem *follower = new QStandardItem(sourceE.attribute("follower"));

					QList<QStandardItem *> row;
					row << item << follower;

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
					mainWindow->postsFrame->list->selectRow(selection[0].row());
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
