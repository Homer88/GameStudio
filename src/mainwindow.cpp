#include "mainwindow.h"
#include "welcomescreen.h"
#include "projectselectionscreen.h"
#include "projectcreator.h"
#include "settings/settingsdialog.h"
#include "codeeditor.h"
#include "language_manager.h"
#include "core/packagemanager.h"
#include "core/packagedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QDialog>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QFontDialog>
#include <QSettings>
#include <QApplication>
#include <QHeaderView>
#include <QDir>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_settingsWindow(nullptr)
    , m_codeEditor(nullptr)
    , m_packageManager(nullptr)
    , m_packageDialog(nullptr)
    , m_fileTree(nullptr)
    , m_fileSystemModel(nullptr)
    , m_terminalPanel(nullptr)
    , m_terminalTabWidget(nullptr)
    , m_terminalOutput(nullptr)
    , m_errorsOutput(nullptr)
    , m_fileMenu(nullptr)
    , m_editMenu(nullptr)
    , m_viewMenu(nullptr)
    , m_helpMenu(nullptr)
    , m_currentProjectPath(QString())
{
    setWindowTitle(LanguageManager::instance().translate("app_title"));
    setMinimumSize(1024, 768);
    
    m_projectCreator = new ProjectCreator(this);
    setupProjectTemplates();
    setupPackageManager();
    setupMenuBar();
    setupUI();
    
    updateLanguage();
    showWelcomeScreen();
}

MainWindow::~MainWindow()
{
}

void MainWindow::updateLanguage()
{
    setWindowTitle(LanguageManager::instance().translate("app_title"));
    
    if (m_welcomeScreen) {
        m_welcomeScreen->updateLanguage();
    }
    
    if (m_projectSelectionScreen) {
        m_projectSelectionScreen->updateLanguage();
    }
}

