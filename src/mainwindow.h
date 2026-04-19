#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLineEdit>
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTreeView>
#include <QFileSystemModel>
#include <QSplitter>
#include <QTextEdit>
#include <QPlainTextEdit>

class WelcomeScreen;
class ProjectSelectionScreen;
class ProjectCreator;
class SettingsWindow;
class CodeEditor;
class PackageManager;
class PackageDialog;

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
    void showPackageManager();
    
    // Editor actions
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void closeFile();
    void showAbout();
    void applyEditorSettings();
    
    // File tree actions
    void onFileClicked(const QModelIndex &index);
    void openCurrentFile();
    
private:
    void setupUI();
    void setupProjectTemplates();
    void setupMenuBar();
    void createLanguageMenu();
    void createEditorMenu();
    void setupCodeEditor();
    void setupFileTree();
    void setupTerminalPanel();
    void createCentralWidget();
    void setupPackageManager();
    
    QWidget *m_centralWidget;
    QStackedWidget *m_stackedWidget;
    WelcomeScreen *m_welcomeScreen;
    ProjectSelectionScreen *m_projectSelectionScreen;
    ProjectCreator *m_projectCreator;
    SettingsWindow *m_settingsWindow;
    CodeEditor *m_codeEditor;
    PackageManager *m_packageManager;
    PackageDialog *m_packageDialog;
    
    // File tree
    QSplitter *m_mainSplitter;
    QSplitter *m_verticalSplitter;
    QWidget *m_editorContainer;
    QTreeView *m_fileTree;
    QFileSystemModel *m_fileSystemModel;
    
    // Terminal panel
    QWidget *m_terminalPanel;
    QTabWidget *m_terminalTabWidget;
    QTextEdit *m_terminalOutput;
    QTextEdit *m_errorsOutput;
    
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
    QAction *m_showFileTreeAction;
    QAction *m_showTerminalAction;
    
    // Settings menu actions
    QAction *m_settingsAction;
    QAction *m_installPackageAction;
    QAction *m_uninstallPackageAction;
    
    // Project creation dialog widgets
    QLineEdit *m_projectNameEdit;
    QLineEdit *m_projectLocationEdit;
    QPushButton *m_browseButton;
    QPushButton *m_createProjectButton;
    QPushButton *m_cancelButton;
    
    QStringList m_projectTemplates;
    QString m_currentProjectPath;
};

#endif // MAINWINDOW_H
