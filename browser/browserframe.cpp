#include "browserframe.h"

BrowserFrame::BrowserFrame(QWidget *parent) : QTabWidget(parent)
{
	setTabsClosable(true);
	connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
}


void BrowserFrame::openLink(QUrl url, QString title) {
	// Set utm params
	url.removeQueryItem("utm_source");
	url.removeQueryItem("utm_medium");
	url.removeQueryItem("utm_term");
	url.removeQueryItem("utm_content");
	url.removeQueryItem("utm_campaign");
	url.addQueryItem("utm_source", "webpedia");
	url.addQueryItem("utm_medium", "reader");

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


void BrowserFrame::browserHome()
{
	QWebView *browser = static_cast<QWebView *>(currentWidget());

	if (browser == 0) {
		openLink(QUrl("http://webpedia.altervista.org/"), "Webpedia");
	} else {
		setTabText(currentIndex(), "Webpedia");
		setTabIcon(currentIndex(), QIcon(":/resources/webpedia.svg"));
		browser->setUrl(QUrl("http://webpedia.altervista.org/"));
	}
}


void BrowserFrame::browserBack()
{
	QWebView *browser = static_cast<QWebView *>(currentWidget());
	if (browser != 0) browser->back();
}


void BrowserFrame::closeTab(int i) {
	if (i == -1) i = currentIndex();

	removeTab(i);
	if (count() == 0) emit hideSignal();
}
