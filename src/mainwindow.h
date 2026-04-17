#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFileDialog>

class WelcomeScreen;
class ProjectSelectionScreen;
class ProjectCreator;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showWelcomeScreen();
    void showProjectSelectionScreen();
    void createProject(const QString &templateName);
    void goBack();

private:
    void setupUI();
    void setupProjectTemplates();
    
    QStackedWidget *m_stackedWidget;
    WelcomeScreen *m_welcomeScreen;
    ProjectSelectionScreen *m_projectSelectionScreen;
    ProjectCreator *m_projectCreator;
    
    // Project creation dialog widgets
    QLineEdit *m_projectNameEdit;
    QLineEdit *m_projectLocationEdit;
    QPushButton *m_browseButton;
    QPushButton *m_createProjectButton;
    QPushButton *m_cancelButton;
    
    QStringList m_projectTemplates;
};

#endif // MAINWINDOW_H
