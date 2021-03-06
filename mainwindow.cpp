#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	QIcon icon(":/resources/webpedia.svg");
	setWindowIcon(icon);

	setWindowTitle(tr("Webpedia Reader"));

	baseUrl = QString("http://webpedia.altervista.org/");

	logged = false;
	login = NULL;

	// Model

	data = new Source();
	sourcesModel = new ModelSources(data);
	postsModel = new ModelPosts(data);

	sourcesListModel = new QStandardItemModel();

	// Connection

	connection = new Connection(this);

	// Actions

	actionPostView = new QAction(QIcon(":/resources/eye.png"), tr("View post in browser"), this);
	actionPostView->setObjectName("actionPostView");
	actionPostView->setShortcut(Qt::Key_V);

	actionPostExternal = new QAction(QIcon(":/resources/external.png"), tr("Open in default browser"), this);
	actionPostExternal->setObjectName("actionPostExternal");
	actionPostExternal->setShortcut(Qt::Key_X);

	actionPostCopy = new QAction(QIcon(":/resources/clipboard-paste.png"), tr("Copy post address"), this);
	actionPostCopy->setObjectName("actionPostCopy");

	actionPostRead = new QAction(QIcon(":/resources/mail-open.png"), tr("Set as read"), this);
	actionPostRead->setObjectName("actionPostRead");
	actionPostRead->setShortcut(Qt::Key_R);

	actionPostUnread = new QAction(QIcon(":/resources/mail.png"), tr("Set as unread"), this);
	actionPostUnread->setObjectName("actionPostUnread");
	actionPostUnread->setShortcut(Qt::Key_U);

	actionPostFlag = new QAction(QIcon(":/resources/flag.png"), tr("Flag this post"), this);
	actionPostFlag->setObjectName("actionPostFlag");
	actionPostFlag->setShortcut(Qt::Key_F);

	actionPostLike = new QAction(QIcon(":/resources/heart.png"), tr("Like this post"), this);
	actionPostLike->setObjectName("actionPostLike");
	actionPostLike->setShortcut(Qt::Key_L);

	actionPostDelete = new QAction(QIcon(":/resources/cross.png"), tr("Delete this post"), this);
	actionPostDelete->setObjectName("actionPostDelete");
	actionPostDelete->setShortcut(Qt::Key_Delete);

	actionRefresh = new QAction(QIcon(":/resources/arrow-circle.png"), tr("&Update all sources"), this);
	actionRefresh->setObjectName("actionRefresh");
	actionRefresh->setShortcut(Qt::CTRL + Qt::Key_F5);

	actionQuit = new QAction(QIcon(":/resources/cross-button.png"), tr("E&xit"), this);
	actionQuit->setObjectName("actionQuit");
	actionQuit->setShortcut(QKeySequence::Quit);

	actionBrowserShow = new QAction(QIcon(":/resources/application-browser.png"), tr("Show browser"), this);
	actionBrowserShow->setShortcut(Qt::CTRL + Qt::Key_B);

	actionBrowserBack = new QAction(QIcon(":/resources/arrow-180.png"), tr("&Back"), this);
	actionBrowserBack->setShortcut(QKeySequence::Back);

	actionBrowserHome = new QAction(QIcon(":/resources/home.png"), tr("&Home"), this);
	actionBrowserHome->setShortcut(Qt::CTRL + Qt::Key_H);

	actionBrowserCloseTab = new QAction(tr("Close Tab"), this);
	actionBrowserCloseTab->setShortcut(QKeySequence::Close);

	actionHideMenubar = new QAction(tr("Hide menu bar"), this);
	actionHideMenubar->setCheckable(true);
	actionHideMenubar->setShortcut(Qt::CTRL + Qt::Key_M);

	actionAddSourceById = new QAction(QIcon(":/resources/feed-plus.png"), tr("Add Source"), this);


	addAction(actionHideMenubar);

	// Browser

	browserFrame = new BrowserFrame(this);
	browserFrame->openLink(QUrl(baseUrl), "Webpedia");

	connect(browserFrame, SIGNAL(hideSignal()), SLOT(hideBrowser()));

	// Sources

	tree = new SourceFrame(this);
	tree->setModel(sourcesModel);

	connection->connect(sourcesModel, SIGNAL(sourceMoved(Source*,Source*,int)), SLOT(moveFolder(Source*,Source*,int)));
	connection->connect(sourcesModel, SIGNAL(sourceRenamed(Source*)), SLOT(renameFolder(Source*)));
	connection->connect(tree->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(sourceSelected(QModelIndex,QModelIndex)));
	connect(tree, SIGNAL(clicked(QModelIndex)), SLOT(hideBrowser()));
	connection->connect(tree, SIGNAL(addFolderSignal(Source*)), SLOT(addFolder(Source *)));
	connection->connect(tree, SIGNAL(expandFolderSignal(Source *)), SLOT(folderExpand(Source *)));
	connection->connect(tree, SIGNAL(collapseFolderSignal(Source *)), SLOT(folderCollapse(Source *)));

	// Posts

	postsFrame = new PostsFrame(postsModel, this);
	connection->connect(postsFrame, SIGNAL(action(PostsArray &, Post::Status)), SLOT(sendAction(PostsArray &, Post::Status)));
	connect(postsFrame, SIGNAL(linkClicked(QUrl, QString)), SLOT(openLink(QUrl, QString)));

	// Global layout

	splitter1 = new QSplitter(Qt::Horizontal);
	QWidget *rightFrame = new QWidget();
	splitter2 = new QStackedLayout();

	splitter2->addWidget(browserFrame);
	splitter2->addWidget(postsFrame);
	rightFrame->setLayout(splitter2);

	splitter1->addWidget(tree);
	splitter1->addWidget(rightFrame);

	this->setCentralWidget(splitter1);

	// Actions connections

	postsFrame->connect(actionPostRead, SIGNAL(triggered()), SLOT(actionPost()));
	postsFrame->connect(actionPostUnread, SIGNAL(triggered()), SLOT(actionPost()));
	postsFrame->connect(actionPostFlag, SIGNAL(triggered()), SLOT(actionPost()));
	postsFrame->connect(actionPostLike, SIGNAL(triggered()), SLOT(actionPost()));
	postsFrame->connect(actionPostDelete, SIGNAL(triggered()), SLOT(actionPost()));
	postsFrame->connect(actionPostCopy, SIGNAL(triggered()), SLOT(actionLinkCopy()));
	postsFrame->connect(actionPostView, SIGNAL(triggered()), SLOT(openPost()));
	postsFrame->connect(actionPostExternal, SIGNAL(triggered()), SLOT(openPost()));

	connect(actionBrowserShow, SIGNAL(triggered()), SLOT(showBrowser()));
	browserFrame->connect(actionBrowserBack, SIGNAL(triggered()), SLOT(browserBack()));
	browserFrame->connect(actionBrowserHome, SIGNAL(triggered()), SLOT(browserHome()));
	browserFrame->connect(actionBrowserCloseTab, SIGNAL(triggered()), SLOT(closeTab()));

	connection->connect(actionRefresh, SIGNAL(triggered()), SLOT(update()));
	connect(actionQuit, SIGNAL(triggered()), SLOT(onBeforeQuit()));

	connect(actionHideMenubar, SIGNAL(triggered()), SLOT(hideMenuBar()));
	connect(postsFrame->list, SIGNAL(signalAddSourceById(int)), connection, SLOT(sourceAdd(int)));

	// Menu barregisterLayout

	menuBar = new QMenuBar();

	QMenu *menuFile = new QMenu(tr("&File"));

	menuConnect = new QAction(QIcon(":/resources/plug-connect.png"), tr("Connect"), menuFile);
	connect(menuConnect, SIGNAL(triggered()), SLOT(userConnect()));
	menuFile->addAction(menuConnect);

	menuDisconnect = new QAction(QIcon(":/resources/plug-disconnect.png"), tr("Disconnect"), menuFile);
	connect(menuDisconnect, SIGNAL(triggered()), SLOT(userDisconnect()));
	menuFile->addAction(menuDisconnect);

	menuFile->addSeparator();

	menuFile->addAction(actionQuit);


	QMenu *menuView = new QMenu(tr("&Browser"));

	menuView->addAction(actionBrowserShow);
	menuView->addAction(actionBrowserBack);
	menuView->addAction(actionBrowserHome);
	menuView->addAction(actionBrowserCloseTab);


	QMenu *menuSources = new QMenu(tr("&Source"));

	menuSources->addAction(actionRefresh);

	menuSources->addSeparator();

	QAction *menuSourcesAddUrl = new QAction(QIcon(":/resources/feed-plus.png"), tr("&Add source by URL"), menuSources);
	connect(menuSourcesAddUrl, SIGNAL(triggered()), SLOT(addSourceByUrl()));
	menuSources->addAction(menuSourcesAddUrl);

	QAction *menuSourcesAddList = new QAction(QIcon(":/resources/feed-plus.png"), tr("&Add source"), menuSources);
	connect(menuSourcesAddList, SIGNAL(triggered()), SLOT(addSourceByList()));
	menuSources->addAction(menuSourcesAddList);

	QAction *menuSourcesAddFolder = new QAction(QIcon(":/resources/folder--plus.png"), tr("Add &folder"), this);
	connect(menuSourcesAddFolder, SIGNAL(triggered()), SLOT(addFolder()));
	menuSources->addAction(menuSourcesAddFolder);


	QMenu *menuPosts = new QMenu(tr("&Post"));

	menuPosts->addAction(actionPostRead);
	menuPosts->addAction(actionPostUnread);
	menuPosts->addAction(actionPostFlag);
	menuPosts->addAction(actionPostLike);
	menuPosts->addAction(actionPostDelete);
	menuPosts->addSeparator();
	menuPosts->addAction(actionPostView);
	menuPosts->addAction(actionPostExternal);
	menuPosts->addSeparator();
	menuPosts->addAction(actionPostCopy);

	QMenu *menuSettings = new QMenu(tr("Se&ttings"));

	QAction *menuSettingsOptions = new QAction(QIcon(":/resources/wrench.png"), tr("&Options"), menuSettings);
	connect(menuSettingsOptions, SIGNAL(triggered()), SLOT(optionsDialog()));
	menuSettings->addAction(menuSettingsOptions);

	menuSettings->addAction(actionHideMenubar);

	QMenu *menuHelp = new QMenu(tr("&Help"));

	QAction *actionHelpAbout = new QAction(tr("About Webpedia Reader"), menuHelp);
	connect(actionHelpAbout, SIGNAL(triggered()), SLOT(menuHelpAbout()));
	menuHelp->addAction(actionHelpAbout);

	QAction *actionHelpAboutQt = new QAction(tr("About Qt"), menuHelp);
	connect(actionHelpAboutQt, SIGNAL(triggered()), SLOT(menuHelpAboutQt()));
	menuHelp->addAction(actionHelpAboutQt);


	menuBar->addMenu(menuFile);
	menuBar->addMenu(menuSources);
	menuBar->addMenu(menuPosts);
	menuBar->addMenu(menuView);
	menuBar->addMenu(menuSettings);
	menuBar->addMenu(menuHelp);

	this->setMenuBar(menuBar);

	// Toolbar

	QToolBar *toolbarRefresh = new QToolBar(tr("Refresh Toolbar"), this);
	toolbarRefresh->addAction(actionRefresh);

	QToolBar *toolbarPosts = new QToolBar(tr("Posts Toolbar"), this);
	toolbarPosts->addAction(actionPostRead);
	toolbarPosts->addAction(actionPostUnread);
	toolbarPosts->addAction(actionPostFlag);
	toolbarPosts->addAction(actionPostLike);
	toolbarPosts->addAction(actionPostDelete);

	QToolBar *toolbarBrowser = new QToolBar(tr("Browser Toolbar"), this);
	toolbarBrowser->addAction(actionBrowserBack);
	toolbarBrowser->addAction(actionBrowserHome);
	toolbarBrowser->addAction(actionBrowserShow);

	addToolBar(toolbarRefresh);
	addToolBar(toolbarPosts);
	addToolBar(toolbarBrowser);


	// Tray Icon

	trayIcon = new QSystemTrayIcon(icon, this);
	trayIcon->setToolTip(tr("Webpedia Reader"));
	trayIcon->show();

	trayIconMenu = new QMenu();

	trayIconMenu->addAction(actionRefresh);
	trayIconMenu->addAction(actionQuit);

	trayIcon->setContextMenu(trayIconMenu);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	// Status bar

	statusLabel = new QLabel();
	statusLabel->setFrameShape(QFrame::NoFrame);
	statusBar()->addWidget(statusLabel);
	statusBar()->setStyleSheet("QStatusBar::item { border: 0 }");
	setStatus(tr("Ready"), false);
}



void MainWindow::init() {
	// Session recovery or Login

	QSettings settings;

	// Window State

	move(settings.value("state/position", QPoint(0, 0)).toPoint());
	resize(settings.value("state/size", QSize(800, 600)).toSize());
	if (settings.value("state/maximized", false).toBool() == true) showMaximized();

	if (settings.contains("state/hsplitter")) {
		QList<int> hSizeList;
		foreach (QVariant size, settings.value("state/hsplitter").toList())
			hSizeList << size.toInt();
		splitter1->setSizes(hSizeList);
	}

	if (settings.contains("state/vsplitter")) {
		QList<int> vSizeList;
		foreach (QVariant size, settings.value("state/vsplitter").toList())
			vSizeList << size.toInt();
		postsFrame->setSizes(vSizeList);
	}

	if (settings.contains("state/sourcecols")) {
		QVariantList sourceCols = settings.value("state/sourcecols").toList();
		for (int i = 0; i < sourceCols.length(); i++) {
			tree->header()->resizeSection(i, sourceCols[i].toInt());
		}

		QVariantList sourceSizes;
		for (int i = 0; i < tree->header()->count(); i++)
			sourceSizes << tree->header()->sectionSize(i);
		settings.setValue("state/sourcecols", sourceSizes);
	}


	// Poller

	user = settings.value("user/user").toString();
	language = settings.value("user/language", "en_US").toString();

	// Settings

	if (user.isEmpty()) {
		menuConnect->setVisible(true);
		menuDisconnect->setVisible(false);
		userConnect();
	} else {
		username = settings.value("user/username").toString();
		token = settings.value("user/token").toString();
		menuConnect->setVisible(false);
		menuDisconnect->setVisible(true);
		logged = true;
		connection->update();
	}

	poller = new QTimer(this);
	poller->start(settings.value("settings/updateFrequency", 60).toInt() * 60000);
	connection->connect(poller, SIGNAL(timeout()), SLOT(update()));

}


