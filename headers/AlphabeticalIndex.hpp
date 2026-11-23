#ifndef ALPHABETICAL_INDEX_HPP
#define ALPHABETICAL_INDEX_HPP 

#include <cstddef>
#include <string> 

#include "BaseSequences/DynamicArray.hpp"
#include "ComplexSequences/SortedSequence.hpp"
#include "Dictionary/HashDictionary.hpp"
#include "KeyValue.hpp"
#include "Exceptions.hpp"

using ArrayIndex = SortedSequence<KeyValue<std::string, DynamicArray<int>>>;
using HashIndex  = HashDictionary<std::string, DynamicArray<int>>;

/* Для возможности разбиения по кол-ву символов или слов */
enum class PageUnit {
    Words, 
    Characters
};

/* pageSize - стандартная вместимость страницы в словах или символах */

/* Лимит символов на странице в зависимости от правил */
int GetPageLimit(int pageNumber, int pageSize);

/* Приведение слова к нижнему регистру и удаление символов по краям */
std::string NormalizeWord(const std::string& word);

/* Приведение слов к нижнему регистру */
std::string Lower(const std::string& word);

/* Разбиение по страницам */
DynamicArray<DynamicArray<std::string>> SplitIntoPages(const std::string& text, int pageSize, PageUnit unit);

/* Подсчет размера слова */
int CountChars(const std::string& word);

/* Создание указателя на основе массива */
ArrayIndex BuildArrayIndex(const DynamicArray<DynamicArray<std::string>>& pages);

/* Создание указателя на основе хеш-таблицы */
HashIndex BuildHashIndex(const DynamicArray<DynamicArray<std::string>>& pages, int bucketCount);

#endif /* ALPHABETICAL_INDEX_HPP */