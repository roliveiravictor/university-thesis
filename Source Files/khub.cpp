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

/* MEMORY LEAK DEBUG */
#include "vld.h"


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

void KHUB::create_MainScreen(int user_id)
{
	//Sets main full screen and shows
	mainWindow = new KHUB();
		mainWindow->setWindowState(mainWindow->windowState() ^ Qt::WindowMaximized);

	//Get user id reference from previous login window
	mainWindow->user_id = user_id;

	//Remove default empty toolbar
	QToolBar* tb = mainWindow->findChild<QToolBar *>();
		mainWindow->removeToolBar(tb);

	mainWindow->set_Actions();
	mainWindow->set_Menu();
	mainWindow->show();

	delete tb;
}

void KHUB::create_LoginScreen(KHUB& loginWindow)
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

void KHUB::create_RegisterScreen()
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

void KHUB::mainWindow_GroupScreen()
{
	
}

void KHUB::dialog_NewGroup()
{
	newGroupDialog = new QWidget();
	boxLayout = new QVBoxLayout(newGroupDialog);

	//Mapper to handle all main screen signals ##### FIX THIS
	signalMapper = new QSignalMapper(this);

	newGroupDialog->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	newGroupDialog->setWindowModality(Qt::ApplicationModal);
	newGroupDialog->setFixedSize(400, 300);
	newGroupDialog->setWindowTitle("New Group");

	txtFieldBoxSetup(&groupNameEdt, "Group Name", false);
	txtFieldBoxSetup(&groupCategoryEdt, "Group Category", false);
	txtFieldBoxSetup(&groupSubjectEdt, "Group Subject", false);

	btBoxSetup(&createGroupBt, "Create", &KHUB::handleNewGroup);
	btBoxSetupInt(&cancelGroupBt, "Cancel", &KHUB::handleDispose, (int) CancelType::cl_newGroup);

	newGroupDialog->setLayout(boxLayout);
	newGroupDialog->show();
}

void KHUB::dialog_JoinGroup()
{
	joinGroupDialog = new QWidget();
	boxLayout = new QVBoxLayout(joinGroupDialog);

	//Mapper to handle all main screen signals ##### FIX THIS
	signalMapper = new QSignalMapper(this);

	joinGroupDialog->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
	joinGroupDialog->setWindowModality(Qt::ApplicationModal);
	joinGroupDialog->setFixedSize(400, 300);
	joinGroupDialog->setWindowTitle("Join Group");

	//btBoxSetup(&joinGroupBt, "Join", &KHUB::handleJoinGroup);
	btBoxSetupInt(&cancelGroupBt, "Cancel", &KHUB::handleDispose, (int) CancelType::cl_joinGroup);

	joinGroupDialog->setLayout(boxLayout);
	joinGroupDialog->show();
}

void KHUB::set_Menu()
{
	QMenu* fileMenu, *groupsMenu, *searchMenu;

	fileMenu = menuBar()->addMenu(tr("&File"));
		fileMenu->addAction(logoutAct);
		fileMenu->addAction(exitAct);
	
	groupsMenu = menuBar()->addMenu(tr("&Groups"));
		groupsMenu->addAction(createGroupAct);
		groupsMenu->addAction(joinGroupAct);

	searchMenu = menuBar()->addMenu(tr("&Search"));
		searchMenu->addAction(newSearchAct);
}

/*******************/
/* Actions Handler */
/*******************/

void KHUB::set_Actions()
{
	//Files Handler
	logoutAct = new QAction(tr("&Logout"), this);
	exitAct = new QAction(tr("&Exit"), this);

	connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
	connect(logoutAct, SIGNAL(triggered()), this, SLOT(logout()));

	//Groups Handler
	createGroupAct = new QAction(tr("&Create Group"), this);
	joinGroupAct = new QAction(tr("&Join Group"), this);

	connect(createGroupAct, SIGNAL(triggered()), this, SLOT(createGroup()));
	connect(joinGroupAct, SIGNAL(triggered()), this, SLOT(joinGroup()));

	//Search Handler
	newSearchAct = new QAction(tr("&New Search"), this);
}

//File Functions
void KHUB::exit()
{
	delete logoutAct;
	delete exitAct;

	delete createGroupAct;
	delete joinGroupAct;

	delete newSearchAct;

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
	dialog_NewGroup();
}

void KHUB::joinGroup()
{
	dialog_JoinGroup();
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
											delete newGroupDialog;
											break;

		case (int) CancelType::cl_joinGroup:
											delete joinGroupDialog;
											break;


			default:
					QMessageBox::critical(0, QObject::tr("Error"), "Error on screen dispose");
	}
}

//Close login screen if credentials are good to go and free resources
void KHUB::handleLogin()
{
	SQL databaseConnection;
	
	//DEBUG LOGIN PASS
	//if (databaseConnection.checkCredentials(loginEdt->text(), passwordEdt->text()))
	user_id = databaseConnection.checkCredentials("victor1234", "gogo");
	
	if (user_id != NULL)
	{
		//Cleean Heap
		delete loginEdt;
		delete passwordEdt;
		delete loginBt;

		//Check if any registration was created before clearing heap
		if (registerBt == NULL)
		{
			delete loginConfirmEdt;
			delete passwordConfirmEdt;
			delete registerBt;
			delete cancelRegisterBt;
		}

		loginWindowPtr->close();

		create_MainScreen(user_id);
		
		delete signalMapper;
	}
		
	else
		QMessageBox::warning(0, QObject::tr("Warning"), "Username or password incorrect. Please try again.");
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
					if (databaseConnection.registerUser(loginEdt->text(), passwordEdt->text()))
					{
						QMessageBox::information(0, QObject::tr("Success"), "Registration Successful.");

						//After register go back to login screen - recreate it
						emit handleReboot();
					}
					
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

		create_RegisterScreen();

		loginWindowPtr->repaint();
		loginWindowPtr->show();
	}
	
}

//Create new group to share knowledge
void KHUB::handleNewGroup()
{
	SQL databaseConnection;

	bool status = databaseConnection.createGroup(user_id, groupNameEdt->text(), groupCategoryEdt->text(), groupSubjectEdt->text());

	if (status)
	{
		delete newGroupDialog;
		QMessageBox::information(0, QObject::tr("Success"), "New group created.");
		mainWindow_GroupScreen();
	}
		
	else
		QMessageBox::critical(0, QObject::tr("Error"), "Could not create a new group.");

}

//Find new group to join
void KHUB::handleJoinGroup()
{

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


