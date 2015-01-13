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

/* Standard C++ includes */
#include <stdlib.h>
#include <iostream>
#include <fstream>

/* C++ Connect Libraries from MySQL */
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

class SQL
{

public:

	SQL();
	
	void Query();

private:

	//Map to acquire sql credential locally
	map <int, string> dbAccess;

	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::ResultSet* res;

	void DatabaseAccess();

};


#endif // sql_H