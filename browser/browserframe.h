#ifndef BROWSERFRAME_H
#define BROWSERFRAME_H

#include <QWebView>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "data/post.h"

class BrowserFrame : public QWidget
{
	Q_OBJECT

private:
	QLabel *browserTitle;

public:
	explicit BrowserFrame(QWidget *parent = 0);
	QWebView *browser;
	void displayPost(Post *post);

signals:
	void hideSignal();

private slots:
	void browserHome();
	void browserClose();
};

#endif // BROWSERFRAME_H
