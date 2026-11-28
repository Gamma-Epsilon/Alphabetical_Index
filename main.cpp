#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "headers/CorrectnessTests.hpp"
#include "headers/Benchmarks.hpp"

using Dict  = HashDictionary<std::string, int>;
using Entry = KeyValue<std::string, DynamicArray<int>>;

/* Очистка консоли */
static void ClearScreen() {
    std::cout.flush(); 
    std::system("cls");
}

/* Сброс остатка ввода до конца строки */
static void ClearInputBuffer() {
    std::cin.clear(); 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/* Ожидание Enter */
static void WaitingForEnter() {
    std::cout << "Нажмите Enter для продолжения..."; 
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    ClearScreen();
}

/* Вывод страниц исходного текста */
static void PrintTextPages(const PaginationResult& result) {
    for(int i = 0; i < result.pages.GetLength(); ++i) {
        std::cout << "Страница [" << (i + 1) << "]: ";

        for(int j = 0; j < result.pages[i].GetLength(); ++j) {
            if(j > 0) std::cout << ' ';
            std::cout << result.pages[i][j];
        }

        std::cout << '\n';
    }
}

/* Вывод номеров страниц */
static void PrintPageNumbers(const DynamicArray<int>& numbers) {
    for(int i = 0; i < numbers.GetLength(); ++i) {
        std::cout << ' ' << numbers[i];
    }

    std::cout << '\n';
}

static void PrintIndexEntry(const Entry& entry) {
    std::cout << entry.key << ':';
    PrintPageNumbers(entry.value);
}

/* Перегрузка вывода в консоль Array и HashIndex */
static void PrintIndex(const ArrayIndex& index) {
    for(int i = 0; i < index.GetLength(); ++i) {
        PrintIndexEntry(index[i]);
    }
}

static void PrintIndex(const HashIndex& index) {
    UniquePtr<Iterator<Entry>> iterator = index.GetIterator();

    while(iterator->HasNext()) {
        PrintIndexEntry(iterator->Next());
    }
}

/* Поиск по готовому ключу. nullptr означает, что слова нет в указателе */
static const DynamicArray<int>* FindWord(const ArrayIndex& index, const std::string& key) {
    Entry word;
    word.key = key;

    int position = index.Index(word, SearchMode::Half);
    if(position == -1) return nullptr;

    return &index[position].value;
}

static const DynamicArray<int>* FindWord(const HashIndex& index, const std::string& key) {
    if(!index.ContainsKey(key)) return nullptr;

    return &index.Get(key);
}

/* Общее меню поиска для массива и хеш-таблицы */
template <class Index>
static void RunIndexMenu(const Index& notDepend, const Index& depend) {
    std::cout << "Указатели построены!\n";

    while(true) {
        std::cout << "\n1) Найти слово без учёта регистра\n"
                  << "2) Найти слово с учётом регистра\n"
                  << "3) Показать указатель без учёта регистра\n"
                  << "4) Показать указатель с учётом регистра\n"
                  << "0) Главное меню\n"
                  << "Ваш выбор: ";

        int choice;
        if(!(std::cin >> choice)) {
            if(std::cin.eof()) return;
            ClearInputBuffer();
            std::cout << "Неверный ввод.\n";
            continue;
        }

        ClearInputBuffer();

        if(choice == 0) return;

        if(choice == 3) {
            PrintIndex(notDepend);
            continue;
        }

        if(choice == 4) {
            PrintIndex(depend);
            continue;
        }

        if(choice != 1 && choice != 2) {
            std::cout << "Неверный пункт меню.\n";
            continue;
        }

        std::cout << "Введите слово: ";
        std::string line;
        if(!std::getline(std::cin, line)) return;

        std::istringstream input(line);

        std::string word;
        std::string extra;

        if(!(input >> word) || (input >> extra)) {
            std::cout << "Введите ровно одно слово.\n";
            continue;
        }

        CaseMode mode = CaseMode::NotDepend;
        if(choice == 2) mode = CaseMode::Depend;

        std::string key = NormalizeWord(word, mode);
        if(key.empty()) {
            std::cout << "После удаления пунктуации слово пустое.\n";
            continue;
        }

        // Ищем в указателе, построенном с тем же режимом регистра
        const DynamicArray<int>* pages = (mode == CaseMode::Depend) ? FindWord(depend, key) : FindWord(notDepend, key);

        if(pages == nullptr) {
            std::cout << "Слово не найдено.\n";
            continue;
        }

        std::cout << "Страницы:";
        PrintPageNumbers(*pages);
    }
}

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
        if(!(std::cin >> main_choice)) {
            if(std::cin.eof()) return 0;
            ClearInputBuffer();
            std::cout << "Неверный ввод.\n";
            WaitingForEnter();
            continue;
        }

        ClearInputBuffer();

        std::string text;
        std::string separation;

        int pageSize;

        PageUnit unit;
        PaginationResult result;

        int indexChoice;
        int bucketCount;

        switch(main_choice) {
        case 1:
            std::cout << "Введите текст:\n";
            if(!std::getline(std::cin, text)) return 0;

            std::cout << "Введите способ разбиения ('символ' или 'слово'): ";
            std::cin >> separation;

            if(separation == "символ") unit = PageUnit::Characters;
            else if(separation == "слово") unit = PageUnit::Words;
            else {
                std::cout << "Неверный вид разбиения.\n";
                ClearInputBuffer();
                WaitingForEnter();
                break;
            }

            std::cout << "Введите размер стандартной страницы (минимум 2): ";
            if(!(std::cin >> pageSize) || pageSize < 2) {
                std::cout << "Укажи целое число не меньше 2.\n";
                ClearInputBuffer();
                WaitingForEnter();
                break;
            }

            result = SplitIntoPages(text, pageSize, unit);

            std::cout << "Введённый текст разбит на страницы!\n";
            PrintTextPages(result);
            ClearInputBuffer();

            std::cout << "Выберите вид указателя:\n"
                      << "1) Отсортированный массив\n"
                      << "2) Хеш-таблица\n"
                      << "Ваш выбор: ";

            if(!(std::cin >> indexChoice) || (indexChoice != 1 && indexChoice != 2)) {
                std::cout << "Выберите 1 или 2.\n";
                ClearInputBuffer();
                WaitingForEnter();
                break;
            }

            ClearInputBuffer();

            if(indexChoice == 1) {
                ArrayIndex notDepend = BuildArrayIndex(result.wordPages, CaseMode::NotDepend);
                ArrayIndex depend    = BuildArrayIndex(result.wordPages, CaseMode::Depend);

                RunIndexMenu(notDepend, depend);
            } else {
                std::cout << "Введите количество корзин хеш-таблицы: ";
                if(!(std::cin >> bucketCount) || bucketCount <= 0) {
                    std::cout << "Введите положительное число корзин.\n";
                    ClearInputBuffer();
                    WaitingForEnter();
                    break;
                }

                ClearInputBuffer();

                HashIndex notDepend = BuildHashIndex(result.wordPages, bucketCount, CaseMode::NotDepend);
                HashIndex depend    = BuildHashIndex(result.wordPages, bucketCount, CaseMode::Depend);

                RunIndexMenu(notDepend, depend);
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
