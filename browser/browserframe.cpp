#include "browserframe.h"

BrowserFrame::BrowserFrame(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *browserLayout = new QVBoxLayout();

	QHBoxLayout *browserToolsLayout = new QHBoxLayout();

	QToolButton *browserToolsBack = new QToolButton();
	browserToolsBack->setIcon(QIcon(":/resources/navigation-180-button.png"));
	browserToolsBack->setToolTip("Indietro");
	browserToolsBack->setAutoRaise(true);
	// browser->connect(browserToolsBack, SIGNAL(clicked()), SLOT(back()));
	browserToolsLayout->addWidget(browserToolsBack);

	QToolButton *browserToolsHome = new QToolButton();
	browserToolsHome->setIcon(QIcon(":/resources/home.png"));
	browserToolsHome->setToolTip("Home");
	browserToolsHome->setAutoRaise(true);
	connect(browserToolsHome, SIGNAL(clicked()), SLOT(browserHome()));
	browserToolsLayout->addWidget(browserToolsHome);

	browserToolsLayout->addStretch();

	browserLayout->addLayout(browserToolsLayout);

	tabWidget = new QTabWidget(this);
	tabWidget->setTabsClosable(true);
	connect(tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
	browserLayout->addWidget(tabWidget);

	setLayout(browserLayout);
}


void BrowserFrame::browserHome()
{
	if (tabWidget->count() == 0) {
		openLink(QUrl("http://webpedia.altervista.org/"), "Webpedia");
	} else {
		int currentIndex = tabWidget->currentIndex();
		tabWidget->setTabText(currentIndex, "Webpedia");
		QWebView *browser = static_cast<QWebView *>(tabWidget->widget(currentIndex));
		browser->setUrl(QUrl("http://webpedia.altervista.org/"));
	}
}



void BrowserFrame::openLink(QUrl url, QString title) {
	QWebView *browser = new QWebView();
	browser->setUrl(url);

	tabWidget->addTab(browser, title);
	tabWidget->setCurrentIndex(tabWidget->count()-1);
}


void BrowserFrame::closeTab(int i) {
	tabWidget->removeTab(i);
	if (tabWidget->count() == 0) emit hideSignal();
}
