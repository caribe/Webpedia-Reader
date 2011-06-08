#ifndef WIZARD_H
#define WIZARD_H

#include <QDialog>
#include <QPushButton>
#include <QStackedLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableView>

class Wizard;

#include "mainwindow.h"

class Wizard : public QDialog
{
    Q_OBJECT
public:
	explicit Wizard(MainWindow *parent = 0);

private:
	QStackedLayout *layout;
	QLineEdit *w10;
	MainWindow *mainWindow;
	QTableView *listTable;

signals:

public slots:
	void step0();
	void step1a();
	void step1b();
	void stepListMode();
	void stepListAccept();

};

#endif // WIZARD_H
