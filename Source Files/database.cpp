v/**********************************************************************/
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
#include <database.h>

Database::Database()
{
	try
	{
		//Acquire database credential
		databaseAccess();

		//Create a connection
		driver = get_driver_instance();
		con = driver->connect(dbAccess[0].data(), dbAccess[1].data(), dbAccess[2].data());

		///Connect to the MySQL test database
		con->setSchema("mycrawler");
	}
		catch (sql::SQLException &e) 
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
		}
}

void Database::query()
{
	try
	{
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT servidor FROM rag");

		while (res->next())
		{
			//Access column fata by numeric offset, 1 is the first column or set column name as string parameter
			cout << res->getString(1).c_str() << endl;
		}
	}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
		}

	delete res;
	delete stmt;
	delete con;
}

//Open local file and gets credential
void Database::databaseAccess()
{
	int counter = 0;
	string line;
	ifstream configFile("config.txt");

	if (configFile.is_open())
	{
		while (getline(configFile, line))
		{
			dbAccess[counter] = line;
			counter++;
		}
		configFile.close();
	}
		else
			cout << "Unable to get file (reason: " << strerror(errno) << ")." << endl;
}