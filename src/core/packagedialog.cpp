#include "packagedialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QSplitter>

PackageDialog::PackageDialog(PackageManager *packageManager, QWidget *parent)
    : QDialog(parent), m_packageManager(packageManager)
{
    setupUi();
    
    // Подключаем сигналы менеджера пакетов
    connect(m_packageManager, &PackageManager::installationProgress,
            this, &PackageDialog::onInstallationProgress);
    connect(m_packageManager, &PackageManager::packageInstalled,
            this, &PackageDialog::onPackageInstalled);
    connect(m_packageManager, &PackageManager::packageUninstalled,
            this, &PackageDialog::onPackageUninstalled);
    connect(m_packageManager, &PackageManager::errorOccurred,
            this, &PackageDialog::onErrorOccurred);
    
    refreshPackageList();
}

void PackageDialog::setupUi()
{
    setWindowTitle(tr("Менеджер пакетов"));
    resize(800, 600);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Создаем сплиттер для разделения списка и деталей
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    
    // Левая панель - список пакетов
    QWidget *leftPanel = new QWidget(splitter);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    
    m_packageList = new QListWidget(leftPanel);
    leftLayout->addWidget(m_packageList);
    
    // Кнопки управления
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_installButton = new QPushButton(tr("Установить пакет..."), leftPanel);
    m_uninstallButton = new QPushButton(tr("Удалить пакет"), leftPanel);
    m_uninstallButton->setEnabled(false);
    
    buttonLayout->addWidget(m_installButton);
    buttonLayout->addWidget(m_uninstallButton);
    leftLayout->addLayout(buttonLayout);
    
    // Правая панель - информация о пакете
    QWidget *rightPanel = new QWidget(splitter);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    
    QGroupBox *detailsGroup = new QGroupBox(tr("Информация о пакете"), rightPanel);
    QVBoxLayout *detailsLayout = new QVBoxLayout(detailsGroup);
    
    m_packageNameLabel = new QLabel(tr("Имя: -"), detailsGroup);
    m_packageVersionLabel = new QLabel(tr("Версия: -"), detailsGroup);
    m_packagePathLabel = new QLabel(tr("Путь: -"), detailsGroup);
    m_packageInstallDateLabel = new QLabel(tr("Дата установки: -"), detailsGroup);
    m_packageIncludeLabel = new QLabel(tr("Заголовки: -"), detailsGroup);
    m_packageLibLabel = new QLabel(tr("Библиотеки: -"), detailsGroup);
    m_packageCompilerLabel = new QLabel(tr("Компилятор: -"), detailsGroup);
    m_packageHelpLabel = new QLabel(tr("Документация: -"), detailsGroup);
    
    detailsLayout->addWidget(m_packageNameLabel);
    detailsLayout->addWidget(m_packageVersionLabel);
    detailsLayout->addWidget(m_packagePathLabel);
    detailsLayout->addWidget(m_packageInstallDateLabel);
    detailsLayout->addWidget(m_packageIncludeLabel);
    detailsLayout->addWidget(m_packageLibLabel);
    detailsLayout->addWidget(m_packageCompilerLabel);
    detailsLayout->addWidget(m_packageHelpLabel);
    detailsLayout->addStretch();
    
    rightLayout->addWidget(detailsGroup);
    
    splitter->addWidget(leftPanel);
    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);
    
    mainLayout->addWidget(splitter);
    
    // Прогресс бар и статус
    QHBoxLayout *progressLayout = new QHBoxLayout();
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    m_statusLabel = new QLabel(tr("Готов"), this);
    
    progressLayout->addWidget(m_progressBar);
    progressLayout->addWidget(m_statusLabel);
    mainLayout->addLayout(progressLayout);
    
    // Кнопка закрытия
    QHBoxLayout *closeLayout = new QHBoxLayout();
    closeLayout->addStretch();
    m_closeButton = new QPushButton(tr("Закрыть"), this);
    closeLayout->addWidget(m_closeButton);
    mainLayout->addLayout(closeLayout);
    
    // Подключаем сигналы кнопок
    connect(m_installButton, &QPushButton::clicked, this, &PackageDialog::onInstallPackage);
    connect(m_uninstallButton, &QPushButton::clicked, this, &PackageDialog::onUninstallPackage);
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_packageList, &QListWidget::currentItemChanged,
            this, &PackageDialog::onPackageSelected);
}

void PackageDialog::refreshPackageList()
{
    m_packageList->clear();
    
    QList<InstalledPackage> packages = m_packageManager->getInstalledPackages();
    for (const InstalledPackage &pkg : packages) {
        QListWidgetItem *item = new QListWidgetItem(pkg.name + " (" + pkg.version + ")");
        item->setData(Qt::UserRole, pkg.id);
        if (!pkg.isActive) {
            item->setForeground(QColor(Qt::gray));
        }
        m_packageList->addItem(item);
    }
    
    if (m_packageList->count() > 0) {
        m_packageList->setCurrentRow(0);
    }
}

