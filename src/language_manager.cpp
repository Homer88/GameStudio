#include "language_manager.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QLocale>
#include <QtXml/QDomDocument>

LanguageManager& LanguageManager::instance()
{
    static LanguageManager instance;
    return instance;
}

void LanguageManager::initialize(const QString& langPath, const QString& configPath)
{
    m_langPath = langPath;
    m_configPath = configPath;
    
    loadConfiguration();
    
    if (m_currentLanguage == "auto" || m_currentLanguage.isEmpty()) {
        m_currentLanguage = detectSystemLanguage();
    }
    
    if (!loadLanguageFile(m_langPath + "/" + m_currentLanguage + ".xml")) {
        if (!loadLanguageFile(m_langPath + "/en.xml")) {
            m_currentLanguage = "en";
        }
    }
}

QString LanguageManager::getCurrentLanguage() const
{
    return m_currentLanguage;
}

QStringList LanguageManager::getAvailableLanguages() const
{
    QStringList languages;
    QDir dir(m_langPath);
    QFileInfoList files = dir.entryInfoList(QStringList() << "*.xml", QDir::Files);
    
    for (const QFileInfo& file : files) {
        QString code = file.baseName();
        languages.append(code);
    }
    
    return languages;
}

QString LanguageManager::getLanguageName(const QString& code) const
{
    if (m_languageNames.contains(code)) {
        return m_languageNames[code];
    }
    
    QFile file(m_langPath + "/" + code + ".xml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QXmlStreamReader reader(&file);
        while (!reader.atEnd() && !reader.hasError()) {
            reader.readNext();
            if (reader.isStartElement() && reader.name() == "name") {
                QString name = reader.readElementText();
                file.close();
                return name;
            }
        }
        file.close();
    }
    
    return code;
}

bool LanguageManager::setLanguage(const QString& languageCode)
{
    if (loadLanguageFile(m_langPath + "/" + languageCode + ".xml")) {
        m_currentLanguage = languageCode;
        saveConfiguration();
        return true;
    }
    return false;
}

QString LanguageManager::translate(const QString& key) const
{
    if (m_translations.contains(key)) {
        return m_translations[key];
    }
    return key;
}

void LanguageManager::saveConfiguration()
{
    QDomDocument doc("configuration");
    QDomElement root = doc.createElement("configuration");
    doc.appendChild(root);
    
    QDomElement appElem = doc.createElement("application");
    QDomElement nameElem = doc.createElement("name");
    nameElem.appendChild(doc.createTextNode("ProjectManager"));
    appElem.appendChild(nameElem);
    QDomElement versionElem = doc.createElement("version");
    versionElem.appendChild(doc.createTextNode("1.0.0"));
    appElem.appendChild(versionElem);
    root.appendChild(appElem);
    
    QDomElement interfaceElem = doc.createElement("interface");
    QDomElement langElem = doc.createElement("language");
    langElem.appendChild(doc.createTextNode(m_currentLanguage));
    interfaceElem.appendChild(langElem);
    root.appendChild(interfaceElem);
    
    QDomElement projectsElem = doc.createElement("projects");
    QDomElement defaultLocElem = doc.createElement("default_location");
    projectsElem.appendChild(defaultLocElem);
    root.appendChild(projectsElem);
    
    QFile file(m_configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream.setCodec("UTF-8");
        stream << doc.toString(4);
        file.close();
    }
}

void LanguageManager::loadConfiguration()
{
    QFile file(m_configPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_currentLanguage = "auto";
        return;
    }
    
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        m_currentLanguage = "auto";
        return;
    }
    file.close();
    
    QDomElement root = doc.documentElement();
    QDomElement interfaceElem = root.firstChildElement("interface");
    if (!interfaceElem.isNull()) {
        QDomElement langElem = interfaceElem.firstChildElement("language");
        if (!langElem.isNull()) {
            m_currentLanguage = langElem.text();
        }
    }
}

QString LanguageManager::detectSystemLanguage() const
{
    QLocale locale;
    QString lang = locale.name().left(2);
    
    QDir dir(m_langPath);
    QString possibleFile = m_langPath + "/" + lang + ".xml";
    
    if (QFile::exists(possibleFile)) {
        return lang;
    }
    
    return "en";
}

bool LanguageManager::loadLanguageFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    m_translations.clear();
    
    QXmlStreamReader reader(&file);
    bool inTranslations = false;
    
    while (!reader.atEnd() && !reader.hasError()) {
        reader.readNext();
        
        if (reader.isStartElement()) {
            if (reader.name() == "translations") {
                inTranslations = true;
            } else if (inTranslations && reader.name() == "message") {
                QStringRef nameRef = reader.attributes().value("name");
                if (!nameRef.isEmpty()) {
                    QString name = nameRef.toString();
                    QString value = reader.readElementText();
                    m_translations[name] = value;
                }
            } else if (reader.name() == "code") {
                QString code = reader.readElementText();
            } else if (reader.name() == "name") {
                QStringRef codeRef = reader.attributes().value("code");
                if (codeRef.isEmpty() && !m_currentLanguage.isEmpty()) {
                } else {
                    QString name = reader.readElementText();
                    if (!codeRef.isEmpty()) {
                        m_languageNames[codeRef.toString()] = name;
                    }
                }
            }
        } else if (reader.isEndElement() && reader.name() == "translations") {
            inTranslations = false;
        }
    }
    
    file.close();
    
    if (reader.hasError()) {
        m_translations.clear();
        return false;
    }
    
    return !m_translations.isEmpty();
}
