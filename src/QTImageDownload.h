#ifndef QTAPPLOOP_H_
#define QTAPPLOOP_H_

#include <QApplication>
#include <QtWebKit/QtWebKit>

class MYQapp
{
		static MYQapp* instance;
	protected:
		MYQapp();
	public:
		virtual ~MYQapp();
		static MYQapp* getInstance();

		void processEvents();
	private:
		QApplication* app;
};

class ImageGrab : QObject
{
	Q_OBJECT

public:
	ImageGrab(QString panoID, int x, int y);
	virtual ~ImageGrab();

	bool hasDataReady();

private:
	QWebPage* page;
	QImage* thumbnail;

	QNetworkAccessManager* manager;

	int m_x;
	int m_y;
	bool m_dataReady; 

private slots:
	void saveScreenshot();
	void replyFinished(QNetworkReply* reply);

signals:
	void finished();

};

class PanoIDGrabber : QObject
{
	Q_OBJECT

public:
	PanoIDGrabber(double lat, double lng);
	virtual ~PanoIDGrabber();

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