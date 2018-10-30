#include "settingsTreeModel.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QFile>

SettingsTreeModel::SettingsTreeModel(const QString &settingsFile, QObject *parent)
    : QAbstractItemModel(parent)
{
    _role[NameRole] = "NameRole";
    _role[DescriptionRole] = "DescriptionRole";
    _role[ValRole] = "ValRole";
    _role[MaxValRole] = "MaxValRole";
    _role[MinValRole] = "MinValRole";
    _role[StepSizeRole] = "StepSizeRole";
    _role[DefValRole] = "DefValRole";
    _role[ValEnumRole] = "ValEnumRole";
    _role[TypeRole] = "TypeRole";

    rootItem = new SettingsTreeItem("Settings");

    if(!settingsFile.isEmpty()) _settingsFile = settingsFile;
    else _settingsFile = DEFAULT_SETTINGS_FILE;

    setupModelData(_settingsFile, rootItem);
}

SettingsTreeModel::~SettingsTreeModel()
{
    delete rootItem;
}

int SettingsTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return 1;
}

QVariant SettingsTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    SettingsTreeItem *item = getItem(index);

    switch (role) {
    case NameRole:          return item->getName();
    case DescriptionRole:   return item->getDescription();
    case ValRole:           return item->getValue();
    case MinValRole:        return item->getMinValue();
    case MaxValRole:        return item->getMaxValue();
    case StepSizeRole:      return item->getStepSize();
    case DefValRole:        return item->getDefaultVal();
    case ValEnumRole:       return item->getValueEnum();
    case TypeRole:          return item->getWidgetType();
    }

    return QVariant();
}

SettingsTreeItem *SettingsTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        SettingsTreeItem *item = static_cast<SettingsTreeItem*>(index.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

QModelIndex SettingsTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    SettingsTreeItem *parentItem = getItem(parent);

    SettingsTreeItem *childItem = parentItem->child(row);
    if (childItem) return createIndex(row, column, childItem);
    else return QModelIndex();
}

QModelIndex SettingsTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    SettingsTreeItem *childItem = getItem(index);
    SettingsTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem) return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int SettingsTreeModel::rowCount(const QModelIndex &parent) const
{
    SettingsTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool SettingsTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    SettingsTreeItem *item = getItem(index);

    switch (role) {
    case NameRole:          return false;
    case DescriptionRole:   item->setDescription(value.toString()); break;
    case ValRole:           item->setValue(value); break;
    case MinValRole:        item->setMinValue(value); break;
    case MaxValRole:        item->setMaxValue(value); break;
    case StepSizeRole:      item->setStepSize(value); break;
    case DefValRole:        item->setDefaultVal(value); break;
    case ValEnumRole:       item->setValueEnum(value); break;
    case TypeRole:          item->setWidgetType(value.toInt()); break;
    default:                return false;
    }

    emit dataChanged(index, index);
    return true;
}

void SettingsTreeModel::setupModelData(const QString &settingsFile, SettingsTreeItem *parent)
{
    QFile jsonFile(settingsFile);

    if (jsonFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        QJsonParseError _err;
        QJsonDocument jsonData(QJsonDocument::fromJson(jsonFile.readAll(), &_err));
        if (jsonData.isNull() || jsonData.isEmpty()){
            qDebug() << "Error: " << _err.errorString() << _err.offset;
        }

        setupModelData(jsonData.object(), parent);
    }
}

void SettingsTreeModel::setupModelData(const QJsonObject &data, SettingsTreeItem *parent)
{
    if(data.contains("description")) parent->setDescription(data["description"].toString());
    parent->setWidgetType( data.contains("widget") ? data["widget"].toString().toInt() : 0);
    if(data.contains("value")) parent->setValue(data["value"].toVariant());
    if(data.contains("min")) parent->setMinValue(data["min"].toVariant());
    if(data.contains("max")) parent->setMaxValue(data["max"].toVariant());
    if(data.contains("default")) parent->setDefaultVal(data["default"].toVariant());
    if(data.contains("step")) parent->setStepSize(data["step"].toVariant());
    if(!data.contains("settings") || !data["settings"].isArray()) return;

    QJsonArray arr = data["settings"].toArray();
    for (int i = 0; i < arr.size(); i++){
        QString setting = arr[i].toString();
        if(data.contains(setting)) {
            parent->insertChildren(setting);
            setupModelData(data[setting].toObject(), parent->child(i));
        }
    }
}

void SettingsTreeModel::saveSettingsFile(bool resetSettings, const QString &filename)
{

}

QHash<int, QByteArray> SettingsTreeModel::roleNames() const
{
    return _role;
}
