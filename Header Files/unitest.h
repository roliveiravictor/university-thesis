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

#ifndef TESTRUNNER_H
#define TESTRUNNER_H

#include <QList>
#include <QTimer>
#include <QCoreApplication>
#include <QtTest>

class UnitTest : public QObject {
  Q_OBJECT

 public:
  UnitTest() : m_overallResult(0){}

  //Add all tests to vector
  void addTest(QObject * test) {
    test->setParent(this);
	m_tests.append(test);
  }

  //Processes vector tests
  bool runTests() {
    int argc = 0;
	char * argv[] = { 0 };
	
    QCoreApplication app(argc, argv);
	QTimer::singleShot(0, this, SLOT(run()));
	app.exec();

    return m_overallResult == 0;
  }

 private slots:
  void run() {
    doRunTests();
	QCoreApplication::instance()->quit();
  }
 private:
	void doRunTests() {
		foreach(QObject * test, m_tests) {
			m_overallResult |= QTest::qExec(test);
		}
	}
	QList<QObject *> m_tests;
	int m_overallResult;
};

#endif // TESTRUNNER_H