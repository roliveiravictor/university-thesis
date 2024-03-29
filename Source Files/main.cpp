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

#include "unitest.h"
#include "khub.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);

  // Modifies UI - Esthetic purposes
  QApplication::setStyle(QStyleFactory::create("Fusion"));

  // Enable plugins for native browser - Helps to avoid crash due to a known bug by Qt at https://bugreports.qt.io/browse/QTBUG-34572 and also in a lot of other reports
  QWebSettings::globalSettings()->setAttribute(QWebSettings::PluginsEnabled, true);
  QWebSettings::globalSettings()->setAttribute(QWebSettings::AutoLoadImages, true);

  KHUB loginWindow;
  loginWindow.create_LoginScreen(loginWindow);
  loginWindow.show();

  /* Unit Test implementation to be checked
  Unit Test
  UnitTest test;
  test.addTest(loginWindow);

  qDebug() << "Overall result: " << (test.runTests() ? "PASS" : "FAIL");*/

  return application.exec();
}