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

SQL::~SQL()
{

}

/*****************/
/* Group Control */
/*****************/

bool SQL::createGroup(int user_id, QString name, QString category, QString subject)
{
	try
	{
		QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

		query.prepare("INSERT INTO groups(user_id, group_name, group_category, group_subject) VALUES ('" + QString::number(user_id) + "', '" + name + "', '" + category + "', '" + subject + "')");
		
		bool status = query.exec();
		closeDB(query);

		return status;
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

/*****************/
/* Login Control */
/*****************/

//Check and confirm login information
int SQL::checkCredentials(QString login, QString password)
{
	try
	{
		//Building QSqlQuery object and passing database setup
		QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
		
		query.prepare("SELECT user_id FROM users where user_name ='" + login + "' AND user_password='" + password + "'");
		query.exec();
		query.first();

		if (query.size() != 0)
		{
			int user_id = query.value(0).toInt();

			closeDB(query);
			return user_id;
		}			
		else
		{
			QMessageBox::critical(0, QObject::tr("Error"), "Your login information was incorret. Please try again.");
			closeDB(query);

			return NULL;
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
bool SQL::registerUser(QString login, QString password)
{
	QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

	query.prepare("INSERT INTO users(user_name, user_password) VALUES ('" + login + "', '" + password + "')");
	bool status = query.exec();
	
	closeDB(query);

	return status;
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

void SQL::closeDB(QSqlQuery query)
{
	query.clear();
	QSqlDatabase::removeDatabase(KHUB_CONNECTION);
}


