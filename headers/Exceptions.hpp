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


class KeyDoesNotExist : public std::exception {
private:
    std::string message;
public:
    /* Конструкторы */
    KeyDoesNotExist() : message("The key is not in a dictionary") {}
    explicit KeyDoesNotExist(const std::string& msg) : message(msg) {} 
    
    const char* what() const noexcept override {
        return message.c_str(); // Возвращение текстового сообщения об ошибке
    }
};


class KeyAlreadyExists : public std::exception {
private:
    std::string message;
public:
    /* Конструкторы */
    KeyAlreadyExists() : message("The key already is in a dictionary") {}
    explicit KeyAlreadyExists(const std::string& msg) : message(msg) {} 
    
    const char* what() const noexcept override {
        return message.c_str(); // Возвращение текстового сообщения об ошибке
    }
};


class WordIsAbsent : public std::exception {
private:
    std::string message;
public:
    /* Конструкторы */
    WordIsAbsent() : message("The key already is in a dictionary") {}
    explicit WordIsAbsent(const std::string& msg) : message(msg) {} 
    
    const char* what() const noexcept override {
        return message.c_str(); // Возвращение текстового сообщения об ошибке
    }
};


#endif /* EXCEPTIONS_H */