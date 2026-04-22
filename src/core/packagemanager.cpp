#include "packagemanager.h"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QUuid>
#include <QDebug>

PackageManager::PackageManager(QObject *parent) : QObject(parent)
{
}

bool PackageManager::initialize(const QString &packagesDir)
{
    m_packagesDir = packagesDir;
    m_indexFile = QDir(m_packagesDir).absoluteFilePath("packages.xml");
    
    // Создаем директорию пакетов, если не существует
    QDir dir(m_packagesDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            emit errorOccurred(tr("Не удалось создать директорию пакетов: %1").arg(packagesDir));
            return false;
        }
    }
    
    // Загружаем индекс установленных пакетов
    return loadPackagesIndex();
}

QString PackageManager::generatePackageId(const QString &name, const QString &version)
{
    // Генерируем уникальный ID на основе имени и версии
    QString base = name.toLower().replace(" ", "_").replace("-", "_");
    return QString("%1_%2").arg(base, version.replace(".", "_"));
}

bool PackageManager::loadPackagesIndex()
{
    m_packages.clear();
    
    if (!QFile::exists(m_indexFile)) {
        return true; // Индекс еще не создан
    }
    
    QFile file(m_indexFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        emit errorOccurred(tr("Не удалось открыть индекс пакетов"));
        return false;
    }
    
    QXmlStreamReader reader(&file);
    InstalledPackage currentPkg;
    bool inPackage = false;
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == "package") {
                inPackage = true;
                currentPkg = InstalledPackage();
                currentPkg.id = reader.attributes().value("id").toString();
            } else if (inPackage) {
                QString value = reader.readElementText();
                if (reader.name() == "name") currentPkg.name = value;
                else if (reader.name() == "version") currentPkg.version = value;
                else if (reader.name() == "installPath") currentPkg.installPath = value;
                else if (reader.name() == "configPath") currentPkg.configPath = value;
                else if (reader.name() == "keywordsPath") currentPkg.keywordsPath = value;
                else if (reader.name() == "compilerPath") currentPkg.compilerPath = value;
                else if (reader.name() == "libPath") currentPkg.libPath = value;
                else if (reader.name() == "includePath") currentPkg.includePath = value;
                else if (reader.name() == "helpPath") currentPkg.helpPath = value;
                else if (reader.name() == "isActive") currentPkg.isActive = (value == "true");
                else if (reader.name() == "installDate") currentPkg.installDate = QDateTime::fromString(value, Qt::ISODate);
            }
        } else if (reader.isEndElement() && reader.name() == "package") {
            inPackage = false;
            if (!currentPkg.id.isEmpty()) {
                m_packages[currentPkg.id] = currentPkg;
            }
        }
    }
    
    file.close();
    
    if (reader.hasError()) {
        emit errorOccurred(tr("Ошибка чтения индекса пакетов: %1").arg(reader.errorString()));
        return false;
    }
    
    return true;
}

bool PackageManager::savePackagesIndex()
{
    QFile file(m_indexFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        emit errorOccurred(tr("Не удалось сохранить индекс пакетов"));
        return false;
    }
    
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    writer.writeStartElement("packages");
    
    for (const InstalledPackage &pkg : m_packages) {
        writer.writeStartElement("package");
        writer.writeAttribute("id", pkg.id);
        
        writer.writeTextElement("name", pkg.name);
        writer.writeTextElement("version", pkg.version);
        writer.writeTextElement("installPath", pkg.installPath);
        writer.writeTextElement("configPath", pkg.configPath);
        writer.writeTextElement("keywordsPath", pkg.keywordsPath);
        writer.writeTextElement("compilerPath", pkg.compilerPath);
        writer.writeTextElement("libPath", pkg.libPath);
        writer.writeTextElement("includePath", pkg.includePath);
        writer.writeTextElement("helpPath", pkg.helpPath);
        writer.writeTextElement("isActive", pkg.isActive ? "true" : "false");
        writer.writeTextElement("installDate", pkg.installDate.toString(Qt::ISODate));
        
        writer.writeEndElement(); // package
    }
    
    writer.writeEndElement(); // packages
    writer.writeEndDocument();
    
    file.close();
    return true;
}

