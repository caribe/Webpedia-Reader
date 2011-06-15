#ifndef WIZARDLIST_H
#define WIZARDLIST_H

#include <QDialog>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QStandardItemModel>
#include <QHeaderView>

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

public slots:

};

#endif // WIZARDLIST_H
