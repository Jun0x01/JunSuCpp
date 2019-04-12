#include "JunSuQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	JunSuQt w;
	w.show();
	return a.exec();
}
