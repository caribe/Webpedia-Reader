#include "postsframe.h"

PostsFrame::PostsFrame(ModelPosts *postsModel, QWidget *parent) : QSplitter(parent)
{
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

	viewer = new QTextBrowser();
	viewer->setOpenLinks(false);
	connect(viewer, SIGNAL(anchorClicked(QUrl)), SLOT(actionLink(QUrl)));
	viewerLayout->addWidget(viewer);

	QWidget *viewerFrame = new QWidget();
	viewerFrame->setLayout(viewerLayout);

	addWidget(viewerFrame);
}


void PostsFrame::actionGeneric(int mode) {
	PostsArray postsList;
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	foreach (QModelIndex index, indexes) {
		postsList << static_cast<Post *>(index.internalPointer());
	}
	if (postsList.length() > 0) emit action(postsList, mode);
}

void PostsFrame::actionDelete() {
	actionGeneric(0);
}

void PostsFrame::actionLike() {
	actionGeneric(1);
}

void PostsFrame::actionUnlike() {
	actionGeneric(2);
}

void PostsFrame::actionFlag() {
	actionGeneric(3);
}

void PostsFrame::actionUnflag() {
	actionGeneric(4);
}

void PostsFrame::actionRead() {
	actionGeneric(5);
}

void PostsFrame::actionUnread() {
	actionGeneric(6);
}



void PostsFrame::postSelected(const QModelIndex & current, const QModelIndex & previous)
{
	Post *post = (Post *)current.internalPointer();
	viewerTitle->setText(QString("<h3>%1</h3>").arg(post->title));
	viewer->setHtml("<html><body>"+post->body+"</body></html>");
}



void PostsFrame::actionLink(QUrl link)
{
	emit linkClicked(link);
}



void PostsFrame::openPost()
{
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) {
		Post *post = static_cast<Post *>(indexes.at(0).internalPointer());
		emit linkClicked(QUrl(post->link));
	}
}



void PostsFrame::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_R:
		actionRead();
	break;
	case Qt::Key_L:
		actionLike();
	break;
	case Qt::Key_F:
		actionFlag();
	break;
	case Qt::Key_Delete:
		actionDelete();
	break;
	default:
		QSplitter::keyPressEvent(event);
	}
}
