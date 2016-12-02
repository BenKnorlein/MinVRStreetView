#ifndef IMAGEGRAB_H_
#define IMAGEGRAB_H_

#include <QtWebKit/QtWebKit>

class ImageGrab : QObject
{
	Q_OBJECT

public:
	ImageGrab(QString panoID);
	virtual ~ImageGrab();

	bool hasDataReady();

private:
	QWebPage* page;
	QImage* thumbnail;

	QNetworkAccessManager* manager;

	int m_nbDownloads;

private slots:
	void saveScreenshot();
	void replyFinished(QNetworkReply* reply);

signals:
	void finished();

};

#endif