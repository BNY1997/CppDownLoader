#include <QApplication>

#include "FDnMainWidget.h"

int main(int argc, char **argv)
{

	QApplication app(argc, argv);
	FDnMainWidget widget;
	widget.show();
	app.exec();
}