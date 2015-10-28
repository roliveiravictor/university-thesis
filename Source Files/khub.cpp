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
}

void KHUB::create_LoginScreen(KHUB& loginWindow) {
  generalMap = new QSignalMapper(this);

  loginWindow.setWindowFlags((windowFlags() | Qt::CustomizeWindowHint) &~Qt::WindowMaximizeButtonHint);
  loginWindow.setFixedSize(400, 300);
  loginWindow.setWindowTitle("FCE");

  QToolBar* tb = loginWindow.findChild<QToolBar *>();
  loginWindow.removeToolBar(tb);
	

  btSetup(&loginBt, "Login", 100, 200, 100, 25, &KHUB::handleLogin);
  btSetupInt(&registerBt, "Register", 225, 200, 100, 25, &KHUB::handleRegister, generalMap, FALSE, (int) ButtonHandler::hl_Register);
	
  txtFieldSetup(&loginEdt, "FCE Username", 115, 100, 200, 25, false);
  txtFieldSetup(&passwordEdt, "Password", 115, 150, 200, 25, true);

  loginWindowPtr = &loginWindow;
}

void KHUB::create_RegisterScreen() {	
  setFixedSize(400, 400);

  loginBt->close();
  registerBt->close();

  btSetup(&cancelRegisterBt, "Cancel", 225, 300, 100, 25, &KHUB::handleReboot);
  btSetupInt(&registerBt, "Register", 100, 300, 100, 25, &KHUB::handleRegister, generalMap, TRUE, (int)ButtonHandler::hl_Register);

  txtFieldSetup(&loginEdt, "FCE Username", 115, 100, 200, 25, false);
  txtFieldSetup(&loginConfirmEdt, "Confirm Username", 115, 150, 200, 25, false);
  txtFieldSetup(&passwordEdt, "Password", 115, 200, 200, 25, true);
  txtFieldSetup(&passwordConfirmEdt, "Confirm Password", 115, 250, 200, 25, true);
}

