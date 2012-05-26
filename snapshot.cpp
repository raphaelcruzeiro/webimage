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

void Snapshot::shot(QUrl url, int minWidth, QString *outputFilename, int quality)
{
    this->minWidth = minWidth;
    this->quality = quality;

    QSize size(minWidth, 768);

    qDebug() << "Loading fake UI...";
    view = new QWebView;
    view->setPage(page);
    QSize newSize = size;
    newSize.setHeight(7000);
    view->setMinimumSize(newSize);

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

        if(minWidth < page->mainFrame()->contentsSize().width()) {
            minWidth = page->mainFrame()->contentsSize().width();
            view->setMinimumWidth(page->mainFrame()->contentsSize().width());
        }

        view->setMinimumHeight(page->mainFrame()->contentsSize().height());
        view->repaint();
        QPixmap pix = QPixmap::grabWidget(view, 0, 0, minWidth, page->mainFrame()->contentsSize().height());
        pix.save(*outputFilename, "JPEG", quality);


        QString thumbFilename = QString("%1_thumb.jpg").arg(outputFilename->split('.')[0]);
        QSize thumbSize((minWidth / 100) * 50, (page->mainFrame()->contentsSize().height() / 100) * 50);
        pix =pix.scaled(thumbSize, Qt::KeepAspectRatio);
        pix.save(thumbFilename, "JPEG", quality);

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
