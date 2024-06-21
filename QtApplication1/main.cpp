#include <QApplication>

#include "FDnMainWidget.h"
#define NUM 32
#define A 1;

int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	FDnMainWidget widget;
	widget.show();
	app.exec();
}