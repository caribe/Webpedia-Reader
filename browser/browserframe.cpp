#include "browserframe.h"

BrowserFrame::BrowserFrame(QWidget *parent) : QWidget(parent)
{
	browser = new QWebView();

	QVBoxLayout *browserLayout = new QVBoxLayout();

	QHBoxLayout *browserToolsLayout = new QHBoxLayout();

	QToolButton *browserToolsBack = new QToolButton();
	browserToolsBack->setIcon(QIcon(":/resources/navigation-180-button.png"));
	browserToolsBack->setToolTip("Indietro");
	browserToolsBack->setAutoRaise(true);
	browser->connect(browserToolsBack, SIGNAL(clicked()), SLOT(back()));
	browserToolsLayout->addWidget(browserToolsBack);

	QToolButton *browserToolsHome = new QToolButton();
	browserToolsHome->setIcon(QIcon(":/resources/home.png"));
	browserToolsHome->setToolTip("Home");
	browserToolsHome->setAutoRaise(true);
	connect(browserToolsHome, SIGNAL(clicked()), SLOT(browserHome()));
	browserToolsLayout->addWidget(browserToolsHome);

	QToolButton *browserToolsClose = new QToolButton();
	browserToolsClose->setIcon(QIcon(":/resources/cross-button.png"));
	browserToolsClose->setToolTip("Close browser");
	browserToolsClose->setAutoRaise(true);
	connect(browserToolsClose, SIGNAL(clicked()), SLOT(browserClose()));
	browserToolsLayout->addWidget(browserToolsClose);

	browserToolsLayout->addStretch();

	browserLayout->addLayout(browserToolsLayout);
	browserLayout->addWidget(browser);

	setLayout(browserLayout);
}


void BrowserFrame::browserHome()
{
	browser->setUrl(QUrl("http://webpedia.altervista.org/"));
}

void BrowserFrame::browserClose()
{
	emit hideSignal();
}


void BrowserFrame::displayPost(Post *post) {
	this->browserTitle->setText(QString("<h3>%1</h3>").arg(post->title));
	this->browser->setHtml("<html><body>"+post->body+"</body></html>");
}
