#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class WelcomeScreen : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);
    ~WelcomeScreen() = default;

signals:
    void startButtonClicked();

private:
    void setupUI();
    
    QVBoxLayout *m_mainLayout;
    QLabel *m_titleLabel;
    QLabel *m_subtitleLabel;
    QPushButton *m_startButton;
};

#endif // WELCOMESCREEN_H
