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
#include "vld.h" // Memory Leak Debug

// Global Pointer to handle main interface changes
KHUB *mainWindowPtr;

KHUB::KHUB(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
}

KHUB::~KHUB(){

}

/*****************/
/* Screens Setup */
/*****************/

void KHUB::create_MainScreen(int user_id) {
  // Sets main full screen and shows
  mainWindowPtr = new KHUB();

  // Application name written on my Thesis
  mainWindowPtr->setWindowTitle("FCE");
  mainWindowPtr->setWindowState(mainWindowPtr->windowState() ^ Qt::WindowMaximized);

  // Get user id reference from previous login window
  mainWindowPtr->user_id = user_id;

  // Remove default empty toolbar
  QToolBar* tb = mainWindowPtr->findChild<QToolBar *>(); 
  
  mainWindowPtr->removeToolBar(tb);
  mainWindowPtr->set_Actions();
  mainWindowPtr->set_Menu();

  mainWindowPtr->show();

  delete tb;

  // TEST LINE - REMOVE AFTER USE
  mainWindowPtr->joinGroup();
  //mainWindowPtr->dialog_Search();
}

void KHUB::create_LoginScreen(KHUB& loginWindow) {
  generalMap = new QSignalMapper(this);

  loginWindow.setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
  loginWindow.setFixedSize(400, 300);

  QToolBar* tb = loginWindow.findChild<QToolBar *>();
  loginWindow.removeToolBar(tb);
	

  btSetup(&loginBt, "Login", 100, 200, 100, 25, &KHUB::handleLogin);
  btSetupInt(&registerBt, "Register", 225, 200, 100, 25, &KHUB::handleRegister, generalMap, FALSE, (int) ButtonHandler::hl_Register);
	
  txtFieldSetup(&loginEdt, "KHUB Username", 115, 100, 200, 25, false);
  txtFieldSetup(&passwordEdt, "Password", 115, 150, 200, 25, true);

  loginWindowPtr = &loginWindow;
}

void KHUB::create_RegisterScreen() {	
  setFixedSize(400, 400);

  loginBt->close();
  registerBt->close();

  btSetup(&cancelRegisterBt, "Cancel", 225, 300, 100, 25, &KHUB::handleReboot);
  btSetupInt(&registerBt, "Register", 100, 300, 100, 25, &KHUB::handleRegister, generalMap, TRUE, (int)ButtonHandler::hl_Register);

  txtFieldSetup(&loginEdt, "KHUB Username", 115, 100, 200, 25, false);
  txtFieldSetup(&loginConfirmEdt, "Confirm Username", 115, 150, 200, 25, false);
  txtFieldSetup(&passwordEdt, "Password", 115, 200, 200, 25, true);
  txtFieldSetup(&passwordConfirmEdt, "Confirm Password", 115, 250, 200, 25, true);
}

void KHUB::create_GroupScreen(bool isCreate) {
  generalMap = new QSignalMapper(this);
  tabs = new QTabWidget();

  gridLayout = new QGridLayout();
  
  localTab = new QWidget();
  sharedTab = new QWidget();

  localTab->setLayout(gridLayout);
 
  QScrollArea *scrollLocal = new QScrollArea();
  scrollLocal->setWidgetResizable(true);
  scrollLocal->setWidget(localTab);

  tabs->addTab(scrollLocal, tr("Local"));
  tabs->addTab(sharedTab, tr("Shared"));
  
  QWidget *central = new QWidget();
  QVBoxLayout *mainLayout = new QVBoxLayout();  

  mainLayout->addWidget(tabs);
  central->setLayout(mainLayout);
    
  mainWindowPtr->setCentralWidget(central);
  
  // Check to see if group already exists and retrieve its information or start from a new one
  // We will start with the first option
  if (isCreate) {
      
  } else {
      
  }
}

/*****************/
/* Dialogs Setup */
/*****************/

