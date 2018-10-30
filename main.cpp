#include "settingsTreeModel.h"

#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    SettingsTreeModel *model = new SettingsTreeModel("", &engine);

    engine.rootContext()->setContextProperty("SettingsTreeModel", model);
    engine.rootContext()->setContextProperty("ModelRootIndex", model->parent(QModelIndex()));

    qmlRegisterUncreatableMetaObject(SettingsTreeModel::staticMetaObject, "SettingsTreeModel", 1, 0, "SettingsTreeRoles", "Error: only enums");
    qmlRegisterUncreatableMetaObject(SettingsTreeItem::staticMetaObject, "SettingsTreeItem", 1, 0, "WidgetType", "Error: only enums");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
