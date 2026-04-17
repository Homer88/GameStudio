#ifndef PROJECTSELECTIONSCREEN_H
#define PROJECTSELECTIONSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

class ProjectSelectionScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSelectionScreen(QWidget *parent = nullptr);
    ~ProjectSelectionScreen() = default;

    void setProjectTemplates(const QStringList &templates);
    QString selectedTemplate() const;

signals:
    void createProjectClicked(const QString &templateName);
    void backButtonClicked();

private:
    void setupUI();
    
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QListWidget *m_projectList;
    QPushButton *m_createButton;
    QPushButton *m_backButton;
};

#endif // PROJECTSELECTIONSCREEN_H
