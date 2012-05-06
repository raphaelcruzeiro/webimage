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

#include "customwebpage.h"

CustomWebPage::CustomWebPage(QObject *parent) : QWebPage(parent) { }

QString CustomWebPage::userAgentForUrl(const QUrl &url) const
{
    return QString("Webimage - Snapshot tool. https://github.com/raphaelcruzeiro/webimage");
}

void CustomWebPage::javaScriptAlert(QWebFrame *originatingFrame, const QString &msg)
{
}

bool CustomWebPage::javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg)
{
    return false;
}

bool CustomWebPage::shouldInterruptJavaScript()
{
    return true;
}