void KHUB::create_GroupScreen(bool isCreate) {
  generalMap = new QSignalMapper(this);
  tabs = new QTabWidget();

  localLayout = new QGridLayout();
  sharedLayout = new QGridLayout();
  
  localTab = new QWidget();
  sharedTab = new QWidget();

  localTab->setLayout(localLayout);
  sharedTab->setLayout(sharedLayout);
 
  QScrollArea *scrollLocal = new QScrollArea();
  scrollLocal->setWidgetResizable(true);
  scrollLocal->setWidget(localTab);
  
  QScrollArea *scrollShared = new QScrollArea();
  scrollShared->setWidgetResizable(true);
  scrollShared->setWidget(sharedTab);

  tabs->addTab(scrollLocal, tr("Local"));
  tabs->addTab(scrollShared, tr("Shared"));
  
  QWidget *central = new QWidget();
  QVBoxLayout *mainLayout = new QVBoxLayout();  

  mainLayout->addWidget(tabs);
  central->setLayout(mainLayout);
    
  mainWindowPtr->setCentralWidget(central);
  
  // Check to see if group already exists and retrieve its information or start from a new one
  // We will start with the first option
  if (!isCreate)
    loadReferences();

  QApplication::setOverrideCursor(Qt::ArrowCursor);
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
  groupsMenu->addAction(aboutThisAct);

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
  aboutThisAct = new QAction(tr("&About This"), this);

  aboutThisAct->setEnabled(false);

  connect(createGroupAct, SIGNAL(triggered()), this, SLOT(createGroup()));
  connect(joinGroupAct, SIGNAL(triggered()), this, SLOT(joinGroup()));
  connect(aboutThisAct, SIGNAL(triggered()), this, SLOT(aboutThisGroup()));

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

void KHUB::aboutThisGroup() {
  switch (groupData.size()) {
    case 1:
      QMessageBox::information(0, QObject::tr("About this group: "), "Group ID: " + QString::number(group_id) + "\nGroup Name: " + groupData.at(0) + "\nGroup Category: Not Specified \nGroup Subject: Not Specified");
      break;
    case 2:
      QMessageBox::information(0, QObject::tr("About this group: "), "Group ID: " + QString::number(group_id) + "\nGroup Name: " + groupData.at(0) + "\nGroup Category: " + groupData.at(1) + "\nGroup Subject: Not Specified");
      break;
    default:
      QMessageBox::information(0, QObject::tr("About this group: "), "Group ID: " + QString::number(group_id) + "\nGroup Name: " + groupData.at(0) + "\nGroup Category: " + groupData.at(1) + "\nGroup Subject: " + groupData.at(2));
      break;
  }
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SQL databaseConnection;
	
  user_id = databaseConnection.checkCredentials(loginEdt->text(), passwordEdt->text());

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
  QApplication::setOverrideCursor(Qt::ArrowCursor);
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
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SQL databaseConnection;

  bool status = databaseConnection.createGroup(user_id, groupNameEdt->text(), groupCategoryEdt->text(), groupSubjectEdt->text());

  if (status) {
	delete newGroupDialog;

    //Gather group information
    groupData = databaseConnection.checkGroup(group_id);
    group_id = groupData.at(3).toInt();

    isGrouped = true;
    aboutThisAct->setEnabled(true);
	create_GroupScreen(true);
  } else {
      QMessageBox::critical(0, QObject::tr("Error"), "Could not create a new group.");
  }
}

// Find new group to join
void KHUB::handleJoinGroup() {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SQL databaseConnection;

  group_id = groupIdEdt->text().toInt();
  bool status = databaseConnection.joinGroup(user_id, group_id);
  groupData = databaseConnection.checkGroup(group_id);

  if (status) {
    delete joinGroupDialog;

    isGrouped = true;
	aboutThisAct->setEnabled(true);
    create_GroupScreen(false);
  } else {
	QMessageBox::critical(0, QObject::tr("Error"), "Could not create a find group to join.");
  }
}

// Find references to share
void KHUB::handleSearch() {
  if (localLayout->layout() != nullptr) {
    QLayoutItem* item;
    while ((item = localLayout->layout()->takeAt(0)) != nullptr) {
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

  upLocalMap = new QSignalMapper(this);
  downLocalMap = new QSignalMapper(this);
  localMap = new QSignalMapper(this);

  // A vector from Buttons and Labels needs to be implemented here to avoid memory leak - It will be needed to clean this vector every time the user demands a search ou join/create a group### FIX THIS
  for (int pos = 0; pos < localUrl.size(); pos++){
    QLabel *link = new QLabel();
    link->setText("<a href=\"" + localUrl.at(pos) + "\">" + localUrl.at(pos) + "</a>");
    link->setTextFormat(Qt::RichText);
    link->setTextInteractionFlags(Qt::TextBrowserInteraction);
    link->setOpenExternalLinks(true);
  
    QPushButton *upArrow;
    btSetupInt(&upArrow, "", 225, 300, 100, 25, &KHUB::handleLocalUpVote, upLocalMap, pos, (int)ButtonHandler::hl_UpLocalVote);
    QIcon ButtonUp("Resources/Arrows/arrow.png");
    upArrow->setIcon(ButtonUp);

    QPushButton *open;
    btSetupInt(&open, "Open", 225, 300, 100, 25, &KHUB::handleLocalUrl, localMap, pos, (int) ButtonHandler::hl_LocalUrl);

    QPushButton *downArrow;
    btSetupInt(&downArrow, "", 225, 300, 100, 25, &KHUB::handleLocalDownVote, downLocalMap, pos, (int)ButtonHandler::hl_DownLocalVote);
    QIcon ButtonDown("Resources/Arrows/downarrow.png");
    downArrow->setIcon(ButtonDown);

    QLabel *separator = new QLabel();
    QLabel *guider = new QLabel("<----------------------------------------------------------------------------------------------------------------------------------------------------->");
 
    localLayout->addWidget(link, componentsPos, 0, 1, -1);
    localLayout->addWidget(guider, componentsPos, 1, 1, -1);
    localLayout->addWidget(upArrow, componentsPos - 1, 2, 1, 1, Qt::AlignBottom);
    localLayout->addWidget(open, componentsPos, 2, 1, 1);
    localLayout->addWidget(downArrow, componentsPos + 1, 2, 1, 1, Qt::AlignTop);
    localLayout->addWidget(separator, componentsPos + 2, 1, 1, 1);

    componentsPos = componentsPos + 5;
  }
  delete searchDialog;
  localTab->setFocus();
}

void KHUB::handleLocalUrl(int referenceID) {
  if (browserTab == NULL) {
    newBrowser();
  } else {
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

void KHUB::handleSharedUrl(int referenceID) {
    if (browserTab == NULL) {
        newBrowser();
    } else {
        delete browserTab;
        newBrowser();
    }
    
    // SET HTML reader
    QVBoxLayout *webLayout = new QVBoxLayout();
    QWebView *reader = new QWebView();

    QString url;
    int i = 0;
    for (auto u : sharedContent) {
      if (i == referenceID) {
        url = u.first;
        break;
      }
      i++;
    }

    reader->load(QUrl(url));
    webLayout->addWidget(reader);
    browserTab->setLayout(webLayout);
}

void KHUB::handleLocalUpVote(int referenceID) {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  
  SQL databaseConnection;

  bool status = databaseConnection.rate(group_id, localUrl.at(referenceID), true); 
  
  /*Crashed logic - Trying to implement a color change (green or red) on voted buttons 

  QPushButton *bt;
  bt = qobject_cast <QPushButton*> (sender());
  QPalette p(bt->palette());
  p.setColor(QPalette::Button, QColor("#85FF5C"));
  bt->setPalette(p);

  update();*/

  if (!status) {
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
  } else {
      loadReferences();
  }

  QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void KHUB::handleSharedUpVote(int referenceID) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    
    SQL databaseConnection;

    QString url;
    int i = 0;
    for (auto u : sharedContent) {
        if (i == referenceID) {
            url = u.first;
            break;
        }
        i++;
    }
    bool status = databaseConnection.rate(group_id, url, true);
    if (!status) {
      QApplication::setOverrideCursor(Qt::ArrowCursor);
      QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
    } else {
        loadReferences();
    }

    QApplication::setOverrideCursor(Qt::ArrowCursor);
}


void KHUB::handleLocalDownVote(int referenceID) {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SQL databaseConnection;

  bool status = databaseConnection.rate(group_id, localUrl.at(referenceID), false);
  if (!status) {
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
  } else {
      loadReferences();
  }
  QApplication::setOverrideCursor(Qt::ArrowCursor);
}

void KHUB::handleSharedDownVote(int referenceID) {
  QApplication::setOverrideCursor(Qt::WaitCursor);
  SQL databaseConnection;

  QString url;
  int i = 0;
  for (auto u : sharedContent) {
      if (i == referenceID) {
          url = u.first;
          break;
      }
      i++;
  }
  bool status = databaseConnection.rate(group_id, url, false);
  if (!status) {
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    QMessageBox::critical(0, QObject::tr("Error"), "Could not rate this reference.");
  } else {
      loadReferences();
  }
  QApplication::setOverrideCursor(Qt::ArrowCursor);
}

/**************/
/* Code Recycling */
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
    case (int)ButtonHandler::hl_UpLocalVote:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleLocalUpVote(int)), Qt::UniqueConnection);
      break;
    case (int)ButtonHandler::hl_UpSharedVote:
        connect(map, SIGNAL(mapped(int)), this, SLOT(handleSharedUpVote(int)), Qt::UniqueConnection);
        break;
    case (int) ButtonHandler::hl_LocalUrl:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleLocalUrl(int)), Qt::UniqueConnection);
      break;
    case (int)ButtonHandler::hl_SharedUrl:
        connect(map, SIGNAL(mapped(int)), this, SLOT(handleSharedUrl(int)), Qt::UniqueConnection);
        break;
    case (int)ButtonHandler::hl_DownLocalVote:
      connect(map, SIGNAL(mapped(int)), this, SLOT(handleLocalDownVote(int)), Qt::UniqueConnection);
      break;    
    case (int)ButtonHandler::hl_DownSharedVote:
        connect(map, SIGNAL(mapped(int)), this, SLOT(handleSharedDownVote(int)), Qt::UniqueConnection);
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
    tabs->addTab(browserTab, tr("Browser"));
    tabs->setCurrentWidget(browserTab);

    closeBrowser = new QPushButton();
    // Might seems non sense to overload a fucntion with the same parameter but there are cases where they need to be different - check for other "btSetupInt" lines
    btSetupInt(&closeBrowser, "x", 0, 0, 25, 25, &KHUB::handleDispose, generalMap,(int) ButtonHandler::hl_DisposeBrowser, (int)ButtonHandler::hl_DisposeBrowser);
    tabs->tabBar()->setTabButton(2, QTabBar::RightSide, closeBrowser); // First argument "2" means tab position
}
    
