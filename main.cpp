#include "stdafx.h"
#include "khub.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	KHUB w;
	w.show();
	return a.exec();
}
