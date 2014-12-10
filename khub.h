#ifndef KHUB_H
#define KHUB_H

#include <QtWidgets/QMainWindow>
#include "ui_khub.h"

class KHUB : public QMainWindow
{
	Q_OBJECT

public:
	KHUB(QWidget *parent = 0);
	~KHUB();

private:
	Ui::KHUBClass ui;
};

#endif // KHUB_H
