#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class IndexOutOfRange : public std::exception {
private:
    std::string message;
public:
    /* Конструкторы */
    IndexOutOfRange() : message("Index out of range") {}
    explicit IndexOutOfRange(const std::string& msg) : message(msg) {} 
    
    const char* what() const noexcept override {
        return message.c_str(); // Возвращение текстового сообщения об ошибке
    }
};

#endif /* EXCEPTIONS_H */