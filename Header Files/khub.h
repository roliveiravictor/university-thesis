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

class KHUB : public QMainWindow
{
	Q_OBJECT

public:

	KHUB(QWidget *parent = 0);

	/* Create Screens */
	void createLoginScreen(KHUB *loginWindow);

private:

	Ui::KHUBClass ui;

	KHUB* mainWindow, *loginWindowPtr;

	/* Buttons */
	QPushButton* loginButton, *registerButton, *cancelButton;

	/* TextField */
	QLineEdit* loginEdit, *passwordEdit, *loginConfirmEdit, *passwordConfirmEdit;

	/* Menu Bar */
	QMenu* fileMenu, *groupsMenu, *searchMenu;

	/* File Actions */
	QAction* exitAct, *logoutAct;

	/* Groups Actions */
	QAction* createGroupAct, *findGroupAct;

	/* Search Actions */
	QAction* newSearchAct;

	/* Create Components */
	void createMenu();
	void createActions();

	/* Create Screens */
	void createMainScreen();
	void createRegisterScreen();

	/* Components Setup */
	void buttonSetup(QPushButton** button, const QString name, int posX, int posY, int width, int height, void(KHUB::*fptr)());
	void textFieldSetup(QLineEdit** textField, const QString name, int posX, int posY, int width, int height, bool isPassword);

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
	void handleCancel();
	void handleLogin();
	void handleRegister();
};

#endif // KHUB_H
