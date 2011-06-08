#include "postsframe.h"

PostsFrame::PostsFrame(ModelPosts *postsModel, QWidget *parent) : QSplitter(parent)
{
	setOrientation(Qt::Vertical);

	list = new PostsList(this);
	list->setModel(postsModel);

	addWidget(list);

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

	// *** Connections

	connect(list->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(postSelected(QModelIndex, QModelIndex)));
}


void PostsFrame::actionDelete() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 0);
}

void PostsFrame::actionLike() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 1);
}

void PostsFrame::actionUnlike() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 2);
}

void PostsFrame::actionFlag() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 3);
}

void PostsFrame::actionUnflag() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 4);
}

void PostsFrame::actionRead() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 5);
}

void PostsFrame::actionUnread() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) emit action(static_cast<Post *>(indexes.at(0).internalPointer()), 6);
}

void PostsFrame::postSelected(const QModelIndex & current, const QModelIndex & previous) {
	Post *post = (Post *)current.internalPointer();
	viewerTitle->setText(QString("<h3>%1</h3>").arg(post->title));
	viewer->setHtml("<html><body>"+post->body+"</body></html>");
}

void PostsFrame::actionLink(QUrl link) {
	emit linkClicked(link);
}

void PostsFrame::openPost() {
	QModelIndexList indexes = list->selectionModel()->selectedRows();
	if (indexes.length() == 1) {
		Post *post = static_cast<Post *>(indexes.at(0).internalPointer());
		emit linkClicked(QUrl(post->link));
	}
}

void PostsFrame::keyPressEvent(QKeyEvent *event) {
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
