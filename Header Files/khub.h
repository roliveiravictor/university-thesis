/***********************************************************************/
/*                                                                     */
/*         Subject: KHUB Thesis										   */
/*		   Author: Victor Rocha                                        */
/*																	   */
/*		 The name of this tool is Knowledge HUB (KHUB) and			   */
/*		 its purpose is educational. The idea is to share			   */
/*		 references (knowledge) between students inside or			   */
/*		 outside the classroom, thus helping students'				   */
/*		 learning or professors' teaching.							   */
/*                                                                     */
/***********************************************************************/
/*                                                                     */
/* READ ME : I know a lot of raw pointers have been used here and that */
/* is totally wrong according to Bjarne Stroustrup recommendations.    */
/* By the time I did this, I was starting my studies with C++ and I    */
/* didn't know that I should have used shared_ptr and unique_ptr to    */
/* avoid memory leaks in a C++ application.                            */
/*                                                                     */
/* Anyway, Those pointers here are being tracked by a special library  */
/* (as you can see at khub.cpp) to handle memory leaks.Every pointer,  */
/* after its use, is being manually deleted so application's           */
/* performance won't be damaged.                                       */
/*                                                                     */
/* It's important to say that Qt has its own smart pointers classes    */
/* like QSharedPointer and QScopedPointer.I might change the code in   */ 
/* the future to know better how this works, but I'm afraid I won't    */
/* have the necessary time until my Thesis deadline.                   */
/*                                                                     */
/***********************************************************************/

#ifndef KHUB_H
#define KHUB_H

#include <QtWidgets/QMainWindow>
#include <qwebview.h>
#include "ui_khub.h"
#include "sql.h"
#include "http.h"

#include <cstdlib>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

class KHUB : public QMainWindow {
  Q_OBJECT
  Q_ENUMS(CancelType)

 public:
  KHUB(QWidget *parent = 0);
  ~KHUB();

  /******************/
  /* Create Screens */
  /******************/

  void create_LoginScreen(KHUB& loginWindow);
  void create_MainScreen(int user_id);

	enum class CancelType {
		cl_newGroup = 10,
		cl_joinGroup = 11,
		cl_search = 20,
	};

 private:

  Ui::KHUBClass ui;

  // Reference to global user id
  int user_id;

  /****************/
  /* KHUB Objects */
  /****************/

  KHUB *loginWindowPtr;

  /******************/
  /* Create Screens */
  /******************/

  //Login Screens
  void create_RegisterScreen();

  // Main Window Screens
  //void create_MainScreen(int user_id);
  void create_GroupScreen(bool isCreate);

  /******************/
  /* Dialog Objects */
  /******************/

  /******************/
  /*     Widgets    */
  /******************/

  QTabWidget *tabs;

  QWidget *newGroupDialog, *joinGroupDialog, *searchDialog;

  /******************/
  /* Create Dialogs */
  /******************/

  void dialog_NewGroup();
  void dialog_JoinGroup();
  void dialog_Search();


  /*****************/
  /* Login Buttons */
  /*****************/

  QPushButton *loginBt, *registerBt = NULL, *cancelRegisterBt;

  /*******************/
  /* Login TextField */
  /*******************/

  QLineEdit *loginEdt, *passwordEdt, *loginConfirmEdt, *passwordConfirmEdt;
	
  /***********************/
  /* Main Window Buttons */
  /***********************/

  //Group Buttons
  QPushButton *createGroupBt, *joinGroupBt;

  //Search Buttons
  QPushButton *searchBt;

  //General Buttons
  QPushButton *cancelBt;
	
  /*************************/
  /* Main Window TextField */
  /*************************/
	
  //Group TextField
  QLineEdit *groupIdEdt, *groupNameEdt, *groupCategoryEdt, *groupSubjectEdt;
		
  //Search Textfield
  QLineEdit *searchEdt;

  /**********************************/
  /* Create Main Window Components */
  /*********************************/

  void set_Menu();
  void set_Actions();
 
  /***********************/
  /* Main Window Actions */
  /***********************/

  // File Actions
  QAction *exitAct, *logoutAct;

  // Groups Actions
  QAction *createGroupAct, *joinGroupAct;

  // Search Actions
  QAction *searchAct;

  /********************************************/
  /* General Signal map for different senders */
  /********************************************/

  QSignalMapper *signalMapper;

  /*******************/
  /* General Layouts */
  /*******************/

  QVBoxLayout *boxLayout;

  QGridLayout *gridLayout;


  /****************************/
  /* General Components Setup */
  /****************************/

  void btSetup(QPushButton **button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)());
  void btSetupInt(QPushButton **button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)(int parameter), int value);

  void btBoxSetup(QPushButton **button, const QString name, void (KHUB::*fptr)());
  void btBoxSetupInt(QPushButton **button, const QString name, void (KHUB::*fptr)(int parameter), int slot);

  void txtFieldSetup(QLineEdit **textField, const QString name, int posX, int posY, int width, int height, bool isPassword);

  void txtFieldBoxSetup(QLineEdit **textField, const QString name, bool isPassword);

 /****************************************/
 /* QT C++ extension for pre-processment */
 /****************************************/

 private slots:

  /**************************/
  /* Login Screen Functions */
  /**************************/

  void handleReboot();
  void handleDispose(int slot);
  void handleLogin();
  void handleRegister(int isRegister);
  void handleUrl();

  /*************************/
  /* Main Window Functions */
  /*************************/
	
  // File Functions
  void exit();
  void logout();

  // Groups  Functions
  void createGroup();
  void joinGroup();

  void handleNewGroup();
  void handleJoinGroup();

  // Search Functions 
  void search();

  void handleSearch();
};

#endif // KHUB_H