#include <QApplication>
#include "snapshot.h"
#include "customwebpage.h"

Snapshot::Snapshot(QObject *parent) : QObject(parent), page(new CustomWebPage), statusCode(0), tries(0)
{
}

void Snapshot::shot(QUrl url, QSize &size, QString *outputFilename, QSize scaleTo, bool ignoreVerticalLimit, bool useSystemUI)
{
    this->scaleTo = scaleTo;
    this->ignoreVerticalLimit = ignoreVerticalLimit;
    this->size = QSize(1024, 3000);
    this->useSystemUI = useSystemUI;

    if(useSystemUI) {
        qDebug() << "Loading UI...";
        view = new QWebView;
        view->setPage(page);
        view->setMinimumSize(1024, 3000);
    }

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

        if(!useSystemUI) {
            QImage image(page->viewportSize(), QImage::Format_ARGB32);
            QPainter painter(&image);

            page->mainFrame()->render(&painter);
            painter.end();

            if(this->scaleTo != QSize(0, 0)) {
                qDebug() << "Scaling to " << scaleTo.width() << "x" << scaleTo.height();
                image = image.scaled(scaleTo);
            }

            image.save(*outputFilename);
        } else {
            QPixmap pix = QPixmap::grabWidget(view, 0, 0, size.width(), page->mainFrame()->contentsSize().height());
            pix.save(*outputFilename);
        }

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

    if(reply->header(QNetworkRequest::ContentTypeHeader).toString().contains(QString("text/html")) && statusCode != 200) {
        statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        redirectUrl = QUrl(reply->header(QNetworkRequest::LocationHeader).toUrl());
    }
}

void Snapshot::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    reply->ignoreSslErrors();
}
