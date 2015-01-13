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
	textFieldSetup(loginEdit, "KHUB - Mail", 115, 100, 200, 25, false);
	textFieldSetup(passwordEdit, "Password", 115, 150, 200, 25, true);
}

void KHUB::createRegisterScreen()
{

}

void KHUB::createMenu()
{
	fileMenu = menuBar()->addMenu(tr("&File"));

	fileMenu->addAction(exitAct);
	fileMenu->addAction(logoutAct);
	
	groupsMenu = menuBar()->addMenu(tr("&Groups"));

	groupsMenu->addAction(createGroupAct);
	groupsMenu->addAction(findGroupAct);

	searchMenu = menuBar()->addMenu(tr("&Search"));

	searchMenu->addAction(newSearchAct);
}

//Events listener
void KHUB::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
}

//Actions Handler
void KHUB::createActions()
{
	/* Files Handler*/
	exitAct = new QAction(tr("&Exit"), this);
	logoutAct = new QAction(tr("&Logout"), this);

	/* Groups Handler */
	createGroupAct = new QAction(tr("&CreateGroup"), this);
	findGroupAct = new QAction(tr("&FindGroup"), this);

	/* Search Handler */
	newSearchAct = new QAction(tr("&NewSearch"), this);
}

//File Functions
void KHUB::exit()
{

}

void KHUB::logout()
{

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
void KHUB::handleLogin()
{
	createMainScreen();

	SQL databaseConnection;
	databaseConnection.Query();
}

void KHUB::handleRegister()
{
	//createMainScreen();
	HTTP request;

	request.SendRequest();
}

//Code Reuse
void KHUB::buttonSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)())
{
	*button = new QPushButton(name, this);
	(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	//Event Listener
	connect(*button, &QPushButton::released, this, fptr);
}

void KHUB::textFieldSetup(QLineEdit *textField, const QString hint, int posX, int posY, int width, int height, bool isPassword)
{
	textField = new QLineEdit("", this);
	textField->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));
	textField->setAlignment(Qt::AlignHCenter);
	textField->setPlaceholderText(hint);

	if (isPassword)
		textField->setEchoMode(passwordEdit->Password);
}
