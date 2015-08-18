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

#ifndef HTTP_H
#define HTTP_H

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QXmlStreamReader>
#include <QString>
#include <QtXml>

#include <iostream>
#include <fstream>

using namespace std;

class HTTP {
 public:
  HTTP();

  void sendRequest(QString keyword, bool Http302);

 private:
  //HTML reference links parsing
  void clean302Reference();
  void cleanMainReference();

  //Read-write operations for references
  vector<QString> readReferences(QString path);
  void writeReferences(QNetworkReply* reply, string path);
};

#endif // KHUB_H