#ifndef SETTINGSTREEMODEL_H
#define SETTINGSTREEMODEL_H

#include "settingsTreeItem.h"

#include <QAbstractItemModel>

#define DEFAULT_SETTINGS_FILE  ":/settings.json"

class SettingsTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum SettingsTreeRoles {
        NameRole = Qt::DisplayRole,
        DescriptionRole = Qt::UserRole + 1,
        ValRole,
        MinValRole,
        MaxValRole,
        StepSizeRole,
        DefValRole,
        ValEnumRole,
        TypeRole
    };
    Q_ENUMS(SettingsTreeRoles)

    Q_PROPERTY(QString settingsFile MEMBER _settingsFile NOTIFY settingsFileChanged)

    explicit SettingsTreeModel(const QString &settingsFile = "", QObject *parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QHash<int, QByteArray> roleNames() const override;

    ~SettingsTreeModel() override;

    Q_INVOKABLE void saveSettingsFile(bool resetSettings = false, const QString &filename = "");

signals:
    void settingsFileChanged();

public slots:
private:
    void setupModelData(const QString &settingsFile, SettingsTreeItem *parent);
    void setupModelData(const QJsonObject &data, SettingsTreeItem *parent);
    SettingsTreeItem *getItem(const QModelIndex &index) const;
    QHash<int, QByteArray> _role;

    SettingsTreeItem *rootItem;
    QString _settingsFile;
};

#endif // SETTINGSTREEMODEL_H
