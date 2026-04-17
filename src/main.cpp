#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Устанавливаем информацию о приложении
    QApplication::setApplicationName("Qt Project Manager");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("MyCompany");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
