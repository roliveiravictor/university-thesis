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

    groupData.push_back(name);
    groupData.push_back(category);
    groupData.push_back(subject);

	query.prepare("INSERT INTO groups(user_id, group_name, group_category, group_subject) VALUES ('" + QString::number(user_id) + "', '" + name + "', '" + category + "', '" + subject + "')");
    
    bool status = query.exec();
	//DB is not being closed here because right after the group is created, he is checked and at SQL::checkGroup the DB is closed
    //closeDB(query);

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

	query.prepare("SELECT * FROM groups WHERE group_id ='" + QString::number(group_id) + "'");

	bool status = query.exec();
	//closeDB(query);
    //DB is not being closed here because right someone joins a group, he is checked and at SQL::checkGroup the DB is closed

	return status;
  } catch (sql::SQLException &e) {
      qDebug() << "# ERR: SQLException in " << __FILE__;
      qDebug() << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
      qDebug() << "# ERR: " << e.what();

      QSqlDatabase::removeDatabase(KHUB_CONNECTION);

      return false;
   }
}

bool SQL::rate(int group_id, QString link, bool isUpVote) {
  try {
    bool status;

    QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));

    query.prepare("SELECT ref_id FROM refs WHERE `ref_hyperlink` = '" + link + "' AND `group_id` = '" + QString::number(group_id) + "'");
    query.exec();

    if (query.size() == 0) {
      query.prepare("INSERT INTO refs(group_id, ref_hyperlink, ref_ratio) VALUES ('" + QString::number(group_id) + "', '" + link + "', '" + QString::number(0) + "')");
      query.exec();
      status = linkExe(query, isUpVote, link, group_id);
    } else {
        status = linkExe(query, isUpVote, link, group_id);
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
bool SQL::linkExe(QSqlQuery query, bool isUpVote, QString link, int group_id) {
  if (isUpVote) {
      query.prepare("UPDATE refs SET `ref_ratio` = `ref_ratio` + 1 WHERE `ref_hyperlink` ='" + link + "' AND `group_id` = '" + QString::number(group_id) + "'");
  } else {
      query.prepare("UPDATE refs SET `ref_ratio` = `ref_ratio` - 1 WHERE `ref_hyperlink` ='" + link + "' AND `group_id` = '" + QString::number(group_id) + "'");
    }
  return query.exec();
}

//Load references for shared tab
map<QString, int> SQL::loadReferences(int group_id) {
    try {
      map<QString, int> data;
      QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
      query.prepare("SELECT ref_ratio, ref_hyperlink FROM refs WHERE `group_id` ='" + QString::number(group_id) + "'");

      bool status = query.exec();
      if (status) {
        while(query.next()) {
          /*According to the SELECT above : 1 is ref_hyperlink - 0 is ref_ratio
          This way, referecenes only will be considered if they have a positive balance*/
          if (query.value(0).toInt()>0)
            data[query.value(1).toString()] = query.value(0).toInt();
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

vector<QString> SQL::checkGroup(int group_id) {
  try {
    QSqlQuery query(QSqlDatabase::database(KHUB_CONNECTION));
    if (!groupData.empty())
        query.prepare("SELECT group_id FROM groups WHERE `group_name` ='" + groupData.at(0) + "' AND `group_category` ='" + groupData.at(1) + "' AND `group_subject` ='" + groupData.at(2) + "'");
    else
        query.prepare("SELECT group_name, group_category, group_subject, group_id FROM groups WHERE `group_id` ='" + QString::number(group_id) + "'");

    bool status = query.exec();
    query.first();

    if (!groupData.empty())
        //Get group_id to groupData vector
        groupData.push_back(query.value(0).toString());
    else
      for (int i = 0; i < query.size(); i++)
          groupData.push_back(query.value(i).toString());

    closeDB(query);

    return groupData;
  }  catch (sql::SQLException &e) {
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


