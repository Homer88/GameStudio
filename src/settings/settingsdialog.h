#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QTranslator>
#include "language_manager.h"

namespace Ui { class SettingsWindow; }

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    // Getters for editor settings
    QString fontFamily() const { return m_fontFamily; }
    int fontSize() const { return m_fontSize; }
    QColor textColor() const { return m_textColor; }
    QColor highlightColor() const { return m_highlightColor; }

private slots:
    void onLanguageChanged(const QString &language);
    void onFontChanged();
    void onColorChanged();
    void onComPortChanged(const QString &port);
    void onBaudRateChanged(int rate);
    void onPackagePathChanged();
    void onModulePathChanged();
    void onSaveClicked();
    void onCancelClicked();

private:
    void loadSettings();
    void saveSettings();
    void updateInterfaceLanguage();
    void populateComPorts();
    void populateLanguages();

    Ui::SettingsWindow *ui;
    QTranslator *m_translator;
    
    // Temporary settings storage
    QString m_currentLanguage;
    QString m_fontFamily;
    int m_fontSize;
    QColor m_textColor;
    QColor m_highlightColor;
    QString m_comPort;
    int m_baudRate;
    QString m_packagePath;
    QString m_modulePath;
};

#endif // SETTINGSWINDOW_H
