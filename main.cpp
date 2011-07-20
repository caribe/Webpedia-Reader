#include <QApplication>
#include <QTranslator>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("Webpedia");
	QCoreApplication::setOrganizationDomain("webpedia.altervista.org");
	QCoreApplication::setApplicationName("Reader");
	QCoreApplication::setApplicationVersion("1.0.0");

	QTranslator translator;
	translator.load(":/languages/lang-it");
	a.installTranslator(&translator);

    MainWindow w;
    w.show();
	w.init();

    return a.exec();
}
