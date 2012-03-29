#ifndef SNAPSHOT_H
#define SNAPSHOT_H
#include <QtCore>
#include <QtWebKit>
#include <QImage>
#include <QPainter>

class Snapshot : QObject
{
    Q_OBJECT

    QWebPage *page;
    QString *outputFilename;

public:
    Snapshot(QObject *parent = 0);
    void shot(QUrl url, QSize &size, QString *outputFilename = new QString("output.png"));

private slots:
    void doneLoading(bool);
};

#endif // SNAPSHOT_H