void KHUB::dialog_NewGroup() {
  newGroupDialog = new QWidget();
  boxLayout = new QVBoxLayout(newGroupDialog);

  // Mapper to handle all main screen signals ##### NOT THE BEST SOLUTION - FIX THIS
  generalMap = new QSignalMapper(this);

  newGroupDialog->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
  newGroupDialog->setWindowModality(Qt::ApplicationModal);
  newGroupDialog->setFixedSize(400, 300);
  newGroupDialog->setWindowTitle("New Group");

  txtFieldBoxSetup(&groupNameEdt, "Group Name", false);
  txtFieldBoxSetup(&groupCategoryEdt, "Group Category", false);
  txtFieldBoxSetup(&groupSubjectEdt, "Group Subject", false);

  btBoxSetup(&createGroupBt, "Create", &KHUB::handleNewGroup);
  btBoxSetupInt(&cancelBt, "Cancel", &KHUB::handleDispose, (int) CancelType::cl_newGroup);

  newGroupDialog->setLayout(boxLayout);
  newGroupDialog->show();
}

void KHUB::dialog_JoinGroup() {
  joinGroupDialog = new QWidget();
  boxLayout = new QVBoxLayout(joinGroupDialog);

  generalMap = new QSignalMapper(this);

  joinGroupDialog->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
  joinGroupDialog->setWindowModality(Qt::ApplicationModal);
  joinGroupDialog->setFixedSize(400, 300);
  joinGroupDialog->setWindowTitle("Join Group");

  txtFieldBoxSetup(&groupIdEdt, "Type Group Id to Join", false);

  btBoxSetup(&joinGroupBt, "Join", &KHUB::handleJoinGroup);
  btBoxSetupInt(&cancelBt, "Cancel", &KHUB::handleDispose, (int) CancelType::cl_joinGroup);

  joinGroupDialog->setLayout(boxLayout);
  joinGroupDialog->show();
}

void KHUB::dialog_Search() {
  if (isGrouped){
    searchDialog = new QWidget();
    boxLayout = new QVBoxLayout(searchDialog);

    generalMap = new QSignalMapper(this);
 
    searchDialog->setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
    searchDialog->setWindowModality(Qt::ApplicationModal);
    searchDialog->setFixedSize(400, 300);
    searchDialog->setWindowTitle("Search");

    txtFieldBoxSetup(&searchEdt, "Type Keywords to Search", false);

    btBoxSetup(&searchBt, "Search", &KHUB::handleSearch);
    btBoxSetupInt(&cancelBt, "Cancel", &KHUB::handleDispose, (int)CancelType::cl_search);

    searchDialog->setLayout(boxLayout);
    searchDialog->show();
  } else {
    QMessageBox::critical(0, QObject::tr("Error"), "Before searching for something, first join a group.");
  }
}

/**************/
/* Menu Setup */
/**************/

void KHUB::set_Menu() {
  QMenu* fileMenu, *groupsMenu, *searchMenu;

  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(logoutAct);
  fileMenu->addAction(exitAct);
	
  groupsMenu = menuBar()->addMenu(tr("&Groups"));
  groupsMenu->addAction(createGroupAct);
  groupsMenu->addAction(joinGroupAct);

  searchMenu = menuBar()->addMenu(tr("&Search"));
  searchMenu->addAction(searchAct);
}

/************/
/*  Actions */
/************/

void KHUB::set_Actions() {
  // Files Actions
  logoutAct = new QAction(tr("&Logout"), this);
  exitAct = new QAction(tr("&Exit"), this);

  connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));
  connect(logoutAct, SIGNAL(triggered()), this, SLOT(logout()));

  // Groups Actions
  createGroupAct = new QAction(tr("&Create Group"), this);
  joinGroupAct = new QAction(tr("&Join Group"), this);

  connect(createGroupAct, SIGNAL(triggered()), this, SLOT(createGroup()));
  connect(joinGroupAct, SIGNAL(triggered()), this, SLOT(joinGroup()));

  // Search Actions
  searchAct = new QAction(tr("&Search"), this);

  connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));
}

