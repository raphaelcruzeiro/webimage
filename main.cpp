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
#include <iostream>

int main(int argc, char *argv[])
{
    if(argc > 0) {
        QApplication a(argc, argv);

        QUrl url;
        QString output;
        int minWidth = 1024;

        if(argc < 2) {
            std::cout << "Usage: webimage <url> <output> <optional: minimum width>" << std::endl;
            return -1;
        }

        for(int i = 0; i < argc; i++) {
            switch (i) {
            case 1:
                url = QUrl(QString(argv[1]));
                break;

            case 2:
                output = QString(argv[2]);
                break;

            case 3:
                minWidth = QString(argv[3]).toInt();
                break;
            }
        }

        Snapshot shot;
        shot.shot(url, minWidth, &output);

        return a.exec();
    }
}
