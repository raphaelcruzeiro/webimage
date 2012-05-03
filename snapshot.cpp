#include <QApplication>
#include "snapshot.h"

Snapshot::Snapshot(QObject *parent) : QObject(parent), page(new QWebPage), statusCode(0), tries(0)
{
}

void Snapshot::shot(QUrl url, QSize &size, QString *outputFilename)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(doneWaiting()));

    this->outputFilename = outputFilename;
    connect(page->networkAccessManager(), SIGNAL(finished(QNetworkReply*)), SLOT(gotReply(QNetworkReply*)));
    connect(page, SIGNAL(loadFinished(bool)), SLOT(doneLoading(bool)));
    connect(page->networkAccessManager(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));
    page->mainFrame()->load(url);
    page->setViewportSize(size);
    page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
}

void Snapshot::doneLoading(bool)
{
    // A reasonable waiting time for any script to execute
    timer->start(3000);
}

void Snapshot::doneWaiting()
{
    if( statusCode != 0 &&
        statusCode != 301 &&
        statusCode != 302 &&
        statusCode != 303
       ) {
        QImage image(page->viewportSize(), QImage::Format_ARGB32);
        QPainter painter(&image);

        page->mainFrame()->render(&painter);

        painter.end();

        image.save(*outputFilename);

        delete outputFilename;
        QApplication::quit();
    }
    else if(statusCode != 0) {
        statusCode = 0;
        qDebug() << "Redirecting to: " + redirectUrl.toString();
        if(page->mainFrame()->url().toString().isEmpty()) {
            qDebug() << "about:blank";
            page->mainFrame()->load(this->redirectUrl);
            qDebug() << "Loading";
        }
    }

    // This should ensure that the program never hangs
    if(statusCode == 0) {
        if(tries > 5) {
            qDebug() << "Giving up.";
            QApplication::quit();
        }
        tries++;
    }
}

void Snapshot::gotReply(QNetworkReply *reply)
{
    if(reply->header(QNetworkRequest::ContentTypeHeader).toString().contains(QString("text/html")))
    {
        qDebug() << "Got reply " + reply->url().toString() + " - " + reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString() + " - " + reply->header(QNetworkRequest::ContentTypeHeader).toString();
    }

    if(reply->header(QNetworkRequest::ContentTypeHeader).toString().contains(QString("text/html"))) {
        statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        redirectUrl = QUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
    }
}

void Snapshot::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors();
}
