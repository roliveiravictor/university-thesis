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

SQL::SQL() {
  try {
    // Acquire sql credential
	databaseAccess();

	QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL", KHUB_CONNECTION);
	
	// Set relevant settings
	database.setHostName(dbAccess[0].data());
	database.setPort(atoi(dbAccess[1].data()));
	database.setDatabaseName(dbAccess[2].data());
	database.setUserName(dbAccess[3].data());
	database.setPassword(dbAccess[4].data());
	
    // Open the database
	if (!database.open()) {
	  QMessageBox::critical(0, QObject::tr("Database Error"), database.lastError().text());
	}
  } catch (sql::SQLException &e) {
	     qDebug() << "# ERR: SQLException in " << __FILE__;
		 qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		 qDebug() << "# ERR: " << e.what();

		 QSqlDatabase::removeDatabase(KHUB_CONNECTION);
	}
}

SQL::~SQL() {

}

/******************/
/* Search Control */
/******************/

bool SQL::search(QString keywords) {
  return NULL;
}

/*****************/
/* Group Control */
/*****************/

bool SQL::createGroup(int user_id, QString name, QString category, QString subject) {
  try {
     QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

	 query.prepare("INSERT INTO groups(user_id, group_name, group_category, group_subject) VALUES ('" + QString::number(user_id) + "', '" + name + "', '" + category + "', '" + subject + "')");
	
     bool status = query.exec();
	 closeDB(query);

	 return status;
   } catch (sql::SQLException &e) {
	   qDebug() << "# ERR: SQLException in " << __FILE__;
	   qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	   qDebug() << "# ERR: " << e.what();

	   QSqlDatabase::removeDatabase(KHUB_CONNECTION);

	   return false;
  }
}

bool SQL::joinGroup(int user_id, int group_id) {
  try {
	QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

	query.prepare("SELECT * FROM groups where group_id ='" + QString::number(group_id) + "'");

	bool status = query.exec();
	closeDB(query);

	return status;
  } catch (sql::SQLException &e) {
	  qDebug() << "# ERR: SQLException in " << __FILE__;
	  qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	  qDebug() << "# ERR: " << e.what();

	  QSqlDatabase::removeDatabase(KHUB_CONNECTION);

      return false;
  }
}

bool SQL::rate(int user_id, int group_id, QString link, bool isUpVote) {
  try {
    bool status;

    QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

    query.prepare("SELECT * FROM refs WHERE `ref_hyperlink` ='" + link + "'");
    query.exec();
    query.first();

    if (query.size() == 0) {
      query.prepare("INSERT INTO refs(user_id, group_id, ref_hyperlink, ref_ratio) VALUES ('" + QString::number(user_id) + "', '" + QString::number(group_id) + "', '" + link + "', '" + QString::number(0) + "')");
      query.exec();
      status = linkExe(query, isUpVote, link);
    } else {
        status = linkExe(query, isUpVote, link);
    }
    closeDB(query);

    return status;
  } catch (sql::SQLException &e) {
      qDebug() << "# ERR: SQLException in " << __FILE__;
      qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
      qDebug() << "# ERR: " << e.what();
    
      QSqlDatabase::removeDatabase(KHUB_CONNECTION);
      return false;
  }
}

//Update rates from voting
bool SQL::linkExe(QSqlQuery query, bool isUpVote, QString link) {
  if (isUpVote) {
    query.prepare("UPDATE refs SET `ref_ratio` = `ref_ratio` + 1 WHERE `ref_hyperlink` ='" + link + "'");
  } else {
      query.prepare("UPDATE refs SET `ref_ratio` = `ref_ratio` - 1 WHERE `ref_hyperlink` ='" + link + "'");
    }
  return query.exec();
}

//Load references for shared tab
map<QString, int> SQL::loadReferences(int group_id) {
    try {
      map<QString, int> data;
      QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
      query.prepare("SELECT ref_hyperlink, ref_ratio FROM refs WHERE `group_id` ='" + QString::number(group_id) + "'");

      bool status = query.exec();
      query.first();

      if (status) {
        while (query.next()){
          /*According to the SELECT above : 0 is ref_hyperlink - 1 is ref_ratio
          This way, referecenes only will be considered if they have a positive balance*/
          if (query.value(1)>0)
            data[query.value(0).toString()] = query.value(1).toInt();
        }
      }
        QSqlDatabase::removeDatabase(KHUB_CONNECTION);
        return data;
      } catch (sql::SQLException &e) {
          qDebug() << "# ERR: SQLException in " << __FILE__;
          qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
          qDebug() << "# ERR: " << e.what();

          QSqlDatabase::removeDatabase(KHUB_CONNECTION);
    }
}

/*****************/
/* Login Control */
/*****************/

// Check and confirm login information
int SQL::checkCredentials(QString login, QString password) {
  try {
	QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
		
	query.prepare("SELECT user_id FROM users WHERE user_name ='" + login + "' AND user_password='" + password + "'");
	query.exec();
	query.first();

	if (query.size() != 0) {
	  int user_id = query.value(0).toInt();

	  closeDB(query);
	  return user_id;
	} else {
	    QMessageBox::critical(0, QObject::tr("Error"), "Your login information was incorret. Please try again.");
	    closeDB(query);

		return NULL;
	}			
  } catch (sql::SQLException &e) {
	  qDebug() << "# ERR: SQLException in " << __FILE__;
	  qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
	  qDebug() << "# ERR: " << e.what();

	  QSqlDatabase::removeDatabase(KHUB_CONNECTION);

	  return false;
  }
}

// Check if user exists
bool SQL::checkUser(QString login) {
  QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

  query.prepare("SELECT user_name FROM users where user_name ='" + login + "'");
  query.exec();

  if (query.size() != 0)
    return true;
  else
    return false;		
}

// Apply for new user
bool SQL::registerUser(QString login, QString password) {
  QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

  query.prepare("INSERT INTO users(user_name, user_password) VALUES ('" + login + "', '" + password + "')");
  bool status = query.exec();
	
  closeDB(query);

  return status;
}

/**********************/
/* Local Information  */
/**********************/

// Open local file and gets credential
void SQL::databaseAccess() {
  int counter = 0;
  string line;
  ifstream configFile("config.txt");

  if (configFile.is_open()) {
    while (getline(configFile, line)) {
	     dbAccess[counter] = line;
	     counter++;
	}
    configFile.close();
    } else {
	  qDebug() << "Unable to get file (reason: " << strerror(errno) << ")." << endl;
    }
}

void SQL::closeDB(QSqlQuery query) {
  query.clear();
  QSqlDatabase::removeDatabase(KHUB_CONNECTION);
}


