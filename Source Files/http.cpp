#include <stdafx.h>
#include <http.h>

#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>



HTTP::HTTP()
{
	
}

void HTTP::SendRequest(){

	// create custom temporary event loop on stack
	QEventLoop eventLoop;

	// "quit()" the event-loop, when the network request "finished()"
	QNetworkAccessManager mgr;
	QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

	// the HTTP request
	QNetworkRequest req(QUrl(QString("http://www.google.com.br")));
	QNetworkReply *reply = mgr.get(req);
	eventLoop.exec(); // blocks stack until "finished()" has been called

	if (reply->error() == QNetworkReply::NoError) {
		//success
		qDebug() << "Success" << reply->readAll();
		delete reply;
	}
	else {
		//failure
		qDebug() << "Failure" << reply->errorString();
		delete reply;
	}
}