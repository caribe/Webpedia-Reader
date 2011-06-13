#include "browserframe.h"

BrowserFrame::BrowserFrame(QWidget *parent) : QTabWidget(parent)
{
	setTabsClosable(true);
	connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
}


void BrowserFrame::browserHome()
{
	if (count() == 0) {
		openLink(QUrl("http://webpedia.altervista.org/"), "Webpedia");
	} else {
		setTabText(currentIndex(), "Webpedia");
		QWebView *browser = static_cast<QWebView *>(widget(currentIndex()));
		browser->setUrl(QUrl("http://webpedia.altervista.org/"));
	}
}



void BrowserFrame::openLink(QUrl url, QString title) {
	QWebView *browser = new QWebView(this);
	browser->setUrl(url);
	connect(browser, SIGNAL(loadFinished(bool)), SLOT(browserLoaded(bool)));

	addTab(browser, title);
	setCurrentIndex(count()-1);
}


void BrowserFrame::browserLoaded(bool ok) {
	if (ok) {
		QWebView *browser = static_cast<QWebView *>(QObject::sender());
		if (browser) for (int i = 0; i < count(); i++) {
			if (widget(i) == browser) {
				setTabText(i, browser->title());
				break;
			}
		}
	}
}


void BrowserFrame::closeTab(int i) {
	removeTab(i);
	if (count() == 0) emit hideSignal();
}
