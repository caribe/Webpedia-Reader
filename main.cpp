#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("Webpedia");
	QCoreApplication::setOrganizationDomain("webpedia.altervista.org");
	QCoreApplication::setApplicationName("Reader");

    MainWindow w;
    w.show();
	w.init();

    return a.exec();
}
