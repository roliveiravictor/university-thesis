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
#include <QSqlRecord>

// C++ Connect Libraries from MySQL
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// Standard C++ includes 
#include <stdlib.h>
#include <iostream>
#include <fstream>

#define KHUB_CONNECTION "khub connection"

using namespace std;

class SQL {
public:
    SQL();
    ~SQL();

    /******************/
    /* Group Control  */
    /******************/

    bool createGroup(int user_id, QString name, QString category, QString subject);
    bool joinGroup(int user_id, int group_id);
    bool rate(int group_id, QString link, bool isUpVote);
    bool exclude(int group_id, QString link);
    vector<QString> checkGroup(int group_id);
    vector<pair<QString, int>> loadReferences(int group_id);

    /*****************/
    /* Login Control */
    /*****************/

    int checkCredentials(QString login, QString password);

    /********************/
    /* Register Control */
    /********************/

    bool registerUser(QString login, QString password);
    bool checkUser(QString login);

private:
    vector<QString> groupData;

    //Verify rate type and update reference value
    bool linkExe(QSqlQuery query, bool isUpvote, QString link, int group_id);

    // Map to acquire sql credential locally
    map <int, string> dbAccess;

    // Local config file with DB access information
    void databaseAccess();

    // Close query and disconnect form DB
    void closeDB(QSqlQuery query);

    void throwSQLError(sql::SQLException *e);
};

#endif // sql_H