// File Functions
void KHUB::exit() {
  delete logoutAct;
  delete exitAct;
  delete createGroupAct;
  delete joinGroupAct;
  delete searchAct;

  QApplication::quit();
}

void KHUB::logout() {
  // Restart
  emit handleReboot();
}

// Groups Functions
void KHUB::createGroup() {
  dialog_NewGroup();
}

void KHUB::joinGroup() {
  dialog_JoinGroup();
}

// Search Functions
void KHUB::search() {
  dialog_Search();
}

/*************/
/* Handlers  */
/*************/

// Restart login screen
void KHUB::handleReboot() {	
  qApp->quit();
  QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void KHUB::handleDispose(int slot) {
  switch (slot) {
	case (int) CancelType::cl_newGroup:
	  delete newGroupDialog;
	  break;

	case (int) CancelType::cl_joinGroup:
	  delete joinGroupDialog;
	  break;

	case (int) CancelType::cl_search:
	  delete searchDialog;
	  break;
          
    case (int) ButtonHandler::hl_DisposeBrowser:
      // Delete tab for closing and set it to NULL for new opening
      delete browserTab;
      delete closeBrowser;
      browserTab = NULL;
      break;

	default:
	  qDebug() << "None slot to delete";
	}
}

// Close login screen if credentials are good to go and free resources
void KHUB::handleLogin() {
  SQL databaseConnection;
	
  // DEBUG LOGIN PASS
  //if (databaseConnection.checkCredentials(loginEdt->text(), passwordEdt->text()))
  user_id = databaseConnection.checkCredentials("victor1234", "gogo");
	
  if (user_id != NULL) {
	// Cleean Heap
	delete loginEdt;
	delete passwordEdt;
	delete loginBt;

    // Check if any registration was created before clearing heap
    if (registerBt == NULL) {
	  delete loginConfirmEdt;
	  delete passwordConfirmEdt;
	  delete registerBt;
	  delete cancelRegisterBt;
    }
    create_MainScreen(user_id);	
    loginWindowPtr->close();
  delete generalMap;
  } else {
    QMessageBox::warning(0, QObject::tr("Warning"), "Username or password incorrect. Please try again.");
  }
}

// Handle register for screen opening or new user 
void KHUB::handleRegister(int isRegister) {
  if (isRegister) {
	if (!QString::compare(loginEdt->text(), loginConfirmEdt->text())) {
	  if (!QString::compare(passwordEdt->text(), passwordConfirmEdt->text(), Qt::CaseInsensitive))	{
	    SQL databaseConnection;
		if (!databaseConnection.checkUser(loginEdt->text())) {
		  if (databaseConnection.registerUser(loginEdt->text(), passwordEdt->text())) {
	        QMessageBox::information(0, QObject::tr("Success"), "Registration Successful.");
            // After register go back to login screen - recreate it
			emit handleReboot();
          }
        } else {
		  QMessageBox::warning(0, QObject::tr("Warning"), "Username already exists. Please try again.");
        }
	  } else {
	    QMessageBox::warning(0, QObject::tr("Warning"), "Your password information does not match. Please try again.");
     }
   } else {
    QMessageBox::warning(0, QObject::tr("Warning"), "Your login information does not match. Please try again.");
        }
  } else {
    loginWindowPtr->hide();
    create_RegisterScreen();
	loginWindowPtr->repaint();
	loginWindowPtr->show();
	}	
}

// Create new group to share knowledge
void KHUB::handleNewGroup() {
  SQL databaseConnection;

  bool status = databaseConnection.createGroup(user_id, groupNameEdt->text(), groupCategoryEdt->text(), groupSubjectEdt->text());

  if (status) {
	delete newGroupDialog;
    isGrouped = true;
	QMessageBox::information(0, QObject::tr("Success:"), "New group created.");
	create_GroupScreen(true);
  } else {
    QMessageBox::critical(0, QObject::tr("Error"), "Could not create a new group.");
  }
}

// Find new group to join
void KHUB::handleJoinGroup() {
  SQL databaseConnection;

  bool status = databaseConnection.joinGroup(user_id, 1); // debug groupId 1 groupId->text().toInt()

  if (status) {
    QMessageBox::information(0, QObject::tr("Joined:"), "Welcome!");
    isGrouped = true;
	delete joinGroupDialog;
    create_GroupScreen(false);

    // TEST LINE - REMOVE AFTER USE
    mainWindowPtr->dialog_Search();

  } else {
	QMessageBox::critical(0, QObject::tr("Error"), "Could not create a find group to join.");
  }
}

// Find references to share
void KHUB::handleSearch() {
  if (gridLayout->layout() != nullptr) {
    QLayoutItem* item;
    while ((item = gridLayout->layout()->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
  }

  HTTP query;

  // Send request to throw 302 Http error - Moved Page
  query.sendRequest(searchEdt->text(), true);

  // Send another to parse references
  query.sendRequest(NULL, false);

  HTTP reader;
  localUrl = reader.readReferences("Main Query Cleaned.txt");

  int componentsPos = 1;

  upVoteMap = new QSignalMapper(this);
  downVoteMap = new QSignalMapper(this);
  openMap = new QSignalMapper(this);

  // A vector from Buttons and Labels needs to be implemented here to avoid memory leak - It will be needed to clean this vector every time the user demands a search ### FIX THIS
  for (int pos = 0; pos < localUrl.size(); pos++){
    QLabel *link = new QLabel();
    link->setText("<a href=\"" + localUrl.at(pos) + "\">" + localUrl.at(pos) + "</a>");
    link->setTextFormat(Qt::RichText);
    link->setTextInteractionFlags(Qt::TextBrowserInteraction);
    link->setOpenExternalLinks(true);
  
    QPushButton *upArrow;
    btSetupInt(&upArrow, "", 225, 300, 100, 25, &KHUB::handleUpVote, upVoteMap, pos, (int)ButtonHandler::hl_UpVote);
    QIcon ButtonUp("Resources/Arrows/arrow.png");
    upArrow->setIcon(ButtonUp);

    QPushButton *open;
    btSetupInt(&open, "Open", 225, 300, 100, 25, &KHUB::handleUrl, openMap, pos, (int) ButtonHandler::hl_OpenUrl);

    QPushButton *downArrow;
    btSetupInt(&downArrow, "", 225, 300, 100, 25, &KHUB::handleDownVote, downVoteMap, pos, (int)ButtonHandler::hl_DownVote);
    QIcon ButtonDown("Resources/Arrows/downarrow.png");
    downArrow->setIcon(ButtonDown);

    QLabel *separator = new QLabel();
    QLabel *guider = new QLabel("<----------------------------------------------------------------------------------------------------------------------------------------------------->");
 
    gridLayout->addWidget(link, componentsPos, 0, 1, -1);
    gridLayout->addWidget(guider, componentsPos, 1, 1, -1);
    gridLayout->addWidget(upArrow, componentsPos - 1, 2, 1, 1, Qt::AlignBottom);
    gridLayout->addWidget(open, componentsPos, 2, 1, 1);
    gridLayout->addWidget(downArrow, componentsPos + 1, 2, 1, 1, Qt::AlignTop);
    gridLayout->addWidget(separator, componentsPos + 2, 1, 1, 1);

    componentsPos = componentsPos + 5;
  }
  localTab->setFocus();
  delete searchDialog;
}

void KHUB::handleUrl(int referenceID) {
  if (browserTab == NULL){
    newBrowser();
  }
  else {
    delete browserTab;
    newBrowser();
  }
  // SET HTML reader
  QVBoxLayout *webLayout = new QVBoxLayout();
  QWebView *reader = new QWebView();

  reader->load(QUrl(localUrl.at(referenceID)));
  webLayout->addWidget(reader);
  browserTab->setLayout(webLayout);
}

void KHUB::handleUpVote(int referenceID) {
  SQL databaseConnection;

  bool status = databaseConnection.rate(user_id, 1, localUrl.at(referenceID), true); // debug groupId 1 - Needs to implement a functionality to return group id
  if (!status) {
    QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
  }
}

void KHUB::handleDownVote(int referenceID) {
    SQL databaseConnection;

    bool status = databaseConnection.rate(user_id, 1, localUrl.at(referenceID), false); // debug groupId 1
    if (!status) {
        QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
    }
}

/**************/
/* Code Reuse */
/**************/

void KHUB::btSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)()) {
  *button = new QPushButton(name, this);	
  (*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));
  
  connect(*button, &QPushButton::released, this, fptr);
}

