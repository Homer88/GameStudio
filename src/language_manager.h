#ifndef LANGUAGE_MANAGER_H
#define LANGUAGE_MANAGER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QLocale>
#include <QSettings>

class LanguageManager : public QObject
{
    Q_OBJECT

public:
    static LanguageManager& instance();
    
    void initialize(const QString& langPath, const QString& configPath);
    QString getCurrentLanguage() const;
    QStringList getAvailableLanguages() const;
    QString getLanguageName(const QString& code) const;
    bool setLanguage(const QString& languageCode);
    QString translate(const QString& key) const;
    void saveConfiguration();
    void loadConfiguration();

private:
    LanguageManager() = default;
    ~LanguageManager() = default;
    LanguageManager(const LanguageManager&) = delete;
    LanguageManager& operator=(const LanguageManager&) = delete;
    
    QString detectSystemLanguage() const;
    bool loadLanguageFile(const QString& filePath);
    QString getConfigPath() const { return m_configPath; }

    QString m_currentLanguage;
    QString m_langPath;
    QString m_configPath;
    QMap<QString, QString> m_translations;
    QMap<QString, QString> m_languageNames;
};

#endif // LANGUAGE_MANAGER_H
