#ifndef SNAPSHOOT_H
#define SNAPSHOOT_H
#include <QtCore>
#include <QtWebKit>
#include <QImage>
#include <QPainter>

class Snapshoot : QObject
{
    Q_OBJECT

    QWebPage *page;
    QString *outputFilename;

public:
    Snapshoot(QObject *parent = 0);
    void shoot(QUrl url, QSize &size, QString *outputFilename = new QString("output.png"));

private slots:
    void doneLoading(bool);
};

#endif // SNAPSHOOT_H
