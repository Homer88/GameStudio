#ifndef PACKAGEDIALOG_H
#define PACKAGEDIALOG_H

#include <QDialog>
#include <QString>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include "core/packagemanager.h"

class PackageDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PackageDialog(PackageManager *packageManager, QWidget *parent = nullptr);
    
private slots:
    void onInstallPackage();
    void onUninstallPackage();
    void onPackageSelected();
    void onInstallationProgress(int percent, const QString &message);
    void onPackageInstalled(const QString &packageId);
    void onPackageUninstalled(const QString &packageId);
    void onErrorOccurred(const QString &error);
    
private:
    void setupUi();
    void refreshPackageList();
    void updatePackageDetails(const QString &packageId);
    
    PackageManager *m_packageManager;
    
    // Элементы интерфейса
    QListWidget *m_packageList;
    QLabel *m_packageNameLabel;
    QLabel *m_packageVersionLabel;
    QLabel *m_packagePathLabel;
    QLabel *m_packageInstallDateLabel;
    QLabel *m_packageIncludeLabel;
    QLabel *m_packageLibLabel;
    QLabel *m_packageCompilerLabel;
    QLabel *m_packageHelpLabel;
    QPushButton *m_installButton;
    QPushButton *m_uninstallButton;
    QPushButton *m_closeButton;
    QProgressBar *m_progressBar;
    QLabel *m_statusLabel;
};

#endif // PACKAGEDIALOG_H
