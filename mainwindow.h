#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QHeaderView>
#include <QSplitter>
#include <QMenuBar>
#include <QAction>
#include <QToolButton>
#include <QSettings>
#include <QSystemTrayIcon>
#include <QStatusBar>
#include <QTimer>

class MainWindow;

#include "connection.h"
#include "sources/sourceframe.h"
#include "posts/postsframe.h"
#include "browser/browserframe.h"

#include "dialogs/wizard.h"
#include "dialogs/login.h"
#include "dialogs/options.h"

#include "model/modelsources.h"
#include "model/modelposts.h"
#include "data/source.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
	QMenu *trayIconMenu;

public:
	MainWindow(QWidget *parent = 0);
	void init();
	void closeEvent(QCloseEvent *event);
	void setStatus(QString label, bool busy);

	QSystemTrayIcon *trayIcon;

	Source *data;
	ModelSources *sourcesModel;
	ModelPosts *postsModel;

	QStandardItemModel *sourcesListModel;
	SourceFrame *tree;
	Wizard *wizard;
	Connection *connection;
	QString baseUrl;
	PostsFrame *postsFrame;
	BrowserFrame *browserFrame;

	QSplitter *splitter1;
	QStackedLayout *splitter2;

	QAction *menuConnect, *menuDisconnect;

	QString username;
	QString token;
	QString user;

	QLabel *statusLabel;

	QTimer *poller;

	bool logged;

private slots:
	void addFolder();
	void openLink(QUrl link, QString title);
	void optionsDialog();
	void hideBrowser();
	void menuHelpAbout();
	void menuHelpAboutQt();
	void onBeforeQuit();

public slots:
	void iconActivated(QSystemTrayIcon::ActivationReason);
	void userConnect();
	void userDisconnect();
};

#endif // MAINWINDOW_H
