#ifndef PACKAGEPARSER_H
#define PACKAGEPARSER_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QDir>

// Структура, описывающая содержимое пакета
struct PackageContent {
    QString configPath;       // Файл конфигурации (обязательно)
    QString keywordsPath;     // Файл кодовых слов (обязательно)
    QString compilerDir;      // Папка с компилятором (опционально)
    QString libDir;           // Папка с библиотеками
    QString includeDir;       // Папка с заголовками
    QString helpDir;          // Папка с помощью (опционально)
    
    // Детали файлов помощи
    struct HelpFile {
        QString path;
        enum Format { HTML, CHM, MD, PDF, UNKNOWN };
        Format format;
    };
    QList<HelpFile> helpFiles;

    bool isValid = false;
    QString errorMessage;
    QString packageName;
    QString packageVersion;
};

class PackageParser : public QObject
{
    Q_OBJECT
public:
    explicit PackageParser(QObject *parent = nullptr);

    // Основной метод парсинга архива (без распаковки - только проверка структуры)
    bool parseArchive(const QString &archivePath, PackageContent &content);
    
    // Метод распаковки в целевую директорию с валидацией
    bool extractTo(const QString &archivePath, const QString &destDir, PackageContent &content);

    // Валидация структуры директории
    bool validateStructure(const QDir &rootDir, PackageContent &content);

signals:
    void extractionProgress(int percent);
    void statusMessage(const QString &msg);

private:
    PackageContent::HelpFile::Format detectHelpFormat(const QString &fileName);
    bool checkFileExists(const QDir &dir, const QString &fileName);
    bool findDirectory(QDir &rootDir, const QString &dirName, QDir &foundDir);
    QString extractConfigValue(const QString &configPath, const QString &key);
};

#endif // PACKAGEPARSER_H
