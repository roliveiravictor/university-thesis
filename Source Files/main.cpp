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

#include <QtWidgets/QApplication>
#include "khub.h"


int main(int argc, char *argv[])
{
	QApplication application(argc, argv);

	KHUB loginWindow;
	
	loginWindow.createLoginScreen(&loginWindow);
	loginWindow.show();

	return application.exec();
}