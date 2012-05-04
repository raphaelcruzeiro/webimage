#ifndef SNAPSHOT_H
#define SNAPSHOT_H
#include <QtCore>
#include <QtWebKit>
#include <QImage>
#include <QPainter>
#include<QTimer>

class CustomWebPage;

class Snapshot : QObject
{
    Q_OBJECT

    CustomWebPage *page;
    QString *outputFilename;
    QTimer *timer;
    int statusCode;
    QUrl redirectUrl;
    QSize scaleTo, size;
    bool ignoreVerticalLimit, useSystemUI;
    QWebView *view;

    int tries;

public:
    Snapshot(QObject *parent = 0);
    void shot(QUrl url, QSize &size, QString *outputFilename = new QString("output.png"),
              QSize scaleTo = QSize(0, 0), bool ignoreVerticalLimit = false, bool useSystemUI = false);

private slots:
    void doneLoading(bool);
    void doneWaiting();
    void gotReply(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // SNAPSHOT_H
