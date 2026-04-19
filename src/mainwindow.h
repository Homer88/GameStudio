#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

class WelcomeScreen;
class ProjectSelectionScreen;
class ProjectCreator;
class SettingsWindow;
class CodeEditor;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateLanguage();

private slots:
    void showWelcomeScreen();
    void showProjectSelectionScreen();
    void createProject(const QString &templateName);
    void goBack();
    void changeLanguage();
    void showSettings();
    
    // Editor actions
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void closeFile();
    void showAbout();
    void applyEditorSettings();

private:
    void setupUI();
    void setupProjectTemplates();
    void setupMenuBar();
    void createLanguageMenu();
    void createEditorMenu();
    void setupCodeEditor();
    
    QStackedWidget *m_stackedWidget;
    WelcomeScreen *m_welcomeScreen;
    ProjectSelectionScreen *m_projectSelectionScreen;
    ProjectCreator *m_projectCreator;
    SettingsWindow *m_settingsWindow;
    CodeEditor *m_codeEditor;
    
    // Menu bar
    QMenuBar *m_menuBar;
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_viewMenu;
    QMenu *m_settingsMenu;
    QMenu *m_helpMenu;
    QMenu *m_languageMenu;
    QActionGroup *m_languageActionGroup;
    
    // File menu actions
    QAction *m_newAction;
    QAction *m_openAction;
    QAction *m_saveAction;
    QAction *m_saveAsAction;
    QAction *m_closeAction;
    
    // Edit menu actions
    QAction *m_undoAction;
    QAction *m_redoAction;
    QAction *m_cutAction;
    QAction *m_copyAction;
    QAction *m_pasteAction;
    
    // View menu actions
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    
    // Settings menu actions
    QAction *m_settingsAction;
    
    // Project creation dialog widgets
    QLineEdit *m_projectNameEdit;
    QLineEdit *m_projectLocationEdit;
    QPushButton *m_browseButton;
    QPushButton *m_createProjectButton;
    QPushButton *m_cancelButton;
    
    QStringList m_projectTemplates;
};

#endif // MAINWINDOW_H
