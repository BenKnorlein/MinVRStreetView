#include "ImageGrab.h"
#include "MYQApp.h"
#include <iostream>
#include <QThread>


ImageGrab::ImageGrab(QString panoID) :QObject(), m_nbDownloads(0){
	MYQapp::getInstance();
	//page = new QWebPage();
	
	manager = new QNetworkAccessManager();
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(replyFinished(QNetworkReply*)));

	for (int y = 0; y < 13; y++){
		for (int x = 0; x < 26; x++){
			m_nbDownloads++;
			QString url = "http://cbk0.google.com/cbk?output=tile&panoid=" + panoID + "&zoom=5&x=" + QString::number(x) + "&y=" + QString::number(y);
			QNetworkRequest  req(url);
			manager->get(req);
		}
	}
	//with screenshot
	//connect(page->mainFrame(), SIGNAL(loadFinished(bool)), this, SLOT(save()));
	//page->mainFrame()->load(QUrl(url));
	//page->setViewportSize(QSize(512, 512));
}

ImageGrab::~ImageGrab()
{
	//page->settings()->clearMemoryCaches();
	//delete page;
	delete manager;
}

void ImageGrab::replyFinished(QNetworkReply* reply)
{
	QString url = reply->request().url().toString();
	QStringList parts = url.split("&");
	int x, y;

	for (QStringList::iterator it = parts.begin();
		it != parts.end(); ++it) {
		
			if (it->startsWith("x="))
			{
				x = it->mid(2, it->length() - 2).toInt();
			}
			else if (it->startsWith("y="))
			{
				y = it->mid(2, it->length() - 2).toInt();
			}
	}
	QFile file("data/tile" + QString::number(x) + "_" + QString::number(y) + ".jpg");
	file.open(QIODevice::WriteOnly);
	file.write(reply->readAll());
	file.close();
	m_nbDownloads--;
	reply->deleteLater();
}

void ImageGrab::saveScreenshot()
{
	//QImage image(page->viewportSize(), QImage::Format_ARGB32);
	//if (!image.isNull()){
	//	QPainter painter(&image);

	//	page->mainFrame()->render(&painter);
	//	painter.end();

	//	QImage thumbnail = image.scaled(512, 512);
	//	thumbnail.save("tile" + QString::number(m_x) + "_" + QString::number(m_y) + ".png");
	//	emit finished();
	//	m_dataReady = true;
	//}
}

bool ImageGrab::hasDataReady()
{
	return (m_nbDownloads == 0);
}
