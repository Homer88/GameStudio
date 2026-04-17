#include <QApplication>
#include <QDir>
#include <QDebug>
#include "mainwindow.h"
#include "language_manager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Устанавливаем информацию о приложении
    QApplication::setApplicationName("Qt Project Manager");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("MyCompany");
    
    // Определяем пути к языковым файлам и конфигурации
    QString appDir = QCoreApplication::applicationDirPath();
    QString langPath = appDir + "/lang";
    QString configPath = appDir + "/conf/configure.xml";
    
    // Проверяем, существует ли директория lang (для разработки)
    if (!QDir(langPath).exists()) {
        // Пытаемся найти в исходниках
        QString sourceLangPath = QDir::currentPath() + "/lang";
        if (QDir(sourceLangPath).exists()) {
            langPath = sourceLangPath;
        }
    }
    
    // Проверяем, существует ли файл конфигурации
    if (!QFile::exists(configPath)) {
        QString sourceConfigPath = QDir::currentPath() + "/conf/configure.xml";
        if (QFile::exists(sourceConfigPath)) {
            configPath = sourceConfigPath;
        }
    }
    
    qDebug() << "Language path:" << langPath;
    qDebug() << "Config path:" << configPath;
    
    // Инициализируем менеджер языков
    LanguageManager::instance().initialize(langPath, configPath);
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
