#include "QTImageDownload.h"
#include <iostream>
#include <QThread>

MYQapp* MYQapp::instance = NULL;

MYQapp::MYQapp()
{
	static int argc = 1;
	static char* argv[] = { (char*)"dummy.exe", NULL };
	app = new QApplication(argc, argv);
}

MYQapp::~MYQapp()
{
	delete app;
}

MYQapp* MYQapp::getInstance()
{
	if (!instance)
	{
		instance = new MYQapp();
	}
	return instance;
}

void MYQapp::processEvents()
{
	app->processEvents();
}

ImageGrab::ImageGrab(QString panoID, int x, int y) :QObject(), m_x(x), m_y(y), m_dataReady(false){
	MYQapp::getInstance();
	//page = new QWebPage();
	QString url = "http://cbk0.google.com/cbk?output=tile&panoid=" + panoID + "&zoom=5&x=" + QString::number(m_x) + "&y=" + QString::number(m_y);
	QNetworkRequest request(url);
	manager = new QNetworkAccessManager();
	connect(manager, SIGNAL(finished(QNetworkReply*)),
		this, SLOT(replyFinished(QNetworkReply*)));
	manager->get(request);

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
	QFile file("tile" + QString::number(m_x) + "_" + QString::number(m_y) + ".jpg");
	file.open(QIODevice::WriteOnly);
	file.write(reply->readAll());

	emit finished();
	m_dataReady = true;
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
	return m_dataReady;
}

PanoIDGrabber::PanoIDGrabber(double lat, double lng) :QObject(), m_lng(lng), m_lat(lat), m_dataReady(false){
	MYQapp::getInstance();
	page = new QWebPage();
	connect(page->mainFrame(), SIGNAL(titleChanged(const QString&)), this, SLOT(titleChanged(const QString&)));
	page->mainFrame()->load(QUrl("file:///C:/Workspace/projects/cavetest/build/bin/Debug/getPanoID.html?lat=" + QString::number(m_lat, 'f', 7) + "&lng=" + QString::number(m_lng, 'f', 7)));
}

PanoIDGrabber::~PanoIDGrabber()
{
	page->settings()->clearMemoryCaches();
	delete page;
}

void PanoIDGrabber::titleChanged(const QString& title)
{
	QWebElement el = page->mainFrame()->findFirstElement("#panoid");
	if (page->mainFrame()->title() == "Done"){
		m_panoID = page->mainFrame()->findFirstElement("#panoid").toPlainText();
		emit finished();
		m_dataReady = true;
	}
}

QString PanoIDGrabber::getPanoID()
{
	return m_panoID;
}

bool PanoIDGrabber::hasDataReady()
{
	return m_dataReady;
}