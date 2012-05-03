#ifndef CUSTOMWEBPAGE_H
#define CUSTOMWEBPAGE_H

#include <QtCore>
#include <QtWebKit>
#include <QWebPage>

class CustomWebPage : public QWebPage
{
public:
    CustomWebPage(QObject *parent = 0);

protected:
    void javaScriptAlert(QWebFrame *originatingFrame, const QString &msg);
    bool javaScriptConfirm(QWebFrame *originatingFrame, const QString &msg);

public slots:
    bool shouldInterruptJavaScript();
};

#endif // CUSTOMWEBPAGE_H
