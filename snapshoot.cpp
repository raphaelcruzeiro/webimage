#include <QApplication>
#include "snapshoot.h"

Snapshoot::Snapshoot(QObject *parent) : QObject(parent), page(new QWebPage)
{
}

void Snapshoot::shoot(QUrl url, QSize &size, QString *outputFilename)
{
    this->outputFilename = outputFilename;
    connect(page, SIGNAL(loadFinished(bool)), SLOT(doneLoading(bool)));
    page->mainFrame()->load(url);
    page->setViewportSize(size);
    page->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
}

void Snapshoot::doneLoading(bool)
{
    QImage image(page->viewportSize(), QImage::Format_ARGB32);
    QPainter painter(&image);

    page->mainFrame()->render(&painter);

    painter.end();

    image.save(*outputFilename);

    delete outputFilename;
    QApplication::quit();
}

