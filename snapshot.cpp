//    Webimage - Website to image render tool
//    Copyright (C) 2012  Raphael Cruzeiro

//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
    this->size = QSize(1024, 6000);
    this->useSystemUI = useSystemUI;

    if(useSystemUI) {
        qDebug() << "Loading UI...";
        view = new QWebView;
        view->setPage(page);
        view->setMinimumSize(1024, 5000);
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

            image.save(*outputFilename, "JPEG");
        } else {
            view->setMaximumHeight(page->mainFrame()->contentsSize().height());
            view->repaint();
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
