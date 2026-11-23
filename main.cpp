#include <iostream>

#include "headers/CorrectnessTests.hpp"
#include "headers/PerformanceTests.hpp"
#include "headers/Console.hpp"

using alphIndex = DynamicArray<DynamicArray<std::string>>;
using Dict      = HashDictionary<std::string, int>;
using Entry     = KeyValue<std::string, DynamicArray<int>>;

int main() { 

    std::cout << "АЛФАВИТНЫЙ УКАЗАТЕЛЬ\n";

    ClearScreen(); 

    while(true) {
        std::cout << "Главное меню:\n"
                  << "1) Ручной режим\n"
                  << "2) Тесты корректности\n"
                  << "3) Тесты производительности\n"
                  << "0) Выход\n"
                  << "Выберите пункт: ";

        int main_choice;
        if (!(std::cin >> main_choice)) {
            ClearInputBuffer();
            std::cout << "Неверный ввод.\n";
            WaitingForEnter();
            continue;
        }

        ClearInputBuffer();

        std::string text;     
        std::string separation; 
        int pageSize; 
        alphIndex pages;    

        switch (main_choice) {

        case 1: 
 
            std::cout << "Введите текст:\n";
            std::getline(std::cin, text); // Считываем всю строку до Enter

            std::cout << "Введите способ разбиения ('символ' или 'слово'))\n";
            std::cin  >> separation;

            if(separation == "символ") {
                std::cout << "Введите количество символов на стандартной странице\n";
                std::cin  >> pageSize; 

                pages = SplitIntoPages(text, pageSize, PageUnit::Characters);

            } else if(separation == "слово") {
                std::cout << "Введите количество слов на стандартной странице\n";
                std::cin  >> pageSize; 

                pages = SplitIntoPages(text, pageSize, PageUnit::Words);

            } else {
                std::cout << "Неверный вид разбиения.\n";
                ClearInputBuffer();
                WaitingForEnter();    
                break;            
            }

            std::cout << "Введенный текст разбит на страницы!\n";

            for(int i = 0; i < pages.GetLength(); i++) {
                std::cout << "Страница [" << (i + 1) << "]: ";
                for(int j = 0; j < pages[i].GetLength(); j++){
                    std::cout << pages[i][j] << " ";
                }
                std::cout << "\n";
            }

            ClearInputBuffer();
            WaitingForEnter();
            break;

        case 2:
            ClearScreen();
            RunAllCorrectnessTests();
            WaitingForEnter();
            break;
        
        case 3: 
            break;
        
        case 0:
            return 0;
        
        default: 
            std::cout << "Неверный пункт меню.\n";
            WaitingForEnter();
            return 0;
        }
    }
}
