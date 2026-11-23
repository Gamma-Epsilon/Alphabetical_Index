#ifndef CONSOLE_CPP
#define CONSOLE_CPP

#include <iostream>
#include <limits>


/* Очистка консоли */
void ClearScreen() {
    std::cout.flush(); 
    std::system("cls");
}

/* Сброс остатка ввода до конца строки */
void ClearInputBuffer() {
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/* Ожидание Enter */
void WaitingForEnter() {
    std::cout << "Нажмите Enter для продолжения..."; 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ClearScreen();
}


#endif /* CONSOLE_CPP */