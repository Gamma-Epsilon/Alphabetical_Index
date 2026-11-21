#ifndef ALPHABETICAL_INDEX_CPP
#define ALPHABETICAL_INDEX_CPP

#include <sstream> // Для обработки слов

#include "../headers/AlphabeticalIndex.hpp"

int GetPageLimit(int pageNumber, int pageSize) {
    if(pageSize <= 0) throw SizeIsNegative("AlphabeticalIndex - Page size should be positive");
    
    if(pageNumber == 1)      return pageSize / 2;
    if(pageNumber % 10 == 0) return pageSize * 3/4;
    
    return pageSize;
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

#endif /* ALPHABETICAL_INDEX_CPP */