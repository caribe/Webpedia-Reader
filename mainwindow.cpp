#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
{
	QIcon icon(":/resources/webpedia.svg");
	setWindowIcon(icon);

	setWindowTitle(tr("Webpedia Reader"));

	baseUrl = QString("http://webpedia.altervista.org/");

	logged = false;

	// Model

	data = new Source();
	sourcesModel = new ModelSources(data);
	postsModel = new ModelPosts(data);

	sourcesListModel = new QStandardItemModel();

	// Connection

	connection = new Connection(this);
	wizard = new Wizard(this);

	// Central widget

	tree = new SourceFrame(this);
	tree->setModel(sourcesModel);
	connection->connect(tree->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), SLOT(sourceSelected(QModelIndex, QModelIndex)));
	connection->connect(tree, SIGNAL(addFolderSignal(Source*)), SLOT(addFolder(Source *)));
	connection->connect(sourcesModel, SIGNAL(sourceMoved(Source*,Source*,int)), SLOT(moveFolder(Source*,Source*,int)));
	connection->connect(sourcesModel, SIGNAL(sourceRenamed(Source*)), SLOT(renameFolder(Source*)));
	connection->connect(tree, SIGNAL(expandFolderSignal(Source *)), SLOT(folderExpand(Source *)));
	connection->connect(tree, SIGNAL(collapseFolderSignal(Source *)), SLOT(folderCollapse(Source *)));

	postsFrame = new PostsFrame(postsModel, this);
	connection->connect(postsFrame, SIGNAL(action(PostsArray &, int)), SLOT(sendAction(PostsArray &, int)));
	connect(postsFrame, SIGNAL(linkClicked(QUrl)), SLOT(openLink(QUrl)));


	// Browser

	browserFrame = new BrowserFrame(this);
	post = browserFrame->browser;
	post->setUrl(QUrl(baseUrl));
	connect(browserFrame, SIGNAL(hideSignal()), SLOT(hideBrowser()));

	// Global layout

	QSplitter *splitter1 = new QSplitter(Qt::Horizontal);
	QWidget *rightFrame = new QWidget();
	splitter2 = new QStackedLayout();

	splitter2->addWidget(browserFrame);
	splitter2->addWidget(postsFrame);
	rightFrame->setLayout(splitter2);

	splitter1->addWidget(tree);
	splitter1->addWidget(rightFrame);

	this->setCentralWidget(splitter1);

	// Menu bar

	QMenuBar *menuBar = new QMenuBar();

	QMenu *menuFile = new QMenu("&File");

	menuConnect = new QAction(QIcon(":/resources/plug-connect.png"), tr("Connect"), menuFile);
	connect(menuConnect, SIGNAL(triggered()), SLOT(userConnect()));
	menuFile->addAction(menuConnect);

	menuDisconnect = new QAction(QIcon(":/resources/plug-disconnect.png"), tr("Disconnect"), menuFile);
	connect(menuDisconnect, SIGNAL(triggered()), SLOT(userDisconnect()));
	menuFile->addAction(menuDisconnect);

	menuFile->addSeparator();

	QAction *menuFileQuit = new QAction(QIcon(":/resources/cross-button.png"), tr("E&xit"), menuFile);
	menuFileQuit->setShortcut(QKeySequence::Quit);
	menuFile->addAction(menuFileQuit);
	this->connect(menuFileQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	menuBar->addMenu(menuFile);

	QMenu *menuSources = new QMenu("&Sources");

	QAction *menuSourcesUpdate = new QAction(QIcon(":/resources/arrow-circle.png"), tr("&Update all sources"), menuSources);
	connection->connect(menuSourcesUpdate, SIGNAL(triggered()), SLOT(update()));
	menuSources->addAction(menuSourcesUpdate);

	menuSources->addSeparator();

	QAction *menuSourcesAdd = new QAction(QIcon(":/resources/feed-plus.png"), tr("&Add source"), menuSources);
	this->connect(menuSourcesAdd, SIGNAL(triggered()), wizard, SLOT(step0()));
	menuSources->addAction(menuSourcesAdd);

	QAction *menuSourcesAddFolder = new QAction(QIcon(":/resources/folder--plus.png"), tr("Add &folder"), this);
	connect(menuSourcesAddFolder, SIGNAL(triggered()), SLOT(addFolder()));
	menuSources->addAction(menuSourcesAddFolder);

	menuBar->addMenu(menuSources);

	QMenu *menuSettings = new QMenu("Se&ttings");
	menuBar->addMenu(menuSettings);

	QAction *menuSettingsOptions = new QAction(QIcon(":/resources/wrench.png"), tr("&Options"), menuSettings);
	connect(menuSettingsOptions, SIGNAL(triggered()), SLOT(optionsDialog()));
	menuSettings->addAction(menuSettingsOptions);

	QMenu *menuHelp = new QMenu("&Help");

	QAction *actionHelpAbout = new QAction(tr("About Webpedia Reader"), menuHelp);
	connect(actionHelpAbout, SIGNAL(triggered()), SLOT(menuHelpAbout()));
	menuHelp->addAction(actionHelpAbout);

	QAction *actionHelpAboutQt = new QAction(tr("About Qt"), menuHelp);
	connect(actionHelpAboutQt, SIGNAL(triggered()), SLOT(menuHelpAboutQt()));
	menuHelp->addAction(actionHelpAboutQt);

	menuBar->addMenu(menuHelp);

	this->setMenuBar(menuBar);

	// Tray Icon

	trayIcon = new QSystemTrayIcon(icon, this);
	trayIcon->setToolTip(tr("Webpedia Reader"));
	trayIcon->show();

	trayIconMenu = new QMenu();

	QAction *trayUpdate = new QAction(QIcon(":/resources/arrow-circle.png"), tr("&Update all sources"), trayIconMenu);
	connection->connect(trayUpdate, SIGNAL(triggered()), SLOT(update()));
	trayIconMenu->addAction(trayUpdate);

	QAction *trayQuit = new QAction(QIcon(":/resources/cross-button.png"), tr("&Exit"), trayIconMenu);
	connect(trayQuit, SIGNAL(triggered()), qApp, SLOT(quit()));
	trayIconMenu->addAction(trayQuit);

	trayIcon->setContextMenu(trayIconMenu);
	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	// Status bar

	statusLabel = new QLabel();
	statusLabel->setFrameShape(QFrame::NoFrame);
	statusBar()->addWidget(statusLabel);
	statusBar()->setStyleSheet("QStatusBar::item { border: 0 }");
	setStatus(tr("Ready"), false);

	// Poller

	poller = new QTimer(this);
	connection->connect(poller, SIGNAL(timeout()), SLOT(update()));
}


void MainWindow::init() {
	// Session recovery or Login

	QSettings settings;
	user = settings.value("user/user").toString();
	poller->start(settings.value("settings/updateFrequency", 60).toInt() * 60000);

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


void MainWindow::userConnect() {
	Login *login = new Login();
	if (login->exec() == QDialog::Accepted) {
		connection->login(login->username->text(), login->password->text());
	}
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


void MainWindow::openLink(QUrl link) {
	post->setUrl(link);
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
}


void MainWindow::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}


void MainWindow::setStatus(QString label, bool busy) {
	statusLabel->setText((busy ? "<img src=\":/resources/network-status-busy.png\"/> " : "<img src=\":/resources/network-status.png\"/> ")+label);
}
