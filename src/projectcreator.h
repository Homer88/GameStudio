#ifndef PROJECTCREATOR_H
#define PROJECTCREATOR_H

#include <QObject>
#include <QString>
#include <QDir>

class ProjectCreator : public QObject
{
    Q_OBJECT

public:
    explicit ProjectCreator(QObject *parent = nullptr);
    ~ProjectCreator() = default;

    bool createProject(const QString &projectName, 
                       const QString &location, 
                       const QString &templateType);
    
    QString lastError() const { return m_lastError; }

private:
    bool createCMakeListsFile(const QString &projectPath, const QString &projectName);
    bool createMainCppFile(const QString &projectPath);
    bool createHeaderFile(const QString &projectPath, const QString &projectName);
    
    QString m_lastError;
};

#endif // PROJECTCREATOR_H
