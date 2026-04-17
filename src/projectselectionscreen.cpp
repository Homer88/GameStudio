#include "projectselectionscreen.h"

ProjectSelectionScreen::ProjectSelectionScreen(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void ProjectSelectionScreen::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title Label
    m_titleLabel = new QLabel("Выберите тип проекта", this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);

    // Project List
    m_projectList = new QListWidget(this);
    m_projectList->setMinimumHeight(200);
    m_projectList->setSelectionMode(QAbstractItemView::SingleSelection);

    // Create Button
    m_createButton = new QPushButton("Создать проект", this);
    m_createButton->setMinimumSize(180, 45);
    m_createButton->setEnabled(false);

    // Back Button
    m_backButton = new QPushButton("Назад", this);
    m_backButton->setMinimumSize(180, 45);

    // Add widgets to layout
    m_mainLayout->addWidget(m_titleLabel);
    m_mainLayout->addWidget(m_projectList);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_createButton);
    buttonLayout->addWidget(m_backButton);
    buttonLayout->addStretch();
    m_mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(m_projectList, &QListWidget::itemSelectionChanged, this, [this]() {
        m_createButton->setEnabled(!m_projectList->selectedItems().isEmpty());
    });

    connect(m_createButton, &QPushButton::clicked, this, [this]() {
        if (!m_projectList->currentItem()) return;
        emit createProjectClicked(m_projectList->currentItem()->text());
    });

    connect(m_backButton, &QPushButton::clicked, this, &ProjectSelectionScreen::backButtonClicked);
}

void ProjectSelectionScreen::setProjectTemplates(const QStringList &templates)
{
    m_projectList->clear();
    m_projectList->addItems(templates);
}

QString ProjectSelectionScreen::selectedTemplate() const
{
    if (m_projectList->currentItem()) {
        return m_projectList->currentItem()->text();
    }
    return QString();
}
