#ifndef BROWSERFRAME_H
#define BROWSERFRAME_H

#include <QWebView>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>

#include "data/post.h"

class BrowserFrame : public QWidget
{
	Q_OBJECT

private:
	QTabWidget *tabWidget;

public:
	explicit BrowserFrame(QWidget *parent = 0);

signals:
	void hideSignal();

public slots:
	void openLink(QUrl url, QString title);

private slots:
	void browserHome();
	void closeTab(int i);
};

#endif // BROWSERFRAME_H
