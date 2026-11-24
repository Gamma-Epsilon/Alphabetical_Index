#ifndef ALPHABETICAL_INDEX_CPP
#define ALPHABETICAL_INDEX_CPP

#include <sstream> // Для обработки слов

#include "../headers/AlphabeticalIndex.hpp"

static const std::string punctuation[] = {".", ",", "!", "?", ";", ":", 
    "\"", "'", "(", ")", "[", "]",
    "«", "»", "—", "–", "…"}; 

static const std::string lower_rus = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
static const std::string UPPER_RUS = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

int GetPageLimit(int pageNumber, int pageSize) {
    if(pageSize <= 0) throw SizeIsNegative("AlphabeticalIndex - Page size should be positive");
    
    if(pageNumber == 1)      return pageSize / 2;
    if(pageNumber % 10 == 0) return pageSize * 3/4;
    
    return pageSize;
}


std::string Lower(const std::string& word) {
    /* Приведение к нижнему регистру */
    std::string result; 

    size_t i = 0;
    while(i < word.size()) {
        char c = word[i];

        if(c >= 'A' && c <= 'Z') {  // Если буква английская
            char end_lower = c + ('a' - 'A');

            result += end_lower;
            i += 1;
        } else if(i + 1 < word.size()) { // Если есть еще один байт (буква, возможно, русская)
            std::string rus  = word.substr(i, 2);
            size_t rus_upper = UPPER_RUS.find(rus);
            
            if(rus_upper != std::string::npos) {
                std::string rus_lower = lower_rus.substr(rus_upper, 2);
                result += rus_lower;
                i += 2;
            } else {
                result += c; 
                i += 1;
            }
        } else {
            result += c;
            i += 1;
        }
    }

    return result; 
}


std::string NormalizeWord(const std::string& word) {
    /* Удаление крайних знаков препинания */
    size_t left  = 0; 
    size_t right = word.size(); // word[right - 1] - последний байт

    
    while(left < right) {
        bool found = false; 
        for(const std::string& symbol : punctuation) {

            if(symbol.size() > right - left) continue; // Помещается ли символ в принципе? 

            if(word.compare(left, symbol.size(), symbol) == 0) {
                left += symbol.size(); 
                found = true;
                break;
            }
        }

        if(!found) break;
    }

    while(right > left) {
        bool found = false;
        for(const std::string& symbol : punctuation) {

            if(symbol.size() > right - left) continue; 

            if(word.compare(right - symbol.size(), symbol.size(), symbol) == 0) {
                right -= symbol.size(); 
                found = true;
                break;
            }
        }

        if(!found) break;
    }

    std::string normalizedWord = word.substr(left, right - left);

    /* Приведение к нижнему регистру и возврат */

    return Lower(normalizedWord);
}


DynamicArray<DynamicArray<std::string>> SplitIntoPages(const std::string& text, int pageSize, PageUnit unit) {
    if(pageSize <= 0) throw SizeIsNegative("AlphabeticalIndex - Page size should be positive");
    
    DynamicArray<DynamicArray<std::string>> allPages;
    DynamicArray<std::string> wordsOnCurrentPage;

    int pageNumber  = 1; 
    int currentUsed = 0; 
    int required    = 0;

    std::istringstream stream(text); // Создаем поток ввода из строки text
    std::string word; 

    while(stream >> word) { // Считываем символы до следующего пробела/табуляции/переноса

        /* Сколько места займет очередное слово */
        if(unit == PageUnit::Words)           required = 1;
        else if(unit == PageUnit::Characters) required = CountChars(word); 

        /* Если слово не помещается даже на обычной странице, то пропускаем его */
        if(required > pageSize) continue;

        /* Пытаемся уместить слово, пока не найдем подходящую страницу */
        while(true) {

            /* Пересчитываем required для текущей страницы */
            if(unit == PageUnit::Words) {
                required = 1;
            } else {
                required = CountChars(word);

                /* Учитываем пробел перед словом */
                if(wordsOnCurrentPage.GetLength() > 0)
                    required++;
            }

            /* Умещается ли слово? */
            if(required + currentUsed <= GetPageLimit(pageNumber, pageSize)) {
                wordsOnCurrentPage.Append(word);
                currentUsed += required;
                break;
            } else {
                /* Сохраняем текущую страницу */
                allPages.Append(wordsOnCurrentPage);

                /* Переходим на новую страницу */
                wordsOnCurrentPage = DynamicArray<std::string>();

                currentUsed = 0;
                pageNumber++;    
            }
        }
    }

    if(wordsOnCurrentPage.GetLength() > 0) allPages.Append(wordsOnCurrentPage);
    
    return allPages;
}


int CountChars(const std::string& word) {
    int length = 0; 

    /* В UTF-8 правило: дополнительные байты одного символа начинаются с 10 (а русские буквы двухбайтовые) */
    /* Можем пропустить такие байты, чтобы их не считать за новую букву */
    for(unsigned char c : word) {
        if((c & 0xc0) != 0x80) length++; 
    }

    /* 0xc0 = 11000000 */
    /* 0x80 = 10000000 */ 
    /* => С помощью маски отсекаем все биты, кроме первых двух */

    return length;
}


/* Создание указателя на основе массива */
ArrayIndex BuildArrayIndex(const DynamicArray<DynamicArray<std::string>>& pages) {
    ArrayIndex array; 

    for(int i = 0; i < pages.GetLength(); i++) {    /* i - страница */
        for(int j = 0; j < pages[i].GetLength(); j++) {
            KeyValue<std::string, DynamicArray<int>> word; 
            word.key = NormalizeWord(pages[i][j]); 

            int indx       = array.Index(word, SearchMode::Half);
            int pageNumber = i + 1; // Т.к страницы с единицы

            if(indx != -1) {
                DynamicArray<int>& numbers = array[indx].value;

                // Страницы в порядке возрастания
                if(numbers.GetLength() == 0 || numbers[numbers.GetLength() - 1] != pageNumber) numbers.Append(pageNumber); 
            } else {
                word.value.Append(pageNumber);
                array.Add(word);
            }
        }   
    }

    return array;
}


/* Создание указателя на основе хеш-таблицы */
HashIndex BuildHashIndex(const DynamicArray<DynamicArray<std::string>>& pages, int bucketCount) {
    HashIndex dictionary(bucketCount); 

    for(int i = 0; i < pages.GetLength(); i++) {    /* i - страница */
        for(int j = 0; j < pages[i].GetLength(); j++) {
            int pageNumber = i + 1; 
            const std::string& word = NormalizeWord(pages[i][j]); 

            if(dictionary.ContainsKey(word)) {
                DynamicArray<int>& numbers = dictionary.Get(word);
                if(numbers.GetLength() == 0 || numbers[numbers.GetLength() - 1] != pageNumber) numbers.Append(pageNumber); 
            } else {
                DynamicArray<int> numbers; 
                numbers.Append(pageNumber);
                dictionary.Add(word, numbers);
            }
        }   
    }

    return dictionary;
}


#endif /* ALPHABETICAL_INDEX_CPP */