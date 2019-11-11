#include "JunSuQt.h"
//#include <QtWidgets/QApplication>
#include <QApplication> // Qt 4.8.6, Linux   ”√

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JunSuQt w;
	w.show();
	return a.exec();
}
