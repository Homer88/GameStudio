#include "packageparser.h"
#include <QZipReader>
#include <QZipWriter>
#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QXmlStreamReader>
#include <QDebug>

PackageParser::PackageParser(QObject *parent) : QObject(parent)
{
}

PackageContent::HelpFile::Format PackageParser::detectHelpFormat(const QString &fileName)
{
    QString ext = QFileInfo(fileName).suffix().toLower();
    if (ext == "html" || ext == "htm")
        return PackageContent::HelpFile::HTML;
    else if (ext == "chm")
        return PackageContent::HelpFile::CHM;
    else if (ext == "md" || ext == "markdown")
        return PackageContent::HelpFile::MD;
    else if (ext == "pdf")
        return PackageContent::HelpFile::PDF;
    return PackageContent::HelpFile::UNKNOWN;
}

bool PackageParser::checkFileExists(const QDir &dir, const QString &fileName)
{
    return dir.exists(fileName);
}

bool PackageParser::findDirectory(QDir &rootDir, const QString &dirName, QDir &foundDir)
{
    // Ищем директорию с указанным именем в корневой директории или на один уровень глубже
    if (rootDir.exists(dirName)) {
        foundDir.setPath(rootDir.absoluteFilePath(dirName));
        return true;
    }
    
    // Поиск в поддиректориях первого уровня
    QFileInfoList entries = rootDir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &entry : entries) {
        QDir subDir(entry.absoluteFilePath());
        if (subDir.exists(dirName)) {
            foundDir.setPath(subDir.absoluteFilePath(dirName));
            return true;
        }
    }
    
    return false;
}

QString PackageParser::extractConfigValue(const QString &configPath, const QString &key)
{
    QFile file(configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    
    QXmlStreamReader reader(&file);
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        if (reader.isStartElement() && reader.name() == key) {
            return reader.readElementText();
        }
    }
    return QString();
}

bool PackageParser::parseArchive(const QString &archivePath, PackageContent &content)
{
    content.isValid = false;
    content.errorMessage.clear();
    content.helpFiles.clear();
    
    if (!QFile::exists(archivePath)) {
        content.errorMessage = tr("Архив не найден: %1").arg(archivePath);
        return false;
    }
    
    QZipReader zipReader(archivePath);
    if (!zipReader.isOpen()) {
        content.errorMessage = tr("Не удалось открыть архив: %1").arg(archivePath);
        return false;
    }
    
    QList<QZipReader::FileInfo> entries = zipReader.fileInfoList();
    if (entries.isEmpty()) {
        content.errorMessage = tr("Архив пуст");
        return false;
    }
    
    // Анализируем содержимое архива
    bool hasConfig = false;
    bool hasKeywords = false;
    bool hasLibDir = false;
    bool hasIncludeDir = false;
    bool hasCompilerDir = false;
    bool hasHelpDir = false;
    
    QString basePath;
    
    for (const QZipReader::FileInfo &info : entries) {
        QString filePath = info.filePath;
        
        // Определяем базовую директорию (первый уровень)
        if (basePath.isEmpty() && filePath.contains('/')) {
            basePath = filePath.section('/', 0, 0);
        }
        
        // Проверяем наличие обязательных файлов
        if (filePath.endsWith("config.xml") || filePath.endsWith("package.xml")) {
            hasConfig = true;
            content.configPath = filePath;
        }
        
        if (filePath.endsWith("keywords.txt") || filePath.endsWith("keywords.xml") || 
            filePath.endsWith("syntax.dat")) {
            hasKeywords = true;
            content.keywordsPath = filePath;
        }
        
        // Проверяем директории
        if (info.isDir) {
            if (filePath.contains("/compiler/") || filePath.endsWith("compiler/")) {
                hasCompilerDir = true;
                content.compilerDir = filePath;
            }
            if (filePath.contains("/lib/") || filePath.endsWith("lib/")) {
                hasLibDir = true;
                content.libDir = filePath;
            }
            if (filePath.contains("/include/") || filePath.endsWith("include/")) {
                hasIncludeDir = true;
                content.includeDir = filePath;
            }
            if (filePath.contains("/help/") || filePath.endsWith("help/") ||
                filePath.contains("/doc/") || filePath.endsWith("doc/")) {
                hasHelpDir = true;
                content.helpDir = filePath;
            }
        }
        
        // Собираем файлы помощи
        if (hasHelpDir || filePath.contains("/help/") || filePath.contains("/doc/")) {
            PackageContent::HelpFile::Format fmt = detectHelpFormat(filePath);
            if (fmt != PackageContent::HelpFile::UNKNOWN) {
                PackageContent::HelpFile hf;
                hf.path = filePath;
                hf.format = fmt;
                content.helpFiles.append(hf);
            }
        }
    }
    
    // Валидация обязательных элементов
    if (!hasConfig) {
        content.errorMessage = tr("Отсутствует файл конфигурации (config.xml или package.xml)");
        return false;
    }
    
    if (!hasKeywords) {
        content.errorMessage = tr("Отсутствует файл кодовых слов (keywords.txt/.xml/.dat)");
        return false;
    }
    
    if (!hasIncludeDir) {
        content.errorMessage = tr("Отсутствует папка include с заголовочными файлами");
        return false;
    }
    
    // libDir может быть пустым, но папка должна существовать
    // Для некоторых пакетов библиотеки могут отсутствовать (только заголовки)
    
    content.isValid = true;
    
    // Извлекаем метаданные из конфига (если возможно)
    // Примечание: для полноценного чтения нужно распаковать файл
    content.packageName = QFileInfo(archivePath).baseName();
    content.packageVersion = "1.0"; // По умолчанию, будет обновлено при распаковке
    
    emit statusMessage(tr("Пакет успешно проанализирован: %1").arg(content.packageName));
    
    return true;
}

