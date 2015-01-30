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

/*****************/
/* Screens Setup */
/*****************/

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
	signalMapper = new QSignalMapper(this);

	loginWindow->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	loginWindow->setFixedSize(400, 300);

	QToolBar* tb = loginWindow->findChild<QToolBar *>();
		loginWindow->removeToolBar(tb);
	
	buttonSetup(&loginButton, "Login", 100, 200, 100, 25, &KHUB::handleLogin);
	buttonSetupInt(&registerButton, "Register", 225, 200, 100, 25, &KHUB::handleRegister, 0);
	
	textFieldSetup(&loginEdit, "KHUB Username", 115, 100, 200, 25, false);
	textFieldSetup(&passwordEdit, "Password", 115, 150, 200, 25, true);

	loginWindowPtr = loginWindow;
}

void KHUB::createRegisterScreen()
{	
	setFixedSize(400, 400);

	loginButton->close();
	registerButton->close();

	buttonSetup(&cancelButton, "Cancel", 225, 300, 100, 25, &KHUB::handleCancel);
	buttonSetupInt(&registerButton, "Register", 100, 300, 100, 25, &KHUB::handleRegister, 1);

	textFieldSetup(&loginEdit, "KHUB Username", 115, 100, 200, 25, false);
	textFieldSetup(&loginConfirmEdit, "Confirm Username", 115, 150, 200, 25, false);
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

/*******************/
/* Actions Handler */
/*******************/

void KHUB::createActions()
{
	//Files Handler
	logoutAct = new QAction(tr("&Logout"), this);
	exitAct = new QAction(tr("&Exit"), this);

	connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
	connect(logoutAct, SIGNAL(triggered()), this, SLOT(logout()));

	//Groups Handler
	createGroupAct = new QAction(tr("&CreateGroup"), this);
	findGroupAct = new QAction(tr("&FindGroup"), this);

	//Search Handler
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

/*************/
/* Listeners */
/*************/

//Restart login screen
void KHUB::handleCancel()
{	
	loginWindowPtr->close();

	loginWindowPtr = new KHUB();
		loginWindowPtr->createLoginScreen(loginWindowPtr);
		loginWindowPtr->show();
}

//Close login screen if credentials are good to go and free resources
void KHUB::handleLogin()
{
	SQL databaseConnection;
	
	if (databaseConnection.checkCredentials(loginEdit->text(), passwordEdit->text()))
	{
		loginWindowPtr->close();

		loginWindowPtr = NULL;
		delete loginWindowPtr;

		createMainScreen();
	}
		
	else
		qDebug() << "Failed to login";
}

//Handle register for screen opening or new user 
void KHUB::handleRegister(int isRegister)
{

	if (isRegister)
	{
		qDebug() << loginEdit->text() + " " + loginConfirmEdit->text() + " " + passwordEdit->text() + " " + passwordConfirmEdit->text();

		if (!QString::compare(loginEdit->text(), loginConfirmEdit->text()))
		{
			if(!QString::compare(passwordEdit->text(), passwordConfirmEdit->text(), Qt::CaseInsensitive))
			{
				SQL databaseConnection;
					
				if (!databaseConnection.checkUser(loginEdit->text()))
				{
					databaseConnection.registerUser(loginEdit->text(), passwordEdit->text());

					//After register go back to login screen - recreate it
					emit handleCancel();
				}
				else
					QMessageBox::warning(0, QObject::tr("Warning"), "Username already exists. Please try again.");
			}
			else
			{
				QMessageBox::warning(0, QObject::tr("Warning"), "Your password information does not match. Please try again.");
			}
		}
		else
			QMessageBox::warning(0, QObject::tr("Warning"), "Your login information does not match. Please try again.");
	}
	else
	{
		loginWindowPtr->hide();

		createRegisterScreen();

		loginWindowPtr->repaint();
		loginWindowPtr->show();
	}
	
}

/**************/
/* Code Reuse */
/**************/

void KHUB::buttonSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)())
{
	*button = new QPushButton(name, this);
	(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	//Event Listener
	connect(*button, &QPushButton::released, this, fptr);
}

void KHUB::buttonSetupInt(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)(int parameter), int value)
{
	*button = new QPushButton(name, this);
	(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	connect(*button, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(*button, value);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleRegister(int)));
	//connect(signalMapper, SIGNAL(mapped(int)), this, fptr);

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
