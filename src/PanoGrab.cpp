#include "PanoGrab.h"
#include "MYQApp.h"
#include <iostream>

PanoGrab::PanoGrab(double lat, double lng) :QObject(), m_lng(lng), m_lat(lat), m_dataReady(false){
	MYQapp::getInstance();
	page = new QWebPage();
	connect(page->mainFrame(), SIGNAL(titleChanged(const QString&)), this, SLOT(titleChanged(const QString&)));
	//make sure to change this directory to your own
	page->mainFrame()->load(QUrl("/users/hsha/google-streetview/MinVRStreetView/src/getPanoID.html?lat=" + QString::number(m_lat, 'f', 7) + "&lng=" + QString::number(m_lng, 'f', 7)));
}

PanoGrab::~PanoGrab()
{
	page->settings()->clearMemoryCaches();
	delete page;
}

void PanoGrab::titleChanged(const QString& title)
{
	QWebElement el = page->mainFrame()->findFirstElement("#panoid");
	if (page->mainFrame()->title() == "Done"){
		m_panoID = page->mainFrame()->findFirstElement("#panoid").toPlainText();
		emit finished();
		m_dataReady = true;
	}
}

QString PanoGrab::getPanoID()
{
	return m_panoID;
}

bool PanoGrab::hasDataReady()
{
	return m_dataReady;
}
