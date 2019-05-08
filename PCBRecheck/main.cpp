#include "PCBRecheck.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PCBRecheck w;
	return a.exec();
}