void KHUB::loadReferences(){
  if (sharedLayout->layout() != nullptr) {
    QLayoutItem* item;
    while ((item = sharedLayout->layout()->takeAt(0)) != nullptr) {
      delete item->widget();
      delete item;
    }
  }
  
  SQL databaseConnection;
  sharedContent = databaseConnection.loadReferences(group_id);

  upSharedMap = new QSignalMapper(this);
  downSharedMap = new QSignalMapper(this);
  sharedMap = new QSignalMapper(this);

  // A vector from Labels and QPushButtons needs to be implemented here to avoid memory leak - It will be needed to clean this vector every time the user demands a search ou join/create a group### FIX THIS
  int pos = 0;
  int componentPos = 1;
  for (auto m : sharedContent) {
    QLabel *links = new QLabel();
    links->setText("<a href=\"" + m.first + "\">" + m.first + "</a>");
    links->setTextFormat(Qt::RichText);
    links->setTextInteractionFlags(Qt::TextBrowserInteraction);
    links->setOpenExternalLinks(true);

    QLabel *rates = new QLabel("Ratio: " + QString::number(m.second));

    QPushButton *upArrow;
    btSetupInt(&upArrow, "", 225, 300, 100, 25, &KHUB::handleSharedUpVote, upSharedMap, pos, (int)ButtonHandler::hl_UpSharedVote);
    QIcon ButtonUp("Resources/Arrows/arrow.png");
    upArrow->setIcon(ButtonUp);

    QPushButton *open;
    btSetupInt(&open, "Open", 225, 300, 100, 25, &KHUB::handleSharedUrl, sharedMap, pos, (int)ButtonHandler::hl_SharedUrl);

    QPushButton *downArrow;
    btSetupInt(&downArrow, "", 225, 300, 100, 25, &KHUB::handleSharedDownVote, downSharedMap, pos, (int)ButtonHandler::hl_DownSharedVote);
    QIcon ButtonDown("Resources/Arrows/downarrow.png");
    downArrow->setIcon(ButtonDown);

    sharedLayout->addWidget(links, componentPos, 0, 1, -1, Qt::AlignLeft);
    sharedLayout->addWidget(rates, componentPos, 1, 1, -1, Qt::AlignLeft);
    sharedLayout->addWidget(upArrow, componentPos - 1, 2, 1, -1, Qt::AlignBottom);
    sharedLayout->addWidget(open, componentPos, 2, 1, -1, Qt::AlignCenter);
    sharedLayout->addWidget(downArrow, componentPos + 1, 2, 1, -1, Qt::AlignTop);

    componentPos = componentPos + 5;
    pos++;
  }
  sharedTab->setFocus();
}

