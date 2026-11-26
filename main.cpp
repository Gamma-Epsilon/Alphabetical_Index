#include <iostream>

#include "headers/CorrectnessTests.hpp"
#include "headers/PerformanceTests.hpp"
#include "headers/Console.hpp"

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
        PaginationResult result;
        
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

                result = SplitIntoPages(text, pageSize, PageUnit::Characters);

            } else if(separation == "слово") {
                std::cout << "Введите количество слов на стандартной странице: ";
                std::cin  >> pageSize; 

                result = SplitIntoPages(text, pageSize, PageUnit::Words);

            } else {
                std::cout << "Неверный вид разбиения.\n";
                ClearInputBuffer();
                WaitingForEnter();    
                break;            
            }

            std::cout << "Введенный текст разбит на страницы!\n";

            for(int i = 0; i < result.pages.GetLength(); i++) {
                std::cout << "Страница [" << (i + 1) << "]: ";
                for(int j = 0; j < result.pages[i].GetLength(); j++){
                    std::cout << result.pages[i][j] << " ";
                }
                std::cout << "\n";
            }

            ClearInputBuffer();

            std::cout << "Выберите вид указателя:\n"
                      << "1) Отсортированный массив\n"
                      << "2) Хеш-таблица\n"
                      << "Ваш выбор: ";
            if(!(std::cin >> indexChoice)) {
                ClearInputBuffer();
                std::cout << "Неверный ввод.\n";
                WaitingForEnter();
                continue;
            }

            std::cout << "1) Без учета регистра\n"
                      << "2) С учетом регистра\n"
                      << "Ваш выбор: ";
            int caseChoice;
            if (!(std::cin >> caseChoice) || (caseChoice != 1 && caseChoice != 2)) {
                std::cout << "Неверный режим регистра.\n";
                ClearInputBuffer();
                WaitingForEnter();
                continue;
            }
            CaseMode mode;
            if(caseChoice == 1) mode = CaseMode::NotDepend;
            else mode = CaseMode::Depend;

            if(indexChoice == 1) {
                ArrayIndex array = BuildArrayIndex(result.wordPages, mode);
                std::cout << "Указатель построен!\n";

                for(int i = 0; i < array.GetLength(); i++) {
                    std::cout << "Слово [" << (i + 1) << "]: " << array[i].key << ": {";
                    for(int j = 0; j < array[i].value.GetLength(); j++) {
                        std::cout << array[i].value[j] << " ";
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
                HashIndex dict = BuildHashIndex(result.wordPages, bucketCount, mode);

                auto iterator = dict.GetIterator(); 

                while(iterator->HasNext()) {
                    const auto& entry = iterator->Next(); 

                    std::cout << "Слово '" << entry.key << "': "; 
                    for(int i = 0; i < entry.value.GetLength(); i++) {
                            std::cout << entry.value[i] << " ";
                        }
                    std::cout << "\n";
                }

                ClearInputBuffer();
                WaitingForEnter();
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