void MainWindow::menuHelpAboutQt() {
	QMessageBox::aboutQt(this);
}


void MainWindow::menuHelpAbout() {
	QMessageBox::about(this, tr("About"), tr("Webpedia Reader is developed by Vincenzo Buttazzo."));
}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
		case QSystemTrayIcon::Trigger:
		case QSystemTrayIcon::DoubleClick:
			if (isVisible()) {
				hide();
			} else {
				show();
			}
		case QSystemTrayIcon::Unknown:
		case QSystemTrayIcon::MiddleClick:
		case QSystemTrayIcon::Context:
		return;
	}
}


void MainWindow::userConnect(Login::Forms form) {
	if (login == NULL) {
		login = new Login(this);
		connect(login, SIGNAL(loginRequest(QString,QString)), SLOT(loginRequest(QString,QString)));
		connect(login, SIGNAL(registerRequest(QString,QString)), SLOT(registerRequest(QString,QString)));
	}
	login->setForm(form);
	login->open();
}


void MainWindow::userDisconnect() {
	QSettings settings;
	settings.remove("user");
	settings.sync();

	username.clear();
	token.clear();
	user.clear();

	sourcesModel->clear();

	menuConnect->setVisible(true);
	menuDisconnect->setVisible(false);
}


void MainWindow::addFolder() {
	connection->addFolder(0);
}


