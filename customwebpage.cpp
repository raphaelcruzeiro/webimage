#include "customwebpage.h"

CustomWebPage::CustomWebPage(QObject *parent) : QWebPage(parent) { }

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
