#ifndef OPTIONS_H
#define OPTIONS_H

#include <QDialog>
#include <QTabWidget>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSettings>

class Options : public QDialog
{
    Q_OBJECT
public:
    explicit Options(QWidget *parent = 0);

private:
	QComboBox *generalUpdFreq;
	QComboBox *generalNumber;
	QComboBox *generalOrder;

signals:

private slots:
	void acceptAction();

public slots:

};

#endif // OPTIONS_H