bool PackageManager::installPackage(const QString &archivePath, QString &error)
{
    error.clear();
    
    // Проверяем архив перед установкой
    PackageContent content;
    if (!m_parser.parseArchive(archivePath, content)) {
        error = content.errorMessage;
        return false;
    }
    
    // Генерируем ID пакета
    QString packageId = generatePackageId(content.packageName, content.packageVersion);
    
    // Проверяем, не установлен ли уже пакет
    if (m_packages.contains(packageId)) {
        error = tr("Пакет %1 версии %2 уже установлен").arg(content.packageName, content.packageVersion);
        return false;
    }
    
    // Создаем директорию для установки
    QString installDir = QDir(m_packagesDir).absoluteFilePath(packageId);
    QDir dir(installDir);
    if (dir.exists()) {
        error = tr("Директория установки уже существует");
        return false;
    }
    
    emit installationProgress(0, tr("Распаковка пакета..."));
    
    // Распаковываем пакет
    PackageContent installedContent;
    if (!m_parser.extractTo(archivePath, installDir, installedContent)) {
        error = installedContent.errorMessage;
        // Откат: удаляем частично распакованную директорию
        removeDirectoryRecursively(installDir);
        return false;
    }
    
    emit installationProgress(100, tr("Установка завершена"));
    
    // Создаем запись об установленном пакете
    InstalledPackage pkg;
    pkg.id = packageId;
    pkg.name = installedContent.packageName;
    pkg.version = installedContent.packageVersion;
    pkg.installPath = installDir;
    pkg.configPath = installedContent.configPath;
    pkg.keywordsPath = installedContent.keywordsPath;
    pkg.compilerPath = installedContent.compilerDir;
    pkg.libPath = installedContent.libDir;
    pkg.includePath = installedContent.includeDir;
    pkg.helpPath = installedContent.helpDir;
    pkg.isActive = true;
    pkg.installDate = QDateTime::currentDateTime();
    
    m_packages[packageId] = pkg;
    
    // Сохраняем индекс
    if (!savePackagesIndex()) {
        error = tr("Не удалось сохранить индекс пакетов");
        return false;
    }
    
    emit packageInstalled(packageId);
    emit installationProgress(100, tr("Пакет %1 успешно установлен").arg(pkg.name));
    
    return true;
}

bool PackageManager::uninstallPackage(const QString &packageId, QString &error)
{
    error.clear();
    
    if (!m_packages.contains(packageId)) {
        error = tr("Пакет не найден: %1").arg(packageId);
        return false;
    }
    
    InstalledPackage pkg = m_packages[packageId];
    
    // Удаляем директорию с пакетом
    if (!removeDirectoryRecursively(pkg.installPath)) {
        error = tr("Не удалось удалить файлы пакета");
        return false;
    }
    
    // Удаляем из индекса
    m_packages.remove(packageId);
    
    if (!savePackagesIndex()) {
        error = tr("Не удалось обновить индекс пакетов");
        return false;
    }
    
    emit packageUninstalled(packageId);
    
    return true;
}

bool PackageManager::removeDirectoryRecursively(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return true;
    }
    
    // Удаляем все содержимое
    QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            removeDirectoryRecursively(entry.absoluteFilePath());
        } else {
            QFile::remove(entry.absoluteFilePath());
        }
    }
    
    // Удаляем саму директорию
    return dir.rmdir(path);
}

QList<InstalledPackage> PackageManager::getInstalledPackages() const
{
    return m_packages.values();
}

InstalledPackage PackageManager::getPackageInfo(const QString &packageId) const
{
    if (m_packages.contains(packageId)) {
        return m_packages[packageId];
    }
    return InstalledPackage();
}

bool PackageManager::isPackageInstalled(const QString &packageId) const
{
    return m_packages.contains(packageId);
}

bool PackageManager::setPackageActive(const QString &packageId, bool active)
{
    if (!m_packages.contains(packageId)) {
        return false;
    }
    
    m_packages[packageId].isActive = active;
    return savePackagesIndex();
}

QStringList PackageManager::getActiveIncludePaths() const
{
    QStringList paths;
    for (const InstalledPackage &pkg : m_packages) {
        if (pkg.isActive && !pkg.includePath.isEmpty()) {
            paths.append(pkg.includePath);
        }
    }
    return paths;
}

QStringList PackageManager::getActiveLibPaths() const
{
    QStringList paths;
    for (const InstalledPackage &pkg : m_packages) {
        if (pkg.isActive && !pkg.libPath.isEmpty()) {
            paths.append(pkg.libPath);
        }
    }
    return paths;
}

QStringList PackageManager::getActiveCompilerPaths() const
{
    QStringList paths;
    for (const InstalledPackage &pkg : m_packages) {
        if (pkg.isActive && !pkg.compilerPath.isEmpty()) {
            paths.append(pkg.compilerPath);
        }
    }
    return paths;
}

bool PackageManager::exportToConfig(const QString &configPath)
{
    // TODO: Экспорт путей активных пакетов в главный конфиг редактора
    // Это будет интегрировано с существующей системой конфигурации
    qDebug() << "Exporting package config to:" << configPath;
    return true;
}
