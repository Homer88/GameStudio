#include "settings/settingsdialog.h"
#include "ui_settingsdialog.h"
#include <QSettings>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QSerialPortInfo>
#include <QMessageBox>

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
    , m_translator(new QTranslator(this))
{
    ui->setupUi(this);
    
    // Initialize temporary settings with current values
    loadSettings();
    
    // Populate UI elements
    populateLanguages();
    populateComPorts();
    
    // Apply current settings to UI
    updateInterfaceLanguage();
    
    // Connect signals
    connect(ui->languageComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &SettingsWindow::onLanguageChanged);
    connect(ui->fontPushButton, &QPushButton::clicked, this, &SettingsWindow::onFontChanged);
    connect(ui->textColorPushButton, &QPushButton::clicked, this, &SettingsWindow::onColorChanged);
    connect(ui->highlightColorPushButton, &QPushButton::clicked, this, &SettingsWindow::onColorChanged);
    connect(ui->comPortComboBox, QOverload<const QString&>::of(&QComboBox::currentTextChanged),
            this, &SettingsWindow::onComPortChanged);
    connect(ui->baudRateComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SettingsWindow::onBaudRateChanged);
    connect(ui->packagePathPushButton, &QPushButton::clicked, this, &SettingsWindow::onPackagePathChanged);
    connect(ui->modulePathPushButton, &QPushButton::clicked, this, &SettingsWindow::onModulePathChanged);
    connect(ui->saveButton, &QPushButton::clicked, this, &SettingsWindow::onSaveClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &SettingsWindow::onCancelClicked);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::loadSettings()
{
    QSettings config("conf/configure.xml", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
    
    // Language
    m_currentLanguage = config.value("General/Language", "auto").toString();
    
    // Fonts
    m_fontFamily = config.value("Editor/FontFamily", "Consolas").toString();
    m_fontSize = config.value("Editor/FontSize", 12).toInt();
    
    // Colors
    m_textColor = config.value("Editor/TextColor", QColor(Qt::black)).value<QColor>();
    m_highlightColor = config.value("Editor/HighlightColor", QColor(Qt::yellow)).value<QColor>();
    
    // COM Port
    m_comPort = config.value("Hardware/ComPort", "").toString();
    m_baudRate = config.value("Hardware/BaudRate", 9600).toInt();
    
    // Paths
    m_packagePath = config.value("Paths/PackagePath", "").toString();
    m_modulePath = config.value("Paths/ModulePath", "").toString();
}

void SettingsWindow::saveSettings()
{
    QSettings config("conf/configure.xml", QSettings::IniFormat);
    config.setIniCodec("UTF-8");
    
    // Save all settings
    config.setValue("General/Language", m_currentLanguage);
    config.setValue("Editor/FontFamily", m_fontFamily);
    config.setValue("Editor/FontSize", m_fontSize);
    config.setValue("Editor/TextColor", m_textColor);
    config.setValue("Editor/HighlightColor", m_highlightColor);
    config.setValue("Hardware/ComPort", m_comPort);
    config.setValue("Hardware/BaudRate", m_baudRate);
    config.setValue("Paths/PackagePath", m_packagePath);
    config.setValue("Paths/ModulePath", m_modulePath);
    
    config.sync();
}

void SettingsWindow::updateInterfaceLanguage()
{
    // Update UI elements based on current language
    ui->languageComboBox->setCurrentText(m_currentLanguage);
    
    // Update font display
    QFont font(m_fontFamily, m_fontSize);
    ui->fontLabel->setFont(font);
    ui->fontLabel->setText(QString("%1, %2").arg(m_fontFamily).arg(m_fontSize));
    
    // Update colors
    QPixmap colorPixmap(32, 32);
    colorPixmap.fill(m_textColor);
    ui->textColorPushButton->setIcon(QIcon(colorPixmap));
    
    colorPixmap.fill(m_highlightColor);
    ui->highlightColorPushButton->setIcon(QIcon(colorPixmap));
    
    // Update COM port
    ui->comPortComboBox->setCurrentText(m_comPort);
    
    // Update baud rate
    int baudIndex = ui->baudRateComboBox->findText(QString::number(m_baudRate));
    if (baudIndex >= 0) {
        ui->baudRateComboBox->setCurrentIndex(baudIndex);
    }
    
    // Update paths
    ui->packagePathLineEdit->setText(m_packagePath);
    ui->modulePathLineEdit->setText(m_modulePath);
}

void SettingsWindow::populateLanguages()
{
    ui->languageComboBox->clear();
    ui->languageComboBox->addItem(tr("Auto (System)"), "auto");
    ui->languageComboBox->addItem("English", "en");
    ui->languageComboBox->addItem("Русский", "ru");
    // Add more languages as needed
}

void SettingsWindow::populateComPorts()
{
    ui->comPortComboBox->clear();
    ui->comPortComboBox->addItem(""); // Empty option
    
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        ui->comPortComboBox->addItem(port.portName(), port.systemLocation());
    }
}

void SettingsWindow::onLanguageChanged(const QString &language)
{
    m_currentLanguage = language;
    // Language will be applied when settings are saved
}

void SettingsWindow::onFontChanged()
{
    bool ok;
    QFont currentFont(m_fontFamily, m_fontSize);
    QFont newFont = QFontDialog::getFont(&ok, currentFont, this, tr("Select Font"));
    
    if (ok) {
        m_fontFamily = newFont.family();
        m_fontSize = newFont.pointSize();
        updateInterfaceLanguage();
    }
}

void SettingsWindow::onColorChanged()
{
    QObject *sender = QObject::sender();
    QColor currentColor;
    QString title;
    
    if (sender == ui->textColorPushButton) {
        currentColor = m_textColor;
        title = tr("Select Text Color");
    } else if (sender == ui->highlightColorPushButton) {
        currentColor = m_highlightColor;
        title = tr("Select Highlight Color");
    } else {
        return;
    }
    
    QColor newColor = QColorDialog::getColor(currentColor, this, title);
    
    if (newColor.isValid()) {
        if (sender == ui->textColorPushButton) {
            m_textColor = newColor;
        } else {
            m_highlightColor = newColor;
        }
        updateInterfaceLanguage();
    }
}

void SettingsWindow::onComPortChanged(const QString &port)
{
    m_comPort = port;
}

void SettingsWindow::onBaudRateChanged(int index)
{
    m_baudRate = ui->baudRateComboBox->itemText(index).toInt();
}

void SettingsWindow::onPackagePathChanged()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Package Directory"),
                                                    m_packagePath);
    if (!dir.isEmpty()) {
        m_packagePath = dir;
        ui->packagePathLineEdit->setText(dir);
    }
}

void SettingsWindow::onModulePathChanged()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Module Directory"),
                                                    m_modulePath);
    if (!dir.isEmpty()) {
        m_modulePath = dir;
        ui->modulePathLineEdit->setText(dir);
    }
}

void SettingsWindow::onSaveClicked()
{
    saveSettings();
    
    QMessageBox::information(this, tr("Settings Saved"), 
                            tr("Settings have been saved successfully.\nSome changes may require restart."));
    accept();
}

void SettingsWindow::onCancelClicked()
{
    reject();
}
