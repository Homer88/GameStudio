#include "projectcreator.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDateTime>
#include <QCoreApplication>

ProjectCreator::ProjectCreator(QObject *parent)
    : QObject(parent)
{
}

bool ProjectCreator::createProject(const QString &projectName, 
                                   const QString &location, 
                                   const QString &templateType)
{
    Q_UNUSED(templateType); // Пока используем один шаблон, позже добавим варианты
    
    QDir parentDir(location);
    if (!parentDir.exists()) {
        m_lastError = "Указанная директория не существует";
        return false;
    }
    
    QString projectPath = location + "/" + projectName;
    QDir projectDir(projectPath);
    
    if (projectDir.exists()) {
        m_lastError = "Проект с таким именем уже существует в указанной директории";
        return false;
    }
    
    // Создаем директорию проекта
    if (!projectDir.mkpath(".")) {
        m_lastError = "Не удалось создать директорию проекта";
        return false;
    }
    
    // Создаем поддиректории
    projectDir.mkdir("src");
    projectDir.mkdir("include");
    
    // Создаем файлы проекта
    if (!createCMakeListsFile(projectPath, projectName)) {
        return false;
    }
    
    if (!createMainCppFile(projectPath)) {
        return false;
    }
    
    if (!createHeaderFile(projectPath, projectName)) {
        return false;
    }
    
    return true;
}

bool ProjectCreator::createCMakeListsFile(const QString &projectPath, const QString &projectName)
{
    QString filePath = projectPath + "/CMakeLists.txt";
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = "Не удалось создать файл CMakeLists.txt";
        return false;
    }
    
    QTextStream out(&file);
    out << "cmake_minimum_required(VERSION 3.16)\n";
    out << "project(" << projectName << " VERSION 1.0.0 LANGUAGES CXX)\n\n";
    out << "set(CMAKE_CXX_STANDARD 17)\n";
    out << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n";
    out << "set(CMAKE_AUTOMOC ON)\n";
    out << "set(CMAKE_AUTORCC ON)\n";
    out << "set(CMAKE_AUTOUIC ON)\n\n";
    out << "find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)\n";
    out << "find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)\n\n";
    out << "set(PROJECT_SOURCES\n";
    out << "    src/main.cpp\n";
    out << ")\n\n";
    out << "if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)\n";
    out << "    qt_add_executable(" << projectName << "\n";
    out << "        MANUAL_FINALIZATION\n";
    out << "        ${PROJECT_SOURCES}\n";
    out << "    )\n";
    out << "else()\n";
    out << "    add_executable(" << projectName << "\n";
    out << "        ${PROJECT_SOURCES}\n";
    out << "    )\n";
    out << "endif()\n\n";
    out << "target_link_libraries(" << projectName << " PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)\n";
    
    if (QT_VERSION_MAJOR >= 6) {
        out << "\nqt_finalize_executable(" << projectName << ")\n";
    }
    
    file.close();
    return true;
}

bool ProjectCreator::createMainCppFile(const QString &projectPath)
{
    QString filePath = projectPath + "/src/main.cpp";
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = "Не удалось создать файл main.cpp";
        return false;
    }
    
    QTextStream out(&file);
    out << "#include <QApplication>\n";
    out << "#include <QWidget>\n\n";
    out << "int main(int argc, char *argv[])\n";
    out << "{\n";
    out << "    QApplication app(argc, argv);\n\n";
    out << "    QWidget window;\n";
    out << "    window.resize(320, 240);\n";
    out << "    window.setWindowTitle(\"New Project\");\n";
    out << "    window.show();\n\n";
    out << "    return app.exec();\n";
    out << "}\n";
    
    file.close();
    return true;
}

bool ProjectCreator::createHeaderFile(const QString &projectPath, const QString &projectName)
{
    QString fileName = projectName.toLower() + ".h";
    QString filePath = projectPath + "/include/" + fileName;
    QFile file(filePath);
    
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_lastError = "Не удалось создать файл заголовка";
        return false;
    }
    
    QString guardName = projectName.toUpper().replace(QRegExp("[^A-Z0-9]"), "_") + "_H";
    
    QTextStream out(&file);
    out << "#ifndef " << guardName << "\n";
    out << "#define " << guardName << "\n\n";
    out << "#include <QWidget>\n\n";
    out << "class " << projectName << " : public QWidget\n";
    out << "{\n";
    out << "    Q_OBJECT\n\n";
    out << "public:\n";
    out << "    explicit " << projectName << "(QWidget *parent = nullptr);\n";
    out << "    ~" << projectName << "() = default;\n";
    out << "};\n\n";
    out << "#endif // " << guardName << "\n";
    
    file.close();
    return true;
}