void MainWindow::openLink(QUrl link, QString title) {
	browserFrame->openLink(link, title);
	splitter2->setCurrentIndex(0);
}


void MainWindow::hideBrowser() {
	splitter2->setCurrentIndex(1);
}


void MainWindow::optionsDialog() {
	Options *options = new Options();
	if (options->exec() == QDialog::Accepted) {
		QSettings settings;
		poller->setInterval(settings.value("settings/updateFrequency", 60).toInt() * 60000);
	}
	delete options;
}


void MainWindow::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}


void MainWindow::setStatus(QString label, bool busy) {
	statusLabel->setText((busy ? "<img src=\":/resources/network-status-busy.png\"/> " : "<img src=\":/resources/network-status.png\"/> ")+label);
}

void MainWindow::onBeforeQuit() {

	QSettings settings;
	settings.setValue("state/position", pos());
	settings.setValue("state/size", size());
	settings.setValue("state/maximized", isMaximized());

	QVariantList hSizeList;
	foreach (int size, splitter1->sizes()) hSizeList << size;
	settings.setValue("state/hsplitter", hSizeList);

	QVariantList sourceSizes;
	for (int i = 0; i < tree->header()->count(); i++)
		sourceSizes << tree->header()->sectionSize(i);
	settings.setValue("state/sourcecols", sourceSizes);

	QVariantList postSizes;
	foreach (int size, postsFrame->sizes()) postSizes << size;
	settings.setValue("state/vsplitter", postSizes);

	qApp->quit();
}


void MainWindow::addSourceByUrl()
{
	WizardUrl *wizardUrl = new WizardUrl(this);
	if (wizardUrl->exec() == QDialog::Accepted) {
		connection->addSource(wizardUrl->getUrl());
	}
	delete wizardUrl;
}


void MainWindow::addSourceByList()
{
	WizardList *wizardList = new WizardList(this, sourcesListModel);
	connection->connect(wizardList, SIGNAL(listRequest(QString)), SLOT(sendGetRequest(QString)));
	connection->sendGetRequest("sources.list");

	if (wizardList->exec() == QDialog::Accepted) {
		connection->sourceAdd(wizardList->getSource());
	}

	delete wizardList;
}



void MainWindow::showBrowser()
{
	splitter2->setCurrentIndex(0);
}


void MainWindow::hideMenuBar()
{
	if (actionHideMenubar->isChecked()) {
		menuBar->hide();
	} else {
		menuBar->show();
	}
}


void MainWindow::registerRequest(const QString &username, const QString &email) {
	Connection::ParamHash data;
	data["username"] = username;
	data["email"] = email;
	connection->sendAuthRequest("account.register", data);
}


void MainWindow::loginRequest(const QString &username, const QString &password) {
	Connection::ParamHash data;
	data["username"] = username;
	data["password"] = password;
	connection->sendAuthRequest("account.login", data);
}