void KHUB::btSetupInt(QPushButton **button, const QString name, int posX, int posY, int width, int height, void (KHUB::*fptr)(int parameter), QSignalMapper* map, int value, int handler) {
  *button = new QPushButton(name, this);
  (*button)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));

  map->setMapping(*button, value);
  connect(*button, SIGNAL(clicked()), map, SLOT(map()));

  switch (handler) {
    case (int) ButtonHandler::hl_Register:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleRegister(int)), Qt::UniqueConnection);
      break;
    case (int)ButtonHandler::hl_UpVote:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleUpVote(int)), Qt::UniqueConnection);
      break;
    case (int) ButtonHandler::hl_OpenUrl:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleUrl(int)), Qt::UniqueConnection);
      break;
    case (int)ButtonHandler::hl_DownVote:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleDownVote(int)), Qt::UniqueConnection);
      break;
    case (int) ButtonHandler::hl_DisposeBrowser:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleDispose(int)), Qt::UniqueConnection);
      break;
    }
}

void KHUB::btBoxSetup(QPushButton **button, const QString name, void (KHUB::*fptr)()) {
  *button = new QPushButton(name, this);

  connect(*button, &QPushButton::released, this, fptr);
  boxLayout->addWidget(*button);
}

void KHUB::btBoxSetupInt(QPushButton **button, const QString name, void (KHUB::*fptr)(int parameter), int slot) {
  *button = new QPushButton(name, this);

  boxLayout->addWidget(*button);
  connect(*button, SIGNAL(clicked()), generalMap, SLOT(map()));
  generalMap->setMapping(*button, slot);

  connect(generalMap, SIGNAL(mapped(int)), this, SLOT(handleDispose(int)), Qt::UniqueConnection);
}

