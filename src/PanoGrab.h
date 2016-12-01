#ifndef PANOGRAB_H_
#define PANOGRAB_H_

#include <QApplication>
#include <QtWebKit/QtWebKit>

class PanoGrab : QObject
{
	Q_OBJECT

public:
	PanoGrab(double lat, double lng);
	virtual ~PanoGrab();

	bool hasDataReady();
	QString getPanoID();

private:
	QWebPage* page;

	double m_lat;
	double m_lng;
	bool m_dataReady;
	QString m_panoID;

private slots:
	void titleChanged(const QString& title);
	

signals:
	void finished();

};

#endif