#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include "core/packageparser.h"

// Информация об установленном пакете
struct InstalledPackage {
    QString id;              // Уникальный идентификатор
    QString name;            // Название пакета
    QString version;         // Версия
    QString installPath;     // Путь установки
    QString configPath;      // Путь к конфигурации
    QString keywordsPath;    // Путь к файлу ключевых слов
    QString compilerPath;    // Путь к компилятору (если есть)
    QString libPath;         // Путь к библиотекам
    QString includePath;     // Путь к заголовкам
    QString helpPath;        // Путь к документации
    bool isActive;           // Активен ли пакет
    QDateTime installDate;   // Дата установки
};

class PackageManager : public QObject
{
    Q_OBJECT
public:
    explicit PackageManager(QObject *parent = nullptr);
    
    // Инициализация менеджера пакетов
    bool initialize(const QString &packagesDir);
    
    // Установка пакета из архива
    bool installPackage(const QString &archivePath, QString &error);
    
    // Удаление пакета по ID
    bool uninstallPackage(const QString &packageId, QString &error);
    
    // Получение списка установленных пакетов
    QList<InstalledPackage> getInstalledPackages() const;
    
    // Получение информации о пакете
    InstalledPackage getPackageInfo(const QString &packageId) const;
    
    // Проверка наличия пакета
    bool isPackageInstalled(const QString &packageId) const;
    
    // Активация/деактивация пакета
    bool setPackageActive(const QString &packageId, bool active);
    
    // Получение путей для всех активных пакетов
    QStringList getActiveIncludePaths() const;
    QStringList getActiveLibPaths() const;
    QStringList getActiveCompilerPaths() const;
    
    // Экспорт информации о пакетах в конфиг редактора
    bool exportToConfig(const QString &configPath);

signals:
    void packageInstalled(const QString &packageId);
    void packageUninstalled(const QString &packageId);
    void installationProgress(int percent, const QString &message);
    void errorOccurred(const QString &error);

private:
    QString generatePackageId(const QString &name, const QString &version);
    bool loadPackagesIndex();
    bool savePackagesIndex();
    bool removeDirectoryRecursively(const QString &path);
    
    QString m_packagesDir;
    QString m_indexFile;
    QMap<QString, InstalledPackage> m_packages;
    PackageParser m_parser;
};

#endif // PACKAGEMANAGER_H
