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

#include <QDebug>

int main(int argc, char *argv[])
{
    if(argc > 0) {
        QApplication a(argc, argv);

        QUrl url;
        QString output;
        QSize size(1024, 768);
        QSize scaleTo(0, 0);
        bool ignoreVerticalLimit = false;
        bool useSystemUI = false;

        for(int i = 0; i < argc; i++) {
            if(strcmp(argv[i], "ignore-vertical-limit") == 0) {
                ignoreVerticalLimit = true;
                continue;
            } else if (strcmp(argv[i], "use-system-ui") == 0) {
                useSystemUI = true;
                continue;
            }

            switch (i) {
            case 1:
                url = QUrl(QString(argv[1]));
                break;

            case 2:
                output = QString(argv[2]);
                break;

            case 3:
                size = QSize(
                            QString(argv[3]).split(QChar('x'))[0].toInt(),
                            QString(argv[3]).split(QChar('x'))[1].toInt()
                );
                break;

            case 4:
                scaleTo = QSize(
                            QString(argv[4]).split(QChar('x'))[0].toInt(),
                            QString(argv[4]).split(QChar('x'))[1].toInt()
                );
                break;
            }
        }

        Snapshot shot;
        shot.shot(url, size, &output, scaleTo, ignoreVerticalLimit, useSystemUI);

        return a.exec();
    }
}
