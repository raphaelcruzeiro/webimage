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
