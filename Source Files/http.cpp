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


#include <stdafx.h>
#include <http.h>

HTTP::HTTP()
{
	
}

void HTTP::sendRequest(QString keyword)
{
	// create custom temporary event loop on stack
	QEventLoop eventLoop;

	// "quit()" the event-loop, when the network request "finished()"
	QNetworkAccessManager manager;
	QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

	// the HTTP request
	QNetworkRequest request(QUrl(QString("http://google.com/search?q=" + keyword)));
	QNetworkReply *reply = manager.get(request);
	eventLoop.exec(); // blocks stack until "finished()" has been called

	if (reply->error() == QNetworkReply::NoError)
	{
		//success
		cout << "\n\n\n#### Success on Query ####\n\n\n";
		force302(reply);
		delete reply;
	}
	else
	{
		//failure
		cout << "\n\n\n#### Failure on Query ####\n\n\n";
		qDebug() << "Failure" << reply->errorString();
		delete reply;
	}
}

//Make the first http request which triggers a 302 error (moved page)
void HTTP::force302(QNetworkReply* reply)
{
	ofstream refFile;

	refFile.open("references.html");
	refFile << reply->readAll().constData();
	refFile.close();

	clean302Reference();
}

//Open 302 reference and remove useless lines - keep the moved link page
void HTTP::clean302Reference()
{
	QString line;
	QFile refFile("references.html");

	if (refFile.open(QIODevice::ReadWrite | QIODevice::Text))
	{
		QTextStream out(&refFile);
		while (!out.atEnd())
		{
			line = out.readLine();
			if (line.contains("REF"))
				break;
		}

		//remove HTML tags and clean link
		line = line.remove(0,line.indexOf("\"") + 1);
		line = line.remove(line.lastIndexOf("\""), line.size());
		

		//delete the entire file and rewrite only the 302 link
		refFile.resize(0);
		cout << line.toStdString();
		out << line;
	}

	refFile.close();
}

//Gets links references from the moved page
void HTTP::acquireReferences()
{

}




