#include <QtWidgets/QApplication>
#include "PCBRecheck.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	PCBRecheck w;
	return a.exec();
}
