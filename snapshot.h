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
    QWebView *view;
    QSize size;

    int tries, quality;

public:
    Snapshot(QObject *parent = 0);
    void shot(QUrl url, QSize &size, QString *outputFilename = new QString("output.png"), int quality = -1);

private slots:
    void doneLoading(bool);
    void doneWaiting();
    void gotReply(QNetworkReply *reply);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
};

#endif // SNAPSHOT_H
