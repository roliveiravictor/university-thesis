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

KHUB::~KHUB()
{
	
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

void KHUB::createLoginScreen(KHUB& loginWindow)
{
	signalMapper = new QSignalMapper(this);

	loginWindow.setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	loginWindow.setFixedSize(400, 300);

	QToolBar* tb = loginWindow.findChild<QToolBar *>();
		loginWindow.removeToolBar(tb);
	
	btSetup(&loginBt, "Login", 100, 200, 100, 25, &KHUB::handleLogin);
	btSetupInt(&registerBt, "Register", 225, 200, 100, 25, &KHUB::handleRegister, FALSE);
	
	txtFieldSetup(&loginEdt, "KHUB Username", 115, 100, 200, 25, false);
	txtFieldSetup(&passwordEdt, "Password", 115, 150, 200, 25, true);

	loginWindowPtr = &loginWindow;
}

void KHUB::createRegisterScreen()
{	
	setFixedSize(400, 400);

	loginBt->close();
	registerBt->close();

	btSetup(&cancelRegisterBt, "Cancel", 225, 300, 100, 25, &KHUB::handleReboot);
	btSetupInt(&registerBt, "Register", 100, 300, 100, 25, &KHUB::handleRegister, TRUE);

	txtFieldSetup(&loginEdt, "KHUB Username", 115, 100, 200, 25, false);
	txtFieldSetup(&loginConfirmEdt, "Confirm Username", 115, 150, 200, 25, false);
	txtFieldSetup(&passwordEdt, "Password", 115, 200, 200, 25, true);
	txtFieldSetup(&passwordConfirmEdt, "Confirm Password", 115, 250, 200, 25, true);
}

void KHUB::createNewGroupScreen()
{
	groupScreen = new QWidget();
	boxLayout = new QVBoxLayout(groupScreen);

	//Mapper to handle all main screen signals ##### FIX THIS
	signalMapper = new QSignalMapper(this);

	groupScreen->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	groupScreen->setWindowModality(Qt::ApplicationModal);
	groupScreen->setFixedSize(400, 300);
	groupScreen->setWindowTitle("New Group");

	txtFieldBoxSetup(&groupNameEdt, "Group Name", false);
	txtFieldBoxSetup(&groupCategoryEdt, "Group Category", false);
	txtFieldBoxSetup(&groupSubjectEdt, "Group Subject", false);

	btBoxSetup(&createGroupBt, "Create", &KHUB::handleNewGroup);
	btBoxSetupInt(&cancelGroupBt, "Cancel", &KHUB::handleDispose, (int) CancelType::cl_newGroup);

	groupScreen->setLayout(boxLayout);
	groupScreen->show();
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

	connect(createGroupAct, SIGNAL(triggered()), this, SLOT(createGroup()));

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
	emit handleReboot();
}

//Groups Functions
void KHUB::createGroup()
{
	createNewGroupScreen();
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
void KHUB::handleReboot()
{	
	qApp->quit();
	QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void KHUB::handleDispose(int slot)
{
	switch (slot)
	{
	case (int) CancelType::cl_newGroup:
										groupScreen->~QWidget(); //### Destructors also get ride of children pointers? I mean, if the object was destroyed am I having memory leak if Im not deleting the pointer? ####
										
										break;

	default:
			QMessageBox::critical(0, QObject::tr("Error"), "Error on screen dispose");;
	}
}

//Close login screen if credentials are good to go and free resources
void KHUB::handleLogin()
{
	SQL databaseConnection;
	
	//DEBUG LOGIN PASS
	//if (databaseConnection.checkCredentials(loginEdt->text(), passwordEdt->text()))
	if (databaseConnection.checkCredentials("victor1234", "gogo"))
	{
		//Cleean Heap
		loginEdt->~QLineEdit();
		passwordEdt->~QLineEdit();
		loginBt->~QPushButton();

		//Check if any registration was created before clearing heap
		if (registerBt == NULL)
		{
			loginConfirmEdt->~QLineEdit();
			passwordConfirmEdt->~QLineEdit();
			registerBt->~QPushButton();
			cancelRegisterBt->~QPushButton();
		}

		loginWindowPtr->close();

		createMainScreen();

		signalMapper->~QSignalMapper();
	}
		
	else
		qDebug() << "Failed to login";
}

//Handle register for screen opening or new user 
void KHUB::handleRegister(int isRegister)
{
	if (isRegister)
	{
		if (!QString::compare(loginEdt->text(), loginConfirmEdt->text()))
		{
			if(!QString::compare(passwordEdt->text(), passwordConfirmEdt->text(), Qt::CaseInsensitive))
			{
				SQL databaseConnection;
					
				if (!databaseConnection.checkUser(loginEdt->text()))
				{
					databaseConnection.registerUser(loginEdt->text(), passwordEdt->text());

					//After register go back to login screen - recreate it
					emit handleReboot();
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

//Create new group to share knowledge
void KHUB::handleNewGroup()
{
	SQL databaseConnection;

	databaseConnection.createGroup(groupNameEdt->text(), groupCategoryEdt->text(), groupSubjectEdt->text());

}

/**************/
/* Code Reuse */
/**************/

void KHUB::btSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)())
{
	*button = new QPushButton(name, this);	
		(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	//Connecting Listener
	connect(*button, &QPushButton::released, this, fptr);
}

void KHUB::btSetupInt(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)(int parameter), int value)
{
	*button = new QPushButton(name, this);
		(*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

	connect(*button, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(*button, value);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleRegister(int)), Qt::UniqueConnection);
}

void KHUB::btBoxSetup(QPushButton **button, const QString name, void (KHUB::*fptr)())
{
	*button = new QPushButton(name, this);

	connect(*button, &QPushButton::released, this, fptr);

	boxLayout->addWidget(*button);
}

void KHUB::btBoxSetupInt(QPushButton **button, const QString name, void (KHUB::*fptr)(int parameter), int slot)
{
	*button = new QPushButton(name, this);

	boxLayout->addWidget(*button);

	connect(*button, SIGNAL(clicked()), signalMapper, SLOT(map()));
	signalMapper->setMapping(*button, slot);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(handleDispose(int)), Qt::UniqueConnection);
}

void KHUB::txtFieldSetup(QLineEdit **textField, const QString hint, int posX, int posY, int width, int height, bool isPassword)
{
	*textField = new QLineEdit("", this);
		(*textField)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));
		(*textField)->setAlignment(Qt::AlignHCenter);
		(*textField)->setPlaceholderText(hint);

	if (isPassword)
		(*textField)->setEchoMode(passwordEdt->Password);
}

void KHUB::txtFieldBoxSetup(QLineEdit **textField, const QString hint, bool isPassword)
{
	*textField = new QLineEdit("", this);
		(*textField)->setAlignment(Qt::AlignHCenter);
		(*textField)->setPlaceholderText(hint);

	boxLayout->addWidget(*textField);

	if (isPassword)
		(*textField)->setEchoMode(passwordEdt->Password);
}
