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
#include "http.h"

HTTP::HTTP(){
	
}

void HTTP::sendRequest(QString keyword, bool Http302) {
  QNetworkRequest request;

  // create custom temporary event loop on stack
  QEventLoop eventLoop;

  // "quit()" the event-loop, when the network request "finished()"
  QNetworkAccessManager manager;
  QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

  // the HTTP request
  if (Http302) {
	request.setUrl(QUrl(QString("http://google.com/search?q=" + keyword))); // main query throws at first a 302 http error - needs two requests to fix this
  } else {
	vector<QString> aux = readReferences("302.html");
	request.setUrl(QUrl(aux.at(0))); // get 302 reference - only exists one, thus position 0
  }
		
  QNetworkReply *reply = manager.get(request);
  eventLoop.exec(); // blocks stack until "finished()" has been called

  if (reply->error() == QNetworkReply::NoError) {
    // success
	if (Http302) {
	  writeReferences(reply, "302.html");
	  clean302Reference();
	} else {
	  writeReferences(reply, "Main Query Cleaned.txt");
	  cleanMainReference();
	}
    delete reply;
  } else {
	// failure
	qDebug() << "Failure" << reply->errorString();
	delete reply;
	}
}

// Make the first http request which triggers a 302 error (moved page)
void HTTP::writeReferences(QNetworkReply* reply, string path) {
  ofstream refFile;

  refFile.open(path);
  refFile << reply->readAll().constData();
  refFile.close();
}

// Open 302 reference and remove useless lines - keep the moved link page
void HTTP::clean302Reference() {
  QString line;
  QFile refFile("302.html");

  if (refFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
	QTextStream out(&refFile);
	while (!out.atEnd()) {
	     line = out.readLine();
	     if (line.contains("REF"))
		   break;
	}
    // remove HTML tags and clean link
    line = line.remove(0,line.indexOf("\"") + 1);
	line = line.remove(line.lastIndexOf("\""), line.size());

	// delete the entire file and rewrite only the 302 link
	refFile.resize(0);
    out << line;
  } else {
	qDebug() << "Failure while cleaning 302 reference";
  }
  refFile.close();
}

// Open Main reference and remove useless lines - keep the moved link page
void HTTP::cleanMainReference() {
  vector<QString> references;
  QString line;
  QFile refFile("Main Query Cleaned.txt");

  if (refFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
    QTextStream out(&refFile);

    // Main Cleaning - This will remove most of the useless content
    while (!out.atEnd()) {
         line = out.readLine();

         // cleanning begin
	     line = line.remove(0, line.indexOf("<ol>")); // OL tag carries google's searched references - everything till this point is useless

         // cleanning end
         line = line.remove(line.indexOf("</ol>"), line.size());
    }

    // Move cursor backward to its initial position
    out.seek(0);

    // Surgical Cleaning - This will acquire the references to be writen
    while (!out.atEnd()) {
        line = out.readLine();

        /*************************************************************************************************/
        /* This cleaning can be improved - As it's, it retrieves 70% of the references provided by google*/
        /*************************************************************************************************/
        line = line.remove(0, line.indexOf("url?q="));
        line = line.remove(line.indexOf("&amp;sa"), line.size());
        
        // Discard lines without references
        if (!line.contains("url?q=") || !line.contains("http"))
          line = "";
        else
          // Replace found references with initial "url?q=" for "" (nothing) - This last line will make references ready for usage
          line = line.replace("url?q=", "") + "\n";

        qDebug() << line;
        references.push_back(line);
    }

	// delete the entire file and rewrite only cleaned references - fix accents
	refFile.resize(0);
        
    for (auto writer = references.begin(); writer != references.end(); ++writer)
       out << *writer;
  } else {
	  qDebug() << "Failure while cleaning Main reference";
  }
  refFile.close();
}

// Gets links references
vector<QString> HTTP::readReferences(QString path) {
  vector<QString> references;
  QString line;
  QFile refFile(path);

  if (refFile.open(QIODevice::ReadWrite | QIODevice::Text)) {
	QTextStream out(&refFile);
  	while (!out.atEnd())
	     references.push_back(out.readLine());
  } else {
	qDebug() << "Failure while reading references";
  }
  refFile.close();
  return references;
}




