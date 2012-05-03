#ifndef SNAPSHOT_H
#define SNAPSHOT_H
#include <QtCore>
#include <QtWebKit>
#include <QImage>
#include <QPainter>
#include<QTimer>

class Snapshot : QObject
{
    Q_OBJECT

    QWebPage *page;
    QString *outputFilename;
    QTimer *timer;
    int statusCode;
    QUrl redirectUrl;

    int tries;

public:
    Snapshot(QObject *parent = 0);
    void shot(QUrl url, QSize &size, QString *outputFilename = new QString("output.png"));

private slots:
    void doneLoading(bool);
    void doneWaiting();
    void gotReply(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // SNAPSHOT_H
