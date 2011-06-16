#ifndef BROWSERFRAME_H
#define BROWSERFRAME_H

#include <QTabWidget>
#include <QWebView>
#include <QDebug>

#include "data/post.h"

class BrowserFrame : public QTabWidget
{
	Q_OBJECT

private:

public:
	explicit BrowserFrame(QWidget *parent = 0);

signals:
	void hideSignal();

public slots:
	void openLink(QUrl url, QString title);
	void browserHome();
	void browserBack();

private slots:
	void closeTab(int i);
	void browserLoaded(bool ok);
};

#endif // BROWSERFRAME_H
