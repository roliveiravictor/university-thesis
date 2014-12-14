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
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	KHUB mainWindow;
	
	//Sets main full screen and shows
	mainWindow.setWindowState(mainWindow.windowState() ^ Qt::WindowMaximized);
	mainWindow.createActions();
	mainWindow.createMenu();
	mainWindow.show();

	return application.exec();
}
