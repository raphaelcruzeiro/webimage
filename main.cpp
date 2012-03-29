#include <QApplication>
#include "snapshot.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    if(argc > 0) {
        QApplication a(argc, argv);

        QSize size;

        if(argc > 1) {
            size = QSize(
                        QString(argv[2]).split(QChar('x'))[0].toInt(),
                        QString(argv[2]).split(QChar('x'))[1].toInt()
            );
        } else {
            size = QSize(1024, 768);
        }

        Snapshot shot;
        if (argc > 2) shot.shot(QUrl(QString(argv[1])), size, new QString(argv[3])); else shot.shot(QUrl(QString(argv[1])), size);

        return a.exec();
    }
}
