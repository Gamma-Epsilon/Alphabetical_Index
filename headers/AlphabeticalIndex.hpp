#ifndef ALPHABETICAL_INDEX_HPP
#define ALPHABETICAL_INDEX_HPP 

#include <cstddef>
#include <string> 

#include "BaseSequences/DynamicArray.hpp"
#include "Exceptions.hpp"

/* Для возможности разбиения по кол-ву символов или слов */
enum class PageUnit {
    Words, 
    Characters
};

/* pageSize - стандартная вместимость страницы в словах или символах */

/* Лимит символов на странице в зависимости от правил */
int GetPageLimit(int pageNumber, int pageSize);

/* Разбиение по страницам */
DynamicArray<DynamicArray<std::string>> SplitIntoPages(const std::string& text, int pageSize, PageUnit unit);

/* Подсчет размера слова */
int CountChars(const std::string& word);


#endif /* ALPHABETICAL_INDEX_HPP */