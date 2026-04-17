#include "mainwindow.h"
#include "welcomescreen.h"
#include "projectselectionscreen.h"
#include "projectcreator.h"
#include "settings/settingsdialog.h"
#include "lang/LanguageManager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QGroupBox>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_settingsWindow(nullptr)
{
    setWindowTitle(LanguageManager::instance().translate("app_title"));
    setMinimumSize(800, 600);
    
    m_projectCreator = new ProjectCreator(this);
    setupProjectTemplates();
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
    
    m_settingsMenu = m_menuBar->addMenu(LanguageManager::instance().translate("settings_menu"));
    
    // Add Settings action
    QAction* settingsAction = new QAction(LanguageManager::instance().translate("settings_action"), this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);
    m_settingsMenu->addAction(settingsAction);
    
    m_settingsMenu->addSeparator();
    
    createLanguageMenu();
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
    m_stackedWidget = new QStackedWidget(this);
    setCentralWidget(m_stackedWidget);
    
    // Welcome Screen
    m_welcomeScreen = new WelcomeScreen();
    m_stackedWidget->addWidget(m_welcomeScreen);
    
    // Project Selection Screen
    m_projectSelectionScreen = new ProjectSelectionScreen();
    m_projectSelectionScreen->setProjectTemplates(m_projectTemplates);
    m_stackedWidget->addWidget(m_projectSelectionScreen);
    
    // Connect signals
    connect(m_welcomeScreen, &WelcomeScreen::startButtonClicked, 
            this, &MainWindow::showProjectSelectionScreen);
    
    connect(m_projectSelectionScreen, &ProjectSelectionScreen::createProjectClicked,
            this, &MainWindow::createProject);
    
    connect(m_projectSelectionScreen, &ProjectSelectionScreen::backButtonClicked,
            this, &MainWindow::goBack);
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
}
