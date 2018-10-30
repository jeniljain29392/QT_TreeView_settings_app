#include "settingsTreeItem.h"

SettingsTreeItem::SettingsTreeItem(const QString &name, SettingsTreeItem *parent)
{
    parentItem = parent;
    _name = name;
}

SettingsTreeItem::~SettingsTreeItem()
{
    qDeleteAll(childItems);
}

SettingsTreeItem *SettingsTreeItem::child(int number)
{
    return childItems.value(number);
}

int SettingsTreeItem::childCount() const
{
    return childItems.count();
}

int SettingsTreeItem::childNumber() const
{
    return parentItem ? parentItem->childItems.indexOf(const_cast<SettingsTreeItem*>(this)) : 0;
}

void SettingsTreeItem::insertChildren(const QString &name)
{
    SettingsTreeItem *item = new SettingsTreeItem(name, this);
    childItems.append(item);
}

SettingsTreeItem *SettingsTreeItem::parent()
{
    return parentItem;
}

void SettingsTreeItem::setDescription(QString description)
{
    _description = description;
}

void SettingsTreeItem::setMinValue(QVariant minVal)
{
    _minValue = minVal;
}

void SettingsTreeItem::setMaxValue(QVariant maxVal)
{
    _maxValue = maxVal;
}

void SettingsTreeItem::setStepSize(QVariant stepSize)
{
    _step = stepSize;
}

void SettingsTreeItem::setValue(QVariant value)
{
    _value = value;
}

void SettingsTreeItem::setDefaultVal(QVariant defVal)
{
    _defValue = defVal;
}

void SettingsTreeItem::setValueEnum(QVariant valEnum)
{
    _valEnum = valEnum;
}

void SettingsTreeItem::setWidgetType(int type)
{
    _type = static_cast<WidgetType>(type);
}

QString SettingsTreeItem::getDescription() const
{
    return _description;
}

QString SettingsTreeItem::getName() const
{
    return _name;
}

QVariant SettingsTreeItem::getMinValue() const
{
    return _minValue;
}

QVariant SettingsTreeItem::getMaxValue() const
{
    return _maxValue;
}

QVariant SettingsTreeItem::getValue() const
{
    return _value;
}

QVariant SettingsTreeItem::getDefaultVal() const
{
    return _defValue;
}

QVariant SettingsTreeItem::getStepSize() const
{
    return  _step;
}

QVariant SettingsTreeItem::getWidgetType() const
{
    return _type;
}

QVariant SettingsTreeItem::getValueEnum() const
{
    return _valEnum;
}
