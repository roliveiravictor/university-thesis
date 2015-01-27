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
#include "sql.h"

SQL::SQL()
{
	try
	{
		//Acquire sql credential
		databaseAccess();

		database = QSqlDatabase::addDatabase("QMYSQL");
		
		//Set relevant settings
		database.setHostName(dbAccess[0].data());
		database.setPort(atoi(dbAccess[1].data()));
		database.setDatabaseName(dbAccess[2].data());
		database.setUserName(dbAccess[3].data());
		database.setPassword(dbAccess[4].data());
		
		//Open the database
		if (!database.open()) 
		{
			QMessageBox::critical(0, QObject::tr("Database Error"), database.lastError().text());
		}
	}
		catch (sql::SQLException &e) 
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
		}
}

bool SQL::checkCredentials(QString login, QString password)
{
	try
	{
		QSqlQuery query;
		query.prepare("SELECT user_name from users where user_name ='" + login + "'");
		query.exec();

		if (query.size() != 0)
			return true;
		else
			return false;
	}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();

			return false;
		}
}

//Open local file and gets credential
void SQL::databaseAccess()
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

//# INSERT ? INSERT INTO users(user_name, user_password) VALUES('victor', 'roliveira');