#include "workoutchart.hpp"
#include <QQuickView>
#include <QQmlEngine>
#include <QGuiApplication>
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickView m_view {};
    m_view.setMinimumHeight(800);
    m_view.setMinimumWidth(900);
    QObject::connect(m_view.engine(), &QQmlEngine::quit, &app, &QGuiApplication::quit, Qt::QueuedConnection);
    m_view.setSource(QUrl("qrc:/ui/main.ui.qml"));
    m_view.show();
    return app.exec();
}