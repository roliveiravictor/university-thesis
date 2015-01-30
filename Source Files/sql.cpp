﻿/**********************************************************************/
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

		QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", KHUB_CONNECTION);
		
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


			QSqlDatabase::removeDatabase(KHUB_CONNECTION);
		}
}

/*****************/
/* Login Control */
/*****************/

//Check and confirm login information
bool SQL::checkCredentials(QString login, QString password)
{
	try
	{
		//Building QSqlQuery object and passing database setup
		QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
		
		query.prepare("SELECT user_name FROM users where user_name ='" + login + "' AND user_password='" + password + "'");
		query.exec();

		if (query.size() != 0)
			return true;
		else
		{
			QMessageBox::critical(0, QObject::tr("Error"), "Your login information was incorret. Please try again.");

			//Closing Database
			query.clear();
			QSqlDatabase::removeDatabase(KHUB_CONNECTION);

			return false;
		}
			
	}
		catch (sql::SQLException &e)
		{
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();


			QSqlDatabase::removeDatabase(KHUB_CONNECTION);

			return false;
		}
}

//Check if user exists
bool SQL::checkUser(QString login)
{
	QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

	query.prepare("SELECT user_name FROM users where user_name ='" + login + "'");
	query.exec();

	if (query.size() != 0)
		return true;
	else
		return false;
}

//Apply for new user
void SQL::registerUser(QString login, QString password)
{
	QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

	query.prepare("INSERT INTO users(user_name, user_password) VALUES('" + login + "', '" + password + "')");
	query.exec();
	query.clear();

	QSqlDatabase::removeDatabase(KHUB_CONNECTION);
}

/**********************/
/* Local Information  */
/**********************/

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