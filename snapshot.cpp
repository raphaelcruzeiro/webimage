#include <QApplication>
#include "snapshot.h"

Snapshot::Snapshot(QObject *parent) : QObject(parent), page(new QWebPage)
{
}

void Snapshot::shot(QUrl url, QSize &size, QString *outputFilename)
{
    this->outputFilename = outputFilename;
    connect(page, SIGNAL(loadFinished(bool)), SLOT(doneLoading(bool)));
    page->mainFrame()->load(url);
    page->setViewportSize(size);
    page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
}

void Snapshot::doneLoading(bool)
{
    QImage image(page->viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);

    page->mainFrame()->render(&painter);

    painter.end();

    image.save(*outputFilename);

    delete outputFilename;
    QApplication::quit();
}

