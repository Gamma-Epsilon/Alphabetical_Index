#include <iostream>

#include "headers/CorrectnessTests.hpp"
#include "headers/PerformanceTests.hpp"
#include "headers/Console.hpp"

using alphIndex = DynamicArray<DynamicArray<std::string>>;
using Dict      = HashDictionary<std::string, int>;
using Entry     = KeyValue<std::string, DynamicArray<int>>;

int main() { 

    ClearScreen(); 

    while(true) {
        std::cout << "АЛФАВИТНЫЙ УКАЗАТЕЛЬ\n";
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
        
        int indexChoice;
        int bucketCount;
        switch (main_choice) {

        case 1: 
 
            std::cout << "Введите текст:\n";
            std::getline(std::cin, text); // Считываем всю строку до Enter

            std::cout << "Введите способ разбиения ('символ' или 'слово')): ";
            std::cin  >> separation;

            if(separation == "символ") {
                std::cout << "Введите количество символов на стандартной странице: ";
                std::cin  >> pageSize; 

                pages = SplitIntoPages(text, pageSize, PageUnit::Characters);

            } else if(separation == "слово") {
                std::cout << "Введите количество слов на стандартной странице: ";
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

            std::cout << "Выберите вид указателя:\n"
                      << "1) Отсортированный массив\n"
                      << "2) Хеш-таблица\n";
            std::cout << "Ваш выбор: ";
            if(!(std::cin >> indexChoice)) {
                ClearInputBuffer();
                std::cout << "Неверный ввод.\n";
                WaitingForEnter();
                continue;
            }

            if(indexChoice == 1) {
                ArrayIndex array = BuildArrayIndex(pages);
                std::cout << "Указатель построен!\n";

                for(int i = 0; i < array.GetLength(); i++) {
                    std::cout << "Слово [" << (i + 1) << "]: " << array[i].key << ": {";
                    for(int j = 0; j < array[i].value.GetLength(); j++) {
                        std::cout << array[i].value[j];
                    }
                    std::cout << "}\n";
                }
                ClearInputBuffer(); 
                WaitingForEnter();

            } else if(indexChoice == 2) {
                std::cout << "Выберите количество корзин в хеш-таблице (1, 2, 3, ...): ";
                if(!(std::cin >> bucketCount)) {
                    ClearInputBuffer();
                    std::cout << "Неверный ввод.\n";
                    WaitingForEnter();
                    continue;
                }
                HashIndex dict = BuildHashIndex(pages, bucketCount);

                for(int i = 1; i <= bucketCount; i++) {
                    for(int j = 1; j <= dict.GetCount(); j++) {
                        
                    }
                }
            }

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
