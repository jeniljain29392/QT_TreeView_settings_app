#ifndef SETTINGSTREEITEM_H
#define SETTINGSTREEITEM_H

#include <QVariant>
#include <QVector>

class SettingsTreeItem : public QObject
{
    Q_OBJECT
public:
    enum WidgetType{
        Slider = 1,
        Spinner,
        CheckBox,
        RadioBtnGroup
    };
    Q_ENUMS(WidgetType)

    SettingsTreeItem(const QString &name, SettingsTreeItem *parent = nullptr);
    ~SettingsTreeItem();

    SettingsTreeItem *child(int number);
    int childCount() const;
    void insertChildren(const QString &name);
    SettingsTreeItem *parent();
    int childNumber() const;

    void setDescription(QString description);
    void setMinValue(QVariant minVal);
    void setMaxValue(QVariant maxVal);
    void setStepSize(QVariant stepSize);
    void setValue(QVariant value);
    void setDefaultVal(QVariant defVal);
    void setValueEnum(QVariant valEnum);
    void setWidgetType(int type);

    QString getDescription() const;
    QString getName() const;
    QVariant getMinValue() const;
    QVariant getMaxValue() const;
    QVariant getValue() const;
    QVariant getDefaultVal() const;
    QVariant getStepSize() const;
    QVariant getWidgetType() const;
    QVariant getValueEnum() const;

private:
    QList<SettingsTreeItem*> childItems;
    QVector<QVariant> itemData;
    SettingsTreeItem *parentItem;

    QString _description, _name;
    QVariant _minValue, _maxValue, _step;
    QVariant _value, _defValue, _valEnum;
    WidgetType _type;
};

#endif // SETTINGSTREEITEM_H
