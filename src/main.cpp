#ifdef QT_QML_DEBUG
#include <QtQuick>
#endif

#include <QScopedPointer>
#include <QQuickView>

#include <sailfishapp.h>

#include "datawriter.h"
#include "facilities.h"

int main(int argc, char *argv[])
{
    QScopedPointer< QGuiApplication > app(SailfishApp::application(argc, argv));
    app->setApplicationName("harbour-cyclingcomputer");

    QScopedPointer< DataWriter > dataWriter(new DataWriter());

    QScopedPointer< GpsFacility > gpsFacility(new GpsFacility());
    gpsFacility.reset();

    QScopedPointer< QQuickView > view(SailfishApp::createView());

    view->rootContext()->setContextProperty("dataWriter", dataWriter.data());

    view->setSource(SailfishApp::pathTo("qml/Main.qml"));
    view->show();

    return app->exec();
}

