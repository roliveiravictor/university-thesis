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

void HTTP::sendRequest()
{

	// create custom temporary event loop on stack
	QEventLoop eventLoop;

	// "quit()" the event-loop, when the network request "finished()"
	QNetworkAccessManager mgr;
	QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

	// the HTTP request
	QNetworkRequest req(QUrl(QString("http://google.com/search?q=Artificial+Intelligence")));
	QNetworkReply *reply = mgr.get(req);
	eventLoop.exec(); // blocks stack until "finished()" has been called

	if (reply->error() == QNetworkReply::NoError) {
		//success
		
		writeLinks(reply);
		delete reply;
	}
	else
	{
		//failure
		qDebug() << "Failure" << reply->errorString();
		delete reply;
	}
}

void HTTP::writeLinks(QNetworkReply* reply)
{
	ofstream myfile;

	myfile.open("references.html");
	
	/*//Convert HTML to XML - Tag strip
	QXmlStreamReader xml(reply->readAll().constData());
	QString textString;

	while (!xml.atEnd()) 
	{
		if (xml.readNext() == QXmlStreamReader::Characters)
		{
			textString += xml.text();
			//textString.remove(QRegExp("<[^>]*>"));

			myfile << textString.constData();

			qDebug() << textString;

		}
	}*/

	qDebug() << reply->readAll().constData();

	myfile.close();

}