bool PackageParser::validateStructure(const QDir &rootDir, PackageContent &content)
{
    content.isValid = false;
    content.errorMessage.clear();
    content.helpFiles.clear();
    
    if (!rootDir.exists()) {
        content.errorMessage = tr("Директория не существует: %1").arg(rootDir.absolutePath());
        return false;
    }
    
    bool hasConfig = false;
    bool hasKeywords = false;
    bool hasIncludeDir = false;
    
    // Поиск файла конфигурации
    QStringList configNames = {"config.xml", "package.xml"};
    for (const QString &configName : configNames) {
        if (checkFileExists(rootDir, configName)) {
            content.configPath = rootDir.absoluteFilePath(configName);
            hasConfig = true;
            
            // Читаем метаданные
            content.packageName = extractConfigValue(content.configPath, "name");
            content.packageVersion = extractConfigValue(content.configPath, "version");
            if (content.packageName.isEmpty())
                content.packageName = rootDir.dirName();
            break;
        }
    }
    
    if (!hasConfig) {
        // Поиск в поддиректориях
        QDirIterator it(rootDir.absolutePath(), configNames, QDir::Files, QDirIterator::Subdirectories);
        if (it.hasNext()) {
            content.configPath = it.next();
            hasConfig = true;
            content.packageName = extractConfigValue(content.configPath, "name");
            content.packageVersion = extractConfigValue(content.configPath, "version");
            if (content.packageName.isEmpty())
                content.packageName = rootDir.dirName();
        }
    }
    
    // Поиск файла ключевых слов
    QStringList keywordNames = {"keywords.txt", "keywords.xml", "syntax.dat"};
    for (const QString &kwName : keywordNames) {
        if (checkFileExists(rootDir, kwName)) {
            content.keywordsPath = rootDir.absoluteFilePath(kwName);
            hasKeywords = true;
            break;
        }
    }
    
    if (!hasKeywords) {
        QDirIterator it(rootDir.absolutePath(), keywordNames, QDir::Files, QDirIterator::Subdirectories);
        if (it.hasNext()) {
            content.keywordsPath = it.next();
            hasKeywords = true;
        }
    }
    
    // Поиск директорий
    QDir tempDir;
    if (findDirectory(const_cast<QDir&>(rootDir), "include", tempDir)) {
        content.includeDir = tempDir.absolutePath();
        hasIncludeDir = true;
    }
    
    findDirectory(const_cast<QDir&>(rootDir), "lib", tempDir);
    if (tempDir.exists())
        content.libDir = tempDir.absolutePath();
    
    findDirectory(const_cast<QDir&>(rootDir), "compiler", tempDir);
    if (tempDir.exists())
        content.compilerDir = tempDir.absolutePath();
    
    if (findDirectory(const_cast<QDir&>(rootDir), "help", tempDir) || 
        findDirectory(const_cast<QDir&>(rootDir), "doc", tempDir)) {
        content.helpDir = tempDir.absolutePath();
        
        // Сканирование файлов помощи
        QDir helpDir(tempDir);
        QStringList helpFilters = {"*.html", "*.htm", "*.chm", "*.md", "*.markdown", "*.pdf"};
        QFileInfoList helpFiles = helpDir.entryInfoList(helpFilters, QDir::Files);
        for (const QFileInfo &hf : helpFiles) {
            PackageContent::HelpFile helpFile;
            helpFile.path = hf.absoluteFilePath();
            helpFile.format = detectHelpFormat(hf.fileName());
            content.helpFiles.append(helpFile);
        }
    }
    
    // Валидация
    if (!hasConfig) {
        content.errorMessage = tr("Отсутствует файл конфигурации");
        return false;
    }
    
    if (!hasKeywords) {
        content.errorMessage = tr("Отсутствует файл кодовых слов");
        return false;
    }
    
    if (!hasIncludeDir) {
        content.errorMessage = tr("Отсутствует папка include");
        return false;
    }
    
    content.isValid = true;
    emit statusMessage(tr("Структура пакета проверена: %1").arg(content.packageName));
    
    return true;
}

