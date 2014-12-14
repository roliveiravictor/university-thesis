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

class KHUB : public QMainWindow
{
	Q_OBJECT

public:
	KHUB(QWidget *parent = 0);

	void createMenu();	
	void createActions();

private:
	Ui::KHUBClass ui;

	/* Menu Bar */
	QMenu *fileMenu;
	QMenu *groupsMenu;
	QMenu *searchMenu;

	/* File Actions */
	QAction *exitAct;
	QAction *logoutAct;

	/* Groups Actions */
	QAction *createGroupAct;
	QAction *findGroupAct;

	/* Search Actions */
	QAction *newSearchAct;

	/* QT C++ extension for pre-processment */
	private slots:

	/* File  Functions */
	void logout();
	void exit();

	/* Groups  Functions */
	void createGroup();
	void findGroup();

	/* Search Functions */
	void newSearch();

protected:

	/* Events Trigger */
	void contextMenuEvent(QContextMenuEvent *event);

};

#endif // KHUB_H
