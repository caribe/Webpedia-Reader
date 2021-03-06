#include "postsframe.h"

PostsFrame::PostsFrame(ModelPosts *postsModel, MainWindow *parent) : QSplitter(parent)
{
	mainWindow = parent;

	setOrientation(Qt::Vertical);

	list = new PostsList(this);
	list->setModel(postsModel);

	addWidget(list);

	connect(list->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(postSelected(QModelIndex,QModelIndex)));
	connect(postsModel, SIGNAL(modelReset()), list, SLOT(resizeColumnsToContents()));
	connect(list, SIGNAL(doubleClicked(QModelIndex)), SLOT(openPost(QModelIndex)));

	// *** Viewer

	QVBoxLayout *viewerLayout = new QVBoxLayout();
	viewerLayout->setContentsMargins(0, 0, 0, 0);

	QHBoxLayout *viewerTitleLayout = new QHBoxLayout();
	viewerTitleLayout->setContentsMargins(0, 0, 0, 0);

	viewerTitle = new QLabel();
	viewerTitle->setWordWrap(true);
	viewerTitleLayout->addWidget(viewerTitle, 1);

	QToolBar *widgetAction = new QToolBar();
	widgetAction->addAction(mainWindow->actionPostView);
	viewerTitleLayout->addWidget(widgetAction);

	viewerLayout->addLayout(viewerTitleLayout);

	viewer = new QWebView();
	viewer->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	connect(viewer, SIGNAL(linkClicked(QUrl)), SLOT(actionLink(QUrl)));

	QStackedWidget *stackedWidget = new QStackedWidget();
	stackedWidget->setFrameShape(QFrame::StyledPanel);
	stackedWidget->setFrameShadow(QFrame::Sunken);
	stackedWidget->addWidget(viewer);

	viewerLayout->addWidget(stackedWidget);

	QWidget *viewerWidget = new QWidget();
	viewerWidget->setLayout(viewerLayout);

	addWidget(viewerWidget);
}


void PostsFrame::actionPost() {
	PostsArray postsList;
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	foreach (QModelIndex index, indexes) {
		postsList << static_cast<Post *>(index.internalPointer());
	}

	if (postsList.length() > 0) {

		QString senderName = QObject::sender()->objectName();

		if (senderName == "actionPostRead") {
			emit action(postsList, Post::read);
		} else if (senderName == "actionPostLike") {
			emit action(postsList, Post::liked);
		} else if (senderName == "actionPostFlag") {
			emit action(postsList, Post::flagged);
		} else if (senderName == "actionPostUnread") {
			emit action(postsList, Post::unread);
		} else if (senderName == "actionPostDelete") {
			emit action(postsList, Post::deleted);
		}
	}
}



void PostsFrame::postSelected(const QModelIndex & index, const QModelIndex & previous)
{
	Post *post = static_cast<Post *>(index.internalPointer());
	viewerTitle->setText(QString("<h3>%1</h3>").arg(post->title));
	viewer->setHtml("<!DOCTYPE html>\n<html>\n<body>\n"+post->body+"</body>\n</html>\n");
}



void PostsFrame::actionLink(QUrl link)
{
	emit linkClicked(link, "Webpedia");
}



void PostsFrame::openPost(const QModelIndex &index)
{
	Post *post = static_cast<Post *>(index.internalPointer());

	if (post->status == Post::unread) {
		PostsArray postsList;
		postsList << post;
		emit action(postsList, Post::read);
	}

	if (QObject::sender()->objectName() == "actionPostExternal") {

		QUrl url(post->link);

		url.removeQueryItem("utm_source");
		url.removeQueryItem("utm_medium");
		url.removeQueryItem("utm_term");
		url.removeQueryItem("utm_content");
		url.removeQueryItem("utm_campaign");
		url.addQueryItem("utm_source", "webpedia");
		url.addQueryItem("utm_medium", "reader");

		QDesktopServices::openUrl(url);
	} else {
		emit linkClicked(QUrl(post->link), post->title);
	}
}



void PostsFrame::openPost()
{
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() > 0) {
		openPost(indexes.at(0));
	}
}



void PostsFrame::actionLinkCopy()
{
	QModelIndexList indexList = list->selectionModel()->selectedRows();
	if (indexList.length() > 0) {
		Post *post = static_cast<Post *>(indexList[0].internalPointer());
		QApplication::clipboard()->setText(post->link);
	}
}