bool PackageParser::extractTo(const QString &archivePath, const QString &destDir, PackageContent &content)
{
    content.isValid = false;
    
    if (!QFile::exists(archivePath)) {
        content.errorMessage = tr("Архив не найден: %1").arg(archivePath);
        return false;
    }
    
    // Создаем целевую директорию
    QDir targetDir(destDir);
    if (!targetDir.exists()) {
        if (!targetDir.mkpath(".")) {
            content.errorMessage = tr("Не удалось создать директорию: %1").arg(destDir);
            return false;
        }
    }
    
    QZipReader zipReader(archivePath);
    if (!zipReader.isOpen()) {
        content.errorMessage = tr("Не удалось открыть архив");
        return false;
    }
    
    QList<QZipReader::FileInfo> entries = zipReader.fileInfoList();
    int totalFiles = entries.size();
    int processedFiles = 0;
    
    for (const QZipReader::FileInfo &info : entries) {
        if (info.isDir) {
            targetDir.mkpath(info.filePath);
        } else {
            // Создаем родительские директории
            QFileInfo fi(info.filePath);
            targetDir.mkpath(fi.path());
            
            // Извлекаем файл
            QByteArray data = zipReader.fileData(info.filePath);
            QString destPath = targetDir.absoluteFilePath(info.filePath);
            
            QFile outFile(destPath);
            if (outFile.open(QIODevice::WriteOnly)) {
                outFile.write(data);
                outFile.close();
                
                // Устанавливаем права доступа
                if (!info.permissions.isNull()) {
                    outFile.setPermissions(info.permissions);
                }
            }
        }
        
        processedFiles++;
        int percent = (processedFiles * 100) / totalFiles;
        emit extractionProgress(percent);
    }
    
    zipReader.close();
    
    // Валидируем структуру после распаковки
    if (!validateStructure(targetDir, content)) {
        return false;
    }
    
    emit statusMessage(tr("Пакет успешно установлен: %1").arg(content.packageName));
    return true;
}
