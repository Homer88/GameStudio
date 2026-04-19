/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsWindow
{
public:
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QWidget *generalTab;
    QFormLayout *generalLayout;
    QLabel *languageLabel;
    QComboBox *languageComboBox;
    QWidget *editorTab;
    QFormLayout *editorLayout;
    QLabel *fontLabel_title;
    QHBoxLayout *fontLayout;
    QLabel *fontLabel;
    QPushButton *fontPushButton;
    QLabel *textColorLabel;
    QPushButton *textColorPushButton;
    QLabel *highlightColorLabel;
    QPushButton *highlightColorPushButton;
    QWidget *hardwareTab;
    QFormLayout *hardwareLayout;
    QLabel *comPortLabel;
    QComboBox *comPortComboBox;
    QLabel *baudRateLabel;
    QComboBox *baudRateComboBox;
    QWidget *packagesTab;
    QFormLayout *packagesLayout;
    QLabel *packagePathLabel;
    QHBoxLayout *packagePathLayout;
    QLineEdit *packagePathLineEdit;
    QPushButton *packagePathPushButton;
    QWidget *pathsTab;
    QFormLayout *pathsLayout;
    QLabel *modulePathLabel;
    QHBoxLayout *modulePathLayout;
    QLineEdit *modulePathLineEdit;
    QPushButton *modulePathPushButton;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *buttonLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *SettingsWindow)
    {
        if (SettingsWindow->objectName().isEmpty())
            SettingsWindow->setObjectName(QString::fromUtf8("SettingsWindow"));
        SettingsWindow->resize(600, 500);
        verticalLayout = new QVBoxLayout(SettingsWindow);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(SettingsWindow);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        generalTab = new QWidget();
        generalTab->setObjectName(QString::fromUtf8("generalTab"));
        generalLayout = new QFormLayout(generalTab);
        generalLayout->setObjectName(QString::fromUtf8("generalLayout"));
        languageLabel = new QLabel(generalTab);
        languageLabel->setObjectName(QString::fromUtf8("languageLabel"));

        generalLayout->setWidget(0, QFormLayout::LabelRole, languageLabel);

        languageComboBox = new QComboBox(generalTab);
        languageComboBox->setObjectName(QString::fromUtf8("languageComboBox"));

        generalLayout->setWidget(0, QFormLayout::FieldRole, languageComboBox);

        tabWidget->addTab(generalTab, QString());
        editorTab = new QWidget();
        editorTab->setObjectName(QString::fromUtf8("editorTab"));
        editorLayout = new QFormLayout(editorTab);
        editorLayout->setObjectName(QString::fromUtf8("editorLayout"));
        fontLabel_title = new QLabel(editorTab);
        fontLabel_title->setObjectName(QString::fromUtf8("fontLabel_title"));

        editorLayout->setWidget(0, QFormLayout::LabelRole, fontLabel_title);

        fontLayout = new QHBoxLayout();
        fontLayout->setObjectName(QString::fromUtf8("fontLayout"));
        fontLabel = new QLabel(editorTab);
        fontLabel->setObjectName(QString::fromUtf8("fontLabel"));

        fontLayout->addWidget(fontLabel);

        fontPushButton = new QPushButton(editorTab);
        fontPushButton->setObjectName(QString::fromUtf8("fontPushButton"));

        fontLayout->addWidget(fontPushButton);


        editorLayout->setLayout(0, QFormLayout::FieldRole, fontLayout);

        textColorLabel = new QLabel(editorTab);
        textColorLabel->setObjectName(QString::fromUtf8("textColorLabel"));

        editorLayout->setWidget(1, QFormLayout::LabelRole, textColorLabel);

        textColorPushButton = new QPushButton(editorTab);
        textColorPushButton->setObjectName(QString::fromUtf8("textColorPushButton"));

        editorLayout->setWidget(1, QFormLayout::FieldRole, textColorPushButton);

        highlightColorLabel = new QLabel(editorTab);
        highlightColorLabel->setObjectName(QString::fromUtf8("highlightColorLabel"));

        editorLayout->setWidget(2, QFormLayout::LabelRole, highlightColorLabel);

        highlightColorPushButton = new QPushButton(editorTab);
        highlightColorPushButton->setObjectName(QString::fromUtf8("highlightColorPushButton"));

        editorLayout->setWidget(2, QFormLayout::FieldRole, highlightColorPushButton);

        tabWidget->addTab(editorTab, QString());
        hardwareTab = new QWidget();
        hardwareTab->setObjectName(QString::fromUtf8("hardwareTab"));
        hardwareLayout = new QFormLayout(hardwareTab);
        hardwareLayout->setObjectName(QString::fromUtf8("hardwareLayout"));
        comPortLabel = new QLabel(hardwareTab);
        comPortLabel->setObjectName(QString::fromUtf8("comPortLabel"));

        hardwareLayout->setWidget(0, QFormLayout::LabelRole, comPortLabel);

        comPortComboBox = new QComboBox(hardwareTab);
        comPortComboBox->setObjectName(QString::fromUtf8("comPortComboBox"));

        hardwareLayout->setWidget(0, QFormLayout::FieldRole, comPortComboBox);

        baudRateLabel = new QLabel(hardwareTab);
        baudRateLabel->setObjectName(QString::fromUtf8("baudRateLabel"));

        hardwareLayout->setWidget(1, QFormLayout::LabelRole, baudRateLabel);

        baudRateComboBox = new QComboBox(hardwareTab);
        baudRateComboBox->addItem(QString());
        baudRateComboBox->addItem(QString());
        baudRateComboBox->addItem(QString());
        baudRateComboBox->addItem(QString());
        baudRateComboBox->addItem(QString());
        baudRateComboBox->setObjectName(QString::fromUtf8("baudRateComboBox"));

        hardwareLayout->setWidget(1, QFormLayout::FieldRole, baudRateComboBox);

        tabWidget->addTab(hardwareTab, QString());
        packagesTab = new QWidget();
        packagesTab->setObjectName(QString::fromUtf8("packagesTab"));
        packagesLayout = new QFormLayout(packagesTab);
        packagesLayout->setObjectName(QString::fromUtf8("packagesLayout"));
        packagePathLabel = new QLabel(packagesTab);
        packagePathLabel->setObjectName(QString::fromUtf8("packagePathLabel"));

        packagesLayout->setWidget(0, QFormLayout::LabelRole, packagePathLabel);

        packagePathLayout = new QHBoxLayout();
        packagePathLayout->setObjectName(QString::fromUtf8("packagePathLayout"));
        packagePathLineEdit = new QLineEdit(packagesTab);
        packagePathLineEdit->setObjectName(QString::fromUtf8("packagePathLineEdit"));

        packagePathLayout->addWidget(packagePathLineEdit);

        packagePathPushButton = new QPushButton(packagesTab);
        packagePathPushButton->setObjectName(QString::fromUtf8("packagePathPushButton"));

        packagePathLayout->addWidget(packagePathPushButton);


        packagesLayout->setLayout(0, QFormLayout::FieldRole, packagePathLayout);

        tabWidget->addTab(packagesTab, QString());
        pathsTab = new QWidget();
        pathsTab->setObjectName(QString::fromUtf8("pathsTab"));
        pathsLayout = new QFormLayout(pathsTab);
        pathsLayout->setObjectName(QString::fromUtf8("pathsLayout"));
        modulePathLabel = new QLabel(pathsTab);
        modulePathLabel->setObjectName(QString::fromUtf8("modulePathLabel"));

        pathsLayout->setWidget(0, QFormLayout::LabelRole, modulePathLabel);

        modulePathLayout = new QHBoxLayout();
        modulePathLayout->setObjectName(QString::fromUtf8("modulePathLayout"));
        modulePathLineEdit = new QLineEdit(pathsTab);
        modulePathLineEdit->setObjectName(QString::fromUtf8("modulePathLineEdit"));

        modulePathLayout->addWidget(modulePathLineEdit);

        modulePathPushButton = new QPushButton(pathsTab);
        modulePathPushButton->setObjectName(QString::fromUtf8("modulePathPushButton"));

        modulePathLayout->addWidget(modulePathPushButton);


        pathsLayout->setLayout(0, QFormLayout::FieldRole, modulePathLayout);

        tabWidget->addTab(pathsTab, QString());

        verticalLayout->addWidget(tabWidget);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName(QString::fromUtf8("buttonLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        buttonLayout->addItem(horizontalSpacer);

        saveButton = new QPushButton(SettingsWindow);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));

        buttonLayout->addWidget(saveButton);

        cancelButton = new QPushButton(SettingsWindow);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        buttonLayout->addWidget(cancelButton);


        verticalLayout->addLayout(buttonLayout);


        retranslateUi(SettingsWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(SettingsWindow);
    } // setupUi

    void retranslateUi(QDialog *SettingsWindow)
    {
        SettingsWindow->setWindowTitle(QCoreApplication::translate("SettingsWindow", "Settings", nullptr));
        languageLabel->setText(QCoreApplication::translate("SettingsWindow", "Language:", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(generalTab), QCoreApplication::translate("SettingsWindow", "General", nullptr));
        fontLabel_title->setText(QCoreApplication::translate("SettingsWindow", "Font:", nullptr));
        fontLabel->setText(QCoreApplication::translate("SettingsWindow", "Consolas, 12", nullptr));
        fontPushButton->setText(QCoreApplication::translate("SettingsWindow", "Change...", nullptr));
        textColorLabel->setText(QCoreApplication::translate("SettingsWindow", "Text Color:", nullptr));
        textColorPushButton->setText(QString());
        highlightColorLabel->setText(QCoreApplication::translate("SettingsWindow", "Highlight Color:", nullptr));
        highlightColorPushButton->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(editorTab), QCoreApplication::translate("SettingsWindow", "Editor", nullptr));
        comPortLabel->setText(QCoreApplication::translate("SettingsWindow", "COM Port:", nullptr));
        baudRateLabel->setText(QCoreApplication::translate("SettingsWindow", "Baud Rate:", nullptr));
        baudRateComboBox->setItemText(0, QCoreApplication::translate("SettingsWindow", "9600", nullptr));
        baudRateComboBox->setItemText(1, QCoreApplication::translate("SettingsWindow", "19200", nullptr));
        baudRateComboBox->setItemText(2, QCoreApplication::translate("SettingsWindow", "38400", nullptr));
        baudRateComboBox->setItemText(3, QCoreApplication::translate("SettingsWindow", "57600", nullptr));
        baudRateComboBox->setItemText(4, QCoreApplication::translate("SettingsWindow", "115200", nullptr));

        tabWidget->setTabText(tabWidget->indexOf(hardwareTab), QCoreApplication::translate("SettingsWindow", "Hardware", nullptr));
        packagePathLabel->setText(QCoreApplication::translate("SettingsWindow", "Package Directory:", nullptr));
        packagePathPushButton->setText(QCoreApplication::translate("SettingsWindow", "Browse...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(packagesTab), QCoreApplication::translate("SettingsWindow", "Packages", nullptr));
        modulePathLabel->setText(QCoreApplication::translate("SettingsWindow", "Module Directory:", nullptr));
        modulePathPushButton->setText(QCoreApplication::translate("SettingsWindow", "Browse...", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(pathsTab), QCoreApplication::translate("SettingsWindow", "Paths", nullptr));
        saveButton->setText(QCoreApplication::translate("SettingsWindow", "Save", nullptr));
        cancelButton->setText(QCoreApplication::translate("SettingsWindow", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsWindow: public Ui_SettingsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
