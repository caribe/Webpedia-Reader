#ifndef WIZARDLIST_H
#define WIZARDLIST_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QStackedWidget>
#include <QTabBar>
#include <QButtonGroup>
#include <QFormLayout>
#include <QLineEdit>

class WizardList : public QDialog
{
    Q_OBJECT
public:
	explicit WizardList(QWidget *parent, QStandardItemModel *model);
	int getSource();

private:
	QTableView *listTable;
	QStackedWidget *stackedContent;
	QLineEdit *searchLineEdit;

signals:
	void listRequest(QString mode);

private slots:
	void buttonClicked(int);
	void searchSubmitted();

public slots:

};

#endif // WIZARDLIST_H
