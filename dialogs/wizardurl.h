#ifndef WIZARDURL_H
#define WIZARDURL_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDialogButtonBox>

class WizardUrl : public QDialog
{
    Q_OBJECT
public:
	explicit WizardUrl(QWidget *parent = 0);
	QString getUrl();

private:
	QLineEdit *urlLineEdit;

signals:

public slots:

};

#endif // WIZARDURL_H
