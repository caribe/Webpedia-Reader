#include "postsframe.h"

PostsFrame::PostsFrame(ModelPosts *postsModel, MainWindow *parent) : QSplitter(parent)
{
	mainWindow = parent;

	setOrientation(Qt::Vertical);

	list = new PostsList(this);
	list->setModel(postsModel);

	addWidget(list);

	connect(list->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(postSelected(QModelIndex, QModelIndex)));

	// *** Viewer

	QVBoxLayout *viewerLayout = new QVBoxLayout();

	QHBoxLayout *viewerTitleLayout = new QHBoxLayout();

	viewerTitle = new QLabel();
	viewerTitle->setWordWrap(true);
	viewerTitleLayout->addWidget(viewerTitle, 1);

	QToolButton *viewerTitleButton = new QToolButton();
	viewerTitleButton->setIcon(QIcon(":/resources/eye.png"));
	viewerTitleButton->setToolTip(tr("Open post in browser"));
	viewerTitleButton->setAutoRaise(true);
	connect(viewerTitleButton, SIGNAL(clicked()), SLOT(openPost()));
	viewerTitleLayout->addWidget(viewerTitleButton);

	viewerLayout->addLayout(viewerTitleLayout);

	viewer = new QWebView();
	viewer->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
	connect(viewer, SIGNAL(linkClicked(QUrl)), SLOT(actionLink(QUrl)));
	viewerLayout->addWidget(viewer);

	QWidget *viewerFrame = new QWidget();
	viewerFrame->setLayout(viewerLayout);

	addWidget(viewerFrame);
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



void PostsFrame::postSelected(const QModelIndex & current, const QModelIndex & previous)
{
	Post *post = static_cast<Post *>(current.internalPointer());
	viewerTitle->setText(QString("<h3>%1</h3>").arg(post->title));
	viewer->setHtml("<!DOCTYPE html>\n<html>\n<body>\n"+post->body+"</body>\n</html>\n");
}



void PostsFrame::actionLink(QUrl link)
{
	emit linkClicked(link, "Webpedia");
}



void PostsFrame::openPost()
{
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() > 0) {
		Post *post = static_cast<Post *>(indexes.at(0).internalPointer());

		if (post->status == Post::unread) {
			PostsArray postsList;
			postsList << post;
			emit action(postsList, Post::read);
		}

		emit linkClicked(QUrl(post->link), post->title);
	}
}
