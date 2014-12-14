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

//Events handler
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
