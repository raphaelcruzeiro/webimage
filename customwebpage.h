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

#ifndef CUSTOMWEBPAGE_H
#define CUSTOMWEBPAGE_H

#include <QtCore>
#include <QtWebKit>
#include <QWebPage>

class CustomWebPage : public QWebPage
{
public:
    CustomWebPage(QObject *parent = 0);
    QString userAgentForUrl(const QUrl &url) const;

protected:
    void javaScriptAlert(QWebFrame *originatingFrame, const QString &msg);
    bool javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg);

public slots:
    bool shouldInterruptJavaScript();
};

#endif // CUSTOMWEBPAGE_H