void PackageDialog::updatePackageDetails(const QString &packageId)
{
    InstalledPackage pkg = m_packageManager->getPackageInfo(packageId);
    
    if (pkg.id.isEmpty()) {
        m_packageNameLabel->setText(tr("Имя: -"));
        m_packageVersionLabel->setText(tr("Версия: -"));
        m_packagePathLabel->setText(tr("Путь: -"));
        m_packageInstallDateLabel->setText(tr("Дата установки: -"));
        m_packageIncludeLabel->setText(tr("Заголовки: -"));
        m_packageLibLabel->setText(tr("Библиотеки: -"));
        m_packageCompilerLabel->setText(tr("Компилятор: -"));
        m_packageHelpLabel->setText(tr("Документация: -"));
        m_uninstallButton->setEnabled(false);
        return;
    }
    
    m_packageNameLabel->setText(tr("Имя: %1").arg(pkg.name));
    m_packageVersionLabel->setText(tr("Версия: %1").arg(pkg.version));
    m_packagePathLabel->setText(tr("Путь: %1").arg(pkg.installPath));
    m_packageInstallDateLabel->setText(tr("Дата установки: %1")
        .arg(pkg.installDate.toString("dd.MM.yyyy hh:mm")));
    m_packageIncludeLabel->setText(tr("Заголовки: %1")
        .arg(pkg.includePath.isEmpty() ? tr("-") : pkg.includePath));
    m_packageLibLabel->setText(tr("Библиотеки: %1")
        .arg(pkg.libPath.isEmpty() ? tr("-") : pkg.libPath));
    m_packageCompilerLabel->setText(tr("Компилятор: %1")
        .arg(pkg.compilerPath.isEmpty() ? tr("-") : pkg.compilerPath));
    m_packageHelpLabel->setText(tr("Документация: %1")
        .arg(pkg.helpPath.isEmpty() ? tr("-") : pkg.helpPath));
    
    m_uninstallButton->setEnabled(true);
}

void PackageDialog::onInstallPackage()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Выберите пакет для установки"),
        QString(),
        tr("Пакеты (*.zip);;Все файлы (*)")
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    m_progressBar->setVisible(true);
    m_progressBar->setValue(0);
    m_statusLabel->setText(tr("Установка пакета..."));
    m_installButton->setEnabled(false);
    m_uninstallButton->setEnabled(false);
    
    QString error;
    bool success = m_packageManager->installPackage(fileName, error);
    
    if (success) {
        refreshPackageList();
        m_statusLabel->setText(tr("Пакет успешно установлен"));
    } else {
        QMessageBox::critical(this, tr("Ошибка установки"), error);
        m_statusLabel->setText(tr("Ошибка установки"));
    }
    
    m_progressBar->setVisible(false);
    m_installButton->setEnabled(true);
    m_uninstallButton->setEnabled(m_packageList->currentItem() != nullptr);
}

void PackageDialog::onUninstallPackage()
{
    QListWidgetItem *currentItem = m_packageList->currentItem();
    if (!currentItem) {
        return;
    }
    
    QString packageId = currentItem->data(Qt::UserRole).toString();
    InstalledPackage pkg = m_packageManager->getPackageInfo(packageId);
    
    int result = QMessageBox::question(
        this,
        tr("Подтверждение удаления"),
        tr("Вы действительно хотите удалить пакет \"%1\" версии %2?")
            .arg(pkg.name, pkg.version),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );
    
    if (result != QMessageBox::Yes) {
        return;
    }
    
    m_progressBar->setVisible(true);
    m_progressBar->setValue(50);
    m_statusLabel->setText(tr("Удаление пакета..."));
    m_installButton->setEnabled(false);
    m_uninstallButton->setEnabled(false);
    
    QString error;
    bool success = m_packageManager->uninstallPackage(packageId, error);
    
    if (success) {
        refreshPackageList();
        m_statusLabel->setText(tr("Пакет успешно удален"));
    } else {
        QMessageBox::critical(this, tr("Ошибка удаления"), error);
        m_statusLabel->setText(tr("Ошибка удаления"));
    }
    
    m_progressBar->setVisible(false);
    m_installButton->setEnabled(true);
    m_uninstallButton->setEnabled(m_packageList->currentItem() != nullptr);
}

void PackageDialog::onPackageSelected()
{
    QListWidgetItem *currentItem = m_packageList->currentItem();
    if (currentItem) {
        QString packageId = currentItem->data(Qt::UserRole).toString();
        updatePackageDetails(packageId);
    } else {
        updatePackageDetails(QString());
    }
}

void PackageDialog::onInstallationProgress(int percent, const QString &message)
{
    m_progressBar->setValue(percent);
    m_statusLabel->setText(message);
}

void PackageDialog::onPackageInstalled(const QString &packageId)
{
    Q_UNUSED(packageId);
    // Обновление уже выполнено в onInstallPackage
}

void PackageDialog::onPackageUninstalled(const QString &packageId)
{
    Q_UNUSED(packageId);
    // Обновление уже выполнено в onUninstallPackage
}

void PackageDialog::onErrorOccurred(const QString &error)
{
    QMessageBox::critical(this, tr("Ошибка"), error);
    m_statusLabel->setText(tr("Ошибка"));
}
