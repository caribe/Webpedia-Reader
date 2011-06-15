#ifndef WIZARDLIST_H
#define WIZARDLIST_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QStackedLayout>
#include <QTabBar>
#include <QButtonGroup>

class WizardList : public QDialog
{
    Q_OBJECT
public:
	explicit WizardList(QWidget *parent, QStandardItemModel *model);
	int getSource();

private:
	QTableView *listTable;

signals:
	void listRequest(QString mode);

private slots:
	void buttonClicked(int);

public slots:

};

#endif // WIZARDLIST_H