void MainWindow::setupMenuBar()
{
    m_menuBar = menuBar();
    
    // File Menu
    m_fileMenu = m_menuBar->addMenu(tr("&File"));
    
    m_newAction = new QAction(tr("&New"), this);
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &MainWindow::newFile);
    m_fileMenu->addAction(m_newAction);
    
    m_openAction = new QAction(tr("&Open..."), this);
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &MainWindow::openFile);
    m_fileMenu->addAction(m_openAction);
    
    m_saveAction = new QAction(tr("&Save"), this);
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &MainWindow::saveFile);
    m_fileMenu->addAction(m_saveAction);
    
    m_saveAsAction = new QAction(tr("Save &As..."), this);
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);
    m_fileMenu->addAction(m_saveAsAction);
    
    m_closeAction = new QAction(tr("&Close"), this);
    m_closeAction->setShortcut(QKeySequence::Close);
    connect(m_closeAction, &QAction::triggered, this, &MainWindow::closeFile);
    m_fileMenu->addAction(m_closeAction);
    
    m_fileMenu->addSeparator();
    
    // Exit action
    QAction* exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    m_fileMenu->addAction(exitAction);
    
    // Edit Menu
    m_editMenu = m_menuBar->addMenu(tr("&Edit"));
    
    m_undoAction = new QAction(tr("&Undo"), this);
    m_undoAction->setShortcut(QKeySequence::Undo);
    connect(m_undoAction, &QAction::triggered, []() {
        QApplication::focusWidget()->setProperty("undo", true);
    });
    m_editMenu->addAction(m_undoAction);
    
    m_redoAction = new QAction(tr("&Redo"), this);
    m_redoAction->setShortcut(QKeySequence::Redo);
    m_editMenu->addAction(m_redoAction);
    
    m_editMenu->addSeparator();
    
    m_cutAction = new QAction(tr("Cu&t"), this);
    m_cutAction->setShortcut(QKeySequence::Cut);
    m_editMenu->addAction(m_cutAction);
    
    m_copyAction = new QAction(tr("&Copy"), this);
    m_copyAction->setShortcut(QKeySequence::Copy);
    m_editMenu->addAction(m_copyAction);
    
    m_pasteAction = new QAction(tr("&Paste"), this);
    m_pasteAction->setShortcut(QKeySequence::Paste);
    m_editMenu->addAction(m_pasteAction);
    
    // View Menu
    m_viewMenu = m_menuBar->addMenu(tr("&View"));
    
    m_showFileTreeAction = new QAction(tr("Show &File Tree"), this);
    m_showFileTreeAction->setCheckable(true);
    m_showFileTreeAction->setChecked(true);
    m_showFileTreeAction->setShortcut(tr("Ctrl+Shift+F"));
    connect(m_showFileTreeAction, &QAction::triggered, this, [this](bool checked) {
        if (m_fileTree) {
            m_fileTree->setVisible(checked);
        }
    });
    m_viewMenu->addAction(m_showFileTreeAction);
    
    m_showTerminalAction = new QAction(tr("Show &Terminal"), this);
    m_showTerminalAction->setCheckable(true);
    m_showTerminalAction->setChecked(true);
    m_showTerminalAction->setShortcut(tr("Ctrl+Shift+T"));
    connect(m_showTerminalAction, &QAction::triggered, this, [this](bool checked) {
        if (m_terminalPanel) {
            m_terminalPanel->setVisible(checked);
        }
    });
    m_viewMenu->addAction(m_showTerminalAction);
    
    m_viewMenu->addSeparator();
    
    m_zoomInAction = new QAction(tr("Zoom &In"), this);
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(m_zoomInAction, &QAction::triggered, [this]() {
        if (m_codeEditor) {
            QFont font = m_codeEditor->font();
            font.setPointSize(font.pointSize() + 1);
            m_codeEditor->setFont(font);
        }
    });
    m_viewMenu->addAction(m_zoomInAction);
    
    m_zoomOutAction = new QAction(tr("Zoom &Out"), this);
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(m_zoomOutAction, &QAction::triggered, [this]() {
        if (m_codeEditor) {
            QFont font = m_codeEditor->font();
            if (font.pointSize() > 6) {
                font.setPointSize(font.pointSize() - 1);
                m_codeEditor->setFont(font);
            }
        }
    });
    m_viewMenu->addAction(m_zoomOutAction);
    
    // Settings Menu
    m_settingsMenu = m_menuBar->addMenu(tr("&Settings"));
    
    m_settingsAction = new QAction(tr("&Settings..."), this);
    connect(m_settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    m_settingsMenu->addAction(m_settingsAction);
    
    m_settingsMenu->addSeparator();
    
    // Package management actions
    m_installPackageAction = new QAction(tr("&Install Package..."), this);
    connect(m_installPackageAction, &QAction::triggered, this, &MainWindow::showPackageManager);
    m_settingsMenu->addAction(m_installPackageAction);
    
    m_uninstallPackageAction = new QAction(tr("&Uninstall Package..."), this);
    connect(m_uninstallPackageAction, &QAction::triggered, this, &MainWindow::showPackageManager);
    m_settingsMenu->addAction(m_uninstallPackageAction);
    
    m_settingsMenu->addSeparator();
    
    createLanguageMenu();
    
    // Help Menu
    m_helpMenu = m_menuBar->addMenu(tr("&Help"));
    
    QAction* aboutAction = new QAction(tr("&About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    m_helpMenu->addAction(aboutAction);
}

void MainWindow::createLanguageMenu()
{
    m_languageMenu = m_settingsMenu->addMenu(LanguageManager::instance().translate("language_menu"));
    
    m_languageActionGroup = new QActionGroup(this);
    m_languageActionGroup->setExclusive(true);
    
    LanguageManager& langMgr = LanguageManager::instance();
    QStringList languages = langMgr.getAvailableLanguages();
    
    for (const QString& langCode : languages) {
        QString langName = langMgr.getLanguageName(langCode);
        QAction* action = new QAction(langName, this);
        action->setCheckable(true);
        action->setData(langCode);
        
        if (langCode == langMgr.getCurrentLanguage()) {
            action->setChecked(true);
        }
        
        m_languageActionGroup->addAction(action);
        m_languageMenu->addAction(action);
        
        connect(action, &QAction::triggered, this, [this, langCode]() {
            changeLanguage();
        });
    }
}

void MainWindow::changeLanguage()
{
    QAction* action = qobject_cast<QAction*>(sender());
    if (!action) return;
    
    QString langCode = action->data().toString();
    LanguageManager& langMgr = LanguageManager::instance();
    
    if (langMgr.setLanguage(langCode)) {
        updateLanguage();
        
        m_languageMenu->clear();
        createLanguageMenu();
    }
}

void MainWindow::setupProjectTemplates()
{
    // Список шаблонов проектов (можно будет дополнить позже)
    m_projectTemplates << "Qt Widgets Application"
                       << "Console Application"
                       << "Empty Project";
}

void MainWindow::setupUI()
{
    createCentralWidget();
    
    // Welcome Screen
    m_welcomeScreen = new WelcomeScreen();
    m_stackedWidget->addWidget(m_welcomeScreen);
    
    // Project Selection Screen
    m_projectSelectionScreen = new ProjectSelectionScreen();
    m_projectSelectionScreen->setProjectTemplates(m_projectTemplates);
    m_stackedWidget->addWidget(m_projectSelectionScreen);
    
    // Code Editor (initially hidden)
    setupCodeEditor();
    
    // File Tree
    setupFileTree();
    
    // Terminal Panel
    setupTerminalPanel();
    
    // Connect signals
    connect(m_welcomeScreen, &WelcomeScreen::startButtonClicked, 
            this, &MainWindow::showProjectSelectionScreen);
    
    connect(m_projectSelectionScreen, &ProjectSelectionScreen::createProjectClicked,
            this, &MainWindow::createProject);
    
    connect(m_projectSelectionScreen, &ProjectSelectionScreen::backButtonClicked,
            this, &MainWindow::goBack);
    
    connect(m_fileTree, &QTreeView::clicked, this, &MainWindow::onFileClicked);
}

void MainWindow::createCentralWidget()
{
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // Main vertical splitter (editor + terminal)
    m_verticalSplitter = new QSplitter(Qt::Vertical, m_centralWidget);
    m_verticalSplitter->setHandleWidth(4);
    
    // Main horizontal splitter (file tree + editor)
    m_mainSplitter = new QSplitter(Qt::Horizontal, m_verticalSplitter);
    m_mainSplitter->setHandleWidth(4);
    
    // File tree (left panel)
    m_fileTree = new QTreeView(m_mainSplitter);
    m_fileTree->setMinimumWidth(200);
    m_fileTree->setMaximumWidth(500);
    m_fileTree->setHeaderHidden(true);
    m_fileTree->setAnimated(true);
    m_fileTree->setIndentation(20);
    m_fileTree->setSortingEnabled(true);
    
    // Editor container (right panel)
    m_editorContainer = new QWidget(m_mainSplitter);
    QVBoxLayout *editorLayout = new QVBoxLayout(m_editorContainer);
    editorLayout->setContentsMargins(0, 0, 0, 0);
    
    m_stackedWidget = new QStackedWidget(m_editorContainer);
    editorLayout->addWidget(m_stackedWidget);
    
    m_mainSplitter->addWidget(m_fileTree);
    m_mainSplitter->addWidget(m_editorContainer);
    m_mainSplitter->setStretchFactor(0, 0);
    m_mainSplitter->setStretchFactor(1, 1);
    
    // Terminal panel (bottom)
    m_terminalPanel = new QWidget(m_verticalSplitter);
    QVBoxLayout *terminalLayout = new QVBoxLayout(m_terminalPanel);
    terminalLayout->setContentsMargins(0, 0, 0, 0);
    
    m_terminalTabWidget = new QTabWidget(m_terminalPanel);
    
    // Terminal output tab
    m_terminalOutput = new QTextEdit(m_terminalPanel);
    m_terminalOutput->setReadOnly(true);
    m_terminalOutput->setFont(QFont("Courier New", 9));
    m_terminalOutput->setStyleSheet("background-color: #1e1e1e; color: #d4d4d4;");
    m_terminalTabWidget->addTab(m_terminalOutput, tr("Terminal"));
    
    // Errors output tab
    m_errorsOutput = new QTextEdit(m_terminalPanel);
    m_errorsOutput->setReadOnly(true);
    m_errorsOutput->setFont(QFont("Courier New", 9));
    m_errorsOutput->setStyleSheet("background-color: #1e1e1e; color: #f48771;");
    m_terminalTabWidget->addTab(m_errorsOutput, tr("Problems"));
    
    terminalLayout->addWidget(m_terminalTabWidget);
    
    m_verticalSplitter->addWidget(m_mainSplitter);
    m_verticalSplitter->addWidget(m_terminalPanel);
    m_verticalSplitter->setStretchFactor(0, 3);
    m_verticalSplitter->setStretchFactor(1, 1);
    
    // Set initial sizes
    m_verticalSplitter->setSizes(QList<int>() << 500 << 150);
    m_mainSplitter->setSizes(QList<int>() << 250 << 750);
    
    mainLayout->addWidget(m_verticalSplitter);
}

void MainWindow::setupCodeEditor()
{
    m_codeEditor = new CodeEditor(this);
    m_stackedWidget->addWidget(m_codeEditor);
    
    // Connect editor signals
    connect(m_codeEditor, &CodeEditor::modificationChanged, this, [this](bool changed) {
        QString title = LanguageManager::instance().translate("app_title");
        if (!m_codeEditor->currentFilePath().isEmpty()) {
            QFileInfo fi(m_codeEditor->currentFilePath());
            title = QString("%1[*] - %2").arg(fi.fileName()).arg(title);
        } else {
            title = QString("[*]%1").arg(title);
        }
        if (changed) {
            setWindowModified(true);
        }
        setWindowTitle(title);
    });
    
    connect(m_codeEditor, &CodeEditor::fileNameChanged, this, [this](const QString &fileName) {
        QString title = LanguageManager::instance().translate("app_title");
        if (!fileName.isEmpty()) {
            QFileInfo fi(fileName);
            title = QString("%1[*] - %2").arg(fi.fileName()).arg(title);
        }
        setWindowTitle(title);
    });
    
    // Connect undo/redo actions to editor
    connect(m_undoAction, &QAction::triggered, m_codeEditor, &QPlainTextEdit::undo);
    connect(m_redoAction, &QAction::triggered, m_codeEditor, &QPlainTextEdit::redo);
    connect(m_cutAction, &QAction::triggered, m_codeEditor, &QPlainTextEdit::cut);
    connect(m_copyAction, &QAction::triggered, m_codeEditor, &QPlainTextEdit::copy);
    connect(m_pasteAction, &QAction::triggered, m_codeEditor, &QPlainTextEdit::paste);
    
    // Load editor settings from config
    applyEditorSettings();
}

void MainWindow::setupFileTree()
{
    m_fileSystemModel = new QFileSystemModel(this);
    m_fileSystemModel->setRootPath(QDir::homePath());
    m_fileSystemModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    
    m_fileTree->setModel(m_fileSystemModel);
    m_fileTree->setRootIndex(m_fileSystemModel->index(QDir::homePath()));
    
    // Set column widths (only show name column)
    m_fileTree->setColumnHidden(1, true);
    m_fileTree->setColumnHidden(2, true);
    m_fileTree->setColumnHidden(3, true);
    
    m_fileTree->hide(); // Initially hidden until project is opened
}

void MainWindow::setupTerminalPanel()
{
    m_terminalPanel->hide(); // Initially hidden
}

void MainWindow::onFileClicked(const QModelIndex &index)
{
    if (!m_fileSystemModel) return;
    
    QString filePath = m_fileSystemModel->filePath(index);
    QFileInfo fileInfo(filePath);
    
    // Only open files, not directories
    if (fileInfo.isFile()) {
        // Check if it's a source file
        QString suffix = fileInfo.suffix().toLower();
        if (suffix == "cpp" || suffix == "h" || suffix == "hpp" || 
            suffix == "c" || suffix == "cc" || suffix == "cxx") {
            m_stackedWidget->setCurrentWidget(m_codeEditor);
            m_codeEditor->loadFile(filePath);
        }
    }
}

void MainWindow::openCurrentFile()
{
    QModelIndex index = m_fileTree->currentIndex();
    if (index.isValid()) {
        onFileClicked(index);
    }
}

void MainWindow::showWelcomeScreen()
{
    m_stackedWidget->setCurrentWidget(m_welcomeScreen);
}

void MainWindow::showProjectSelectionScreen()
{
    m_stackedWidget->setCurrentWidget(m_projectSelectionScreen);
}

void MainWindow::createProject(const QString &templateName)
{
    // Создаем диалоговое окно для ввода имени проекта и выбора lokasi
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Создание нового проекта");
    dialog->setMinimumSize(500, 300);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(dialog);
    
    // Project Name
    QGroupBox *nameGroup = new QGroupBox("Имя проекта", dialog);
    QVBoxLayout *nameLayout = new QVBoxLayout(nameGroup);
    m_projectNameEdit = new QLineEdit(dialog);
    m_projectNameEdit->setPlaceholderText("Введите имя проекта");
    nameLayout->addWidget(m_projectNameEdit);
    mainLayout->addWidget(nameGroup);
    
    // Project Location
    QGroupBox *locationGroup = new QGroupBox("Расположение", dialog);
    QHBoxLayout *locationLayout = new QHBoxLayout(locationGroup);
    m_projectLocationEdit = new QLineEdit(dialog);
    m_projectLocationEdit->setPlaceholderText("Выберите директорию для проекта");
    m_browseButton = new QPushButton("Обзор...", dialog);
    locationLayout->addWidget(m_projectLocationEdit);
    locationLayout->addWidget(m_browseButton);
    mainLayout->addWidget(locationGroup);
    
    // Template info
    QLabel *templateLabel = new QLabel(QString("Шаблон: %1").arg(templateName), dialog);
    mainLayout->addWidget(templateLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    m_createProjectButton = new QPushButton("Создать", dialog);
    m_createProjectButton->setEnabled(false);
    m_cancelButton = new QPushButton("Отмена", dialog);
    buttonLayout->addWidget(m_createProjectButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);
    
    // Connect signals
    connect(m_browseButton, &QPushButton::clicked, this, [this, dialog]() {
        QString dir = QFileDialog::getExistingDirectory(dialog, "Выберите директорию",
                                                        QDir::homePath(),
                                                        QFileDialog::ShowDirsOnly);
        if (!dir.isEmpty()) {
            m_projectLocationEdit->setText(dir);
        }
    });
    
    connect(m_projectNameEdit, &QLineEdit::textChanged, this, [this]() {
        m_createProjectButton->setEnabled(!m_projectNameEdit->text().trimmed().isEmpty() &&
                                          !m_projectLocationEdit->text().trimmed().isEmpty());
    });
    
    connect(m_projectLocationEdit, &QLineEdit::textChanged, this, [this]() {
        m_createProjectButton->setEnabled(!m_projectNameEdit->text().trimmed().isEmpty() &&
                                          !m_projectLocationEdit->text().trimmed().isEmpty());
    });
    
    connect(m_cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);
    
    connect(m_createProjectButton, &QPushButton::clicked, this, [this, dialog, templateName]() {
        QString projectName = m_projectNameEdit->text().trimmed();
        QString location = m_projectLocationEdit->text().trimmed();
        
        if (m_projectCreator->createProject(projectName, location, templateName)) {
            QMessageBox::information(this, "Успех", 
                                     QString("Проект '%1' успешно создан в:\n%2")
                                     .arg(projectName).arg(location));
            dialog->accept();
            
            // Open the project directory in file tree
            m_currentProjectPath = location + "/" + projectName;
            m_fileSystemModel->setRootPath(m_currentProjectPath);
            m_fileTree->setRootIndex(m_fileSystemModel->index(m_currentProjectPath));
            m_fileTree->show();
            m_terminalPanel->show();
            
            // Switch to editor view
            m_stackedWidget->setCurrentWidget(m_codeEditor);
        } else {
            QMessageBox::critical(this, "Ошибка",
                                  QString("Не удалось создать проект:\n%1")
                                  .arg(m_projectCreator->lastError()));
        }
    });
    
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::goBack()
{
    showWelcomeScreen();
}

void MainWindow::showSettings()
{
    if (!m_settingsWindow) {
        m_settingsWindow = new SettingsWindow(this);
    }
    
    m_settingsWindow->exec();
    
    // Apply editor settings after dialog is closed
    applyEditorSettings();
}

void MainWindow::setupPackageManager()
{
    // Определяем директорию для пакетов
    QString packagesDir = QDir::home().absoluteFilePath(".codeeditor/packages");
    
    // Создаем и инициализируем менеджер пакетов
    m_packageManager = new PackageManager(this);
    if (!m_packageManager->initialize(packagesDir)) {
        qWarning() << "Failed to initialize package manager";
    }
    
    m_packageDialog = nullptr; // Будет создан по запросу
}

void MainWindow::showPackageManager()
{
    if (!m_packageDialog) {
        m_packageDialog = new PackageDialog(m_packageManager, this);
    }
    
    m_packageDialog->exec();
}

void MainWindow::applyEditorSettings()
{
    if (m_codeEditor && m_settingsWindow) {
        m_codeEditor->updateEditorSettings(
            m_settingsWindow->fontFamily(),
            m_settingsWindow->fontSize(),
            m_settingsWindow->textColor(),
            m_settingsWindow->highlightColor()
        );
    }
}

void MainWindow::newFile()
{
    // Switch to editor view
    m_stackedWidget->setCurrentWidget(m_codeEditor);
    
    // Clear current content
    if (m_codeEditor) {
        m_codeEditor->clear();
        m_codeEditor->setCurrentFilePath(QString());
        m_codeEditor->document()->setModified(false);
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                     tr("C++ Files (*.cpp *.h *.hpp);;All Files (*)"));
    if (!fileName.isEmpty()) {
        // Switch to editor view
        m_stackedWidget->setCurrentWidget(m_codeEditor);
        
        // Load the file
        if (m_codeEditor) {
            m_codeEditor->loadFile(fileName);
        }
    }
}

void MainWindow::saveFile()
{
    if (!m_codeEditor) return;
    
    if (m_codeEditor->currentFilePath().isEmpty()) {
        saveFileAs();
    } else {
        m_codeEditor->saveFile(m_codeEditor->currentFilePath());
    }
}

void MainWindow::saveFileAs()
{
    if (!m_codeEditor) return;
    
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), 
                                                     m_codeEditor->currentFilePath(),
                                                     tr("C++ Files (*.cpp *.h *.hpp);;All Files (*)"));
    if (!fileName.isEmpty()) {
        m_codeEditor->saveFile(fileName);
    }
}

void MainWindow::closeFile()
{
    if (!m_codeEditor) return;
    
    if (m_codeEditor->isModified()) {
        QMessageBox::StandardButton ret = QMessageBox::warning(
            this, tr("Unsaved Changes"),
            tr("The document has been modified.\nDo you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        
        if (ret == QMessageBox::Save) {
            saveFile();
        } else if (ret == QMessageBox::Cancel) {
            return;
        }
    }
    
    // Switch back to welcome screen
    showWelcomeScreen();
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, tr("About"), 
                       tr("<h2>Code Editor</h2>"
                          "<p>Version 1.0.0</p>"
                          "<p>A cross-platform code editor built with C++ and Qt.</p>"
                          "<p>&copy; 2024 MyCompany</p>"));
}
