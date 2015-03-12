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

#ifndef sql_H
#define sql_H

#include <QSqlQuery>
#include <QSqlError>


/* C++ Connect Libraries from MySQL */
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define KHUB_CONNECTION "khub connection"

using namespace std;

class SQL
{

public:

	SQL();
	~SQL();
	
	/* Group Control  */
	bool createGroup(int user_id, QString name, QString category, QString subject);

	/* Login Control */
	int checkCredentials(QString login, QString password);

	/* Register Control */
	bool registerUser(QString login, QString password);
	bool checkUser(QString login);

private:

	/* Map to acquire sql credential locally */
	map <int, string> dbAccess;
	
	/* Local config file with DB access information */
	void databaseAccess();

	void closeDB(QSqlQuery query);
};


#endif // sql_H