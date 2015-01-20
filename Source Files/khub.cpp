/**********************************************************************/
/*                                                                    */
/*         Subject: KHUB Thesis										  */
/*		   Author: Victor Rocha                                       */
/*																	  */
/*		 The name of this tool is Knowledge HUB (KHUB) and			  */
/*		 its purpose is educational. The idea is to share			  */
/*		 references (knowledge) between students inside or			  */
/*		 outside the classroom, thus helping students'				  */
/*		 learning or professors' teaching.							  */
/*                                                                    */
/**********************************************************************/


#include "stdafx.h"

#include "khub.h"


KHUB::KHUB(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void KHUB::createMainScreen()
{
	//Sets main full screen and shows
	mainWindow = new KHUB();

	mainWindow->setWindowState(mainWindow->windowState() ^ Qt::WindowMaximized);

	//Remove default empty toolbar
	QToolBar* tb = mainWindow->findChild<QToolBar *>();
		mainWindow->removeToolBar(tb);

	mainWindow->createActions();
	mainWindow->createMenu();
	mainWindow->show();
}

void KHUB::createLoginScreen(KHUB *loginWindow)
{
	loginWindow->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	loginWindow->setFixedSize(400, 300);

	//Remove default empty toolbar
	QToolBar* tb = loginWindow->findChild<QToolBar *>();
		loginWindow->removeToolBar(tb);
	
	//Buttons Setup
	buttonSetup(&loginButton, "Login", 100, 200, 100, 25, &KHUB::handleLogin);
	buttonSetup(&registerButton, "Register", 225, 200, 100, 25, &KHUB::handleRegister);
	
	//Text Fields Setup
	textFieldSetup(&loginEdit, "KHUB E-mail", 115, 100, 200, 25, false);
	textFieldSetup(&passwordEdit, "Password", 115, 150, 200, 25, true);

	loginWindowPtr = loginWindow;
}

void KHUB::createRegisterScreen()
{	
	setFixedSize(400, 400);

	loginButton->close();
	registerButton->close();

	//Buttons Setup
	buttonSetup(&cancelButton, "Cancel", 225, 300, 100, 25, &KHUB::handleCancel);
	buttonSetup(&registerButton, "Register", 100, 300, 100, 25, &KHUB::handleRegister);

	//Text Fields Setup
	textFieldSetup(&loginEdit, "KHUB E-mail", 115, 100, 200, 25, false);
	textFieldSetup(&loginConfirmEdit, "Confirm E-mail", 115, 150, 200, 25, false);
	textFieldSetup(&passwordEdit, "Password", 115, 200, 200, 25, true);
	textFieldSetup(&passwordConfirmEdit, "Confirm Password", 115, 250, 200, 25, true);
}

void KHUB::createMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
		fileMenu->addAction(logoutAct);
		fileMenu->addAction(exitAct);
	
	groupsMenu = menuBar()->addMenu(tr("&Groups"));
		groupsMenu->addAction(createGroupAct);
		groupsMenu->addAction(findGroupAct);

	searchMenu = menuBar()->addMenu(tr("&Search"));
		searchMenu->addAction(newSearchAct);
}

//Actions Handler
void KHUB::createActions()
{
	/* Files Handler*/
	logoutAct = new QAction(tr("&Logout"), this);
	exitAct = new QAction(tr("&Exit"), this);

	connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
	connect(logoutAct, SIGNAL(triggered()), this, SLOT(logout()));

	/* Groups Handler */
	createGroupAct = new QAction(tr("&CreateGroup"), this);
	findGroupAct = new QAction(tr("&FindGroup"), this);

	/* Search Handler */
	newSearchAct = new QAction(tr("&NewSearch"), this);
}

//File Functions
void KHUB::exit()
{
	QApplication::quit();
}

void KHUB::logout()
{
	//Restart
	qApp->quit();
	QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

//Groups Functions
void KHUB::createGroup()
{

}

void KHUB::findGroup()
{

}

//Search Functions
void KHUB::newSearch()
{

}

//Listeners
void KHUB::handleCancel()
{	
	loginWindowPtr->close();

	loginWindowPtr = new KHUB();
		loginWindowPtr->createLoginScreen(loginWindowPtr);
		loginWindowPtr->show();
}

void KHUB::handleLogin()
{
	loginWindowPtr->close();
	
	loginWindowPtr = NULL;
		delete loginWindowPtr;

	SQL databaseConnection;
	
	if (databaseConnection.checkCredentials())
		createMainScreen();
	else
		qDebug() << "falhou";
}

void KHUB::handleRegister()
{
	loginWindowPtr->hide();

	createRegisterScreen();

	loginWindowPtr->repaint();
	loginWindowPtr->show();
}

//Code Reuse
void KHUB::buttonSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)())
{
	*button = new QPushButton(name, this);
	(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	//Event Listener
	connect(*button, &QPushButton::released, this, fptr);
}

void KHUB::textFieldSetup(QLineEdit **textField, const QString hint, int posX, int posY, int width, int height, bool isPassword)
{
	*textField = new QLineEdit("", this);
	(*textField)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));
	(*textField)->setAlignment(Qt::AlignHCenter);
	(*textField)->setPlaceholderText(hint);

	if (isPassword)
		(*textField)->setEchoMode(passwordEdit->Password);
}