void KHUB::txtFieldSetup(QLineEdit **textField, const QString hint, int posX, int posY, int width, int height, bool isPassword) {
  *textField = new QLineEdit("", this);
  (*textField)->setGeometry(QRect(QPoint(posX, posY), QSize(width, height)));
  (*textField)->setAlignment(Qt::AlignHCenter);
  (*textField)->setPlaceholderText(hint);

  if (isPassword)
    (*textField)->setEchoMode(passwordEdt->Password);
}

void KHUB::txtFieldBoxSetup(QLineEdit **textField, const QString hint, bool isPassword) {
  *textField = new QLineEdit("", this);
  (*textField)->setAlignment(Qt::AlignHCenter);
  (*textField)->setPlaceholderText(hint);

  boxLayout->addWidget(*textField);

  if (isPassword)
    (*textField)->setEchoMode(passwordEdt->Password);
}

void KHUB::newBrowser(){
    browserTab = new QWidget();
    tabs->addTab(browserTab, tr("Navegador"));
    tabs->setCurrentWidget(browserTab);

    closeBrowser = new QPushButton();
    // Might seems non sense to overload a fucntion with the same parameter but there are cases where they need to be different - check for other "btSetupInt" lines
    btSetupInt(&closeBrowser, "x", 0, 0, 25, 25, &KHUB::handleDispose, generalMap,(int) ButtonHandler::hl_DisposeBrowser, (int)ButtonHandler::hl_DisposeBrowser);
    tabs->tabBar()->setTabButton(2, QTabBar::RightSide, closeBrowser); // First argument "2" means tab position
}
    


