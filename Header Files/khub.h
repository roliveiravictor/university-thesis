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

#ifndef KHUB_H
#define KHUB_H

#include <QtWidgets/QMainWindow>
#include "ui_khub.h"
#include "sql.h"
#include "http.h"

#include <cstdlib>
#include <stdio.h>

#define FALSE 0
#define TRUE 1

class KHUB : public QMainWindow
{
	Q_OBJECT

	Q_ENUMS(CancelType)

public:

	KHUB(QWidget *parent = 0);
	~KHUB();

	/* Create Screens */
	void create_LoginScreen(KHUB& loginWindow);
	void create_MainScreen(int user_id);

	enum class CancelType
	{
		cl_newGroup = 10
	};

private:

	Ui::KHUBClass ui;

	/* Reference to global user id */
	int user_id;

	/* Screens */
	KHUB* mainWindow, *loginWindowPtr;

	QWidget* groupDialog;

	/* Layouts */
	QVBoxLayout* boxLayout;

	/* Buttons */
	QPushButton* loginBt, *registerBt = NULL, *cancelRegisterBt, *createGroupBt, *cancelGroupBt;

	/* TextField */
	QLineEdit* loginEdt, *passwordEdt, *loginConfirmEdt, *passwordConfirmEdt,
				*groupNameEdt, *groupCategoryEdt, *groupSubjectEdt;

	/* File Actions */
	QAction* exitAct, *logoutAct;

	/* Groups Actions */
	QAction* createGroupAct, *findGroupAct;

	/* Search Actions */
	QAction* newSearchAct;

	/* Signal map for different senders */
	QSignalMapper* signalMapper;

	/* Create Components */
	void set_Menu();
	void set_Actions();

	/* Create Screens */
	//void create_MainScreen(int user_id);
	void create_RegisterScreen();

	void mainWindow_GroupScreen();

	/* Create Dialogs */
	void dialog_NewGroup();

	/* Components Setup */
	void btSetup(QPushButton** button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)());
	void btSetupInt(QPushButton** button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)(int parameter), int value);

	void btBoxSetup(QPushButton **button, const QString name, void (KHUB::*fptr)());
	void btBoxSetupInt(QPushButton** button, const QString name, void (KHUB::*fptr)(int parameter), int slot);

	void txtFieldSetup(QLineEdit** textField, const QString name, int posX, int posY, int width, int height, bool isPassword);

	void txtFieldBoxSetup(QLineEdit** textField, const QString name, bool isPassword);

/* QT C++ extension for pre-processment */
private slots:

	/* Main Screen Functions */
	
	/* File Functions */
	void exit();
	void logout();

	/* Groups  Functions */
	void createGroup();
	void findGroup();

	/* Search Functions */
	void newSearch();

	/* Login Screen Functions */
	void handleReboot();
	void handleDispose(int slot);
	void handleLogin();
	void handleRegister(int isRegister);
	void handleNewGroup();

};

#endif // KHUB_H