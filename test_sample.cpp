// Пример кода C++ для проверки подсветки синтаксиса
#include <iostream>
#include <QString>

class MyClass {
public:
    MyClass() : m_value(0) {}
    
    int getValue() const {
        return m_value;
    }
    
    void setValue(int value) {
        if (value >= 0) {
            m_value = value;
        }
    }
    
private:
    int m_value;
};

int main() {
    MyClass obj;
    obj.setValue(42);
    
    // Это комментарий
    std::cout << "Value: " << obj.getValue() << std::endl;
    
    /* Многострочный
       комментарий */
    
    QString text = "Hello, World!";
    double pi = 3.14159;
    
    return 0;
}
