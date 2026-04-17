#include "welcomescreen.h"
#include "language_manager.h"
#include <QHBoxLayout>

WelcomeScreen::WelcomeScreen(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
    updateLanguage();
}

void WelcomeScreen::updateLanguage()
{
    LanguageManager& langMgr = LanguageManager::instance();
    
    m_titleLabel->setText(langMgr.translate("welcome_title"));
    m_subtitleLabel->setText(langMgr.translate("welcome_subtitle"));
    m_startButton->setText(langMgr.translate("start_button"));
}

void WelcomeScreen::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(40, 40, 40, 40);

    // Title Label
    m_titleLabel = new QLabel("", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    // Subtitle Label
    m_subtitleLabel = new QLabel("", this);
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    QFont subtitleFont = m_subtitleLabel->font();
    subtitleFont.setPointSize(14);
    m_subtitleLabel->setFont(subtitleFont);

    // Start Button
    m_startButton = new QPushButton("", this);
    m_startButton->setMinimumSize(200, 50);
    QFont buttonFont = m_startButton->font();
    buttonFont.setPointSize(12);
    m_startButton->setFont(buttonFont);

    // Add stretch to center content vertically
    m_mainLayout->addStretch();
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_subtitleLabel);
    m_mainLayout->addStretch();
    
    // Center the button horizontally
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_startButton);
    buttonLayout->addStretch();
    m_mainLayout->addLayout(buttonLayout);
    
    m_mainLayout->addStretch();

    // Connect signal
    connect(m_startButton, &QPushButton::clicked, this, &WelcomeScreen::startButtonClicked);
}
