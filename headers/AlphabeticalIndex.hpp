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

/* Для возможности поиска с учетом и без учета регистра */
enum class CaseMode {
    Depend, 
    NotDepend
};

/* Возвращаемое значение SplitIntoPages */
struct PaginationResult {
    DynamicArray<DynamicArray<std::string>> pages;                     // Страницы
    DynamicArray<KeyValue<std::string, DynamicArray<int>>> wordPages; // Слова на каких страницах
};

/* pageSize - стандартная вместимость страницы в словах или символах */

/* Лимит символов на странице в зависимости от правил */
int GetPageLimit(int pageNumber, int pageSize);

/* Приведение слова к нижнему регистру и удаление символов по краям */
std::string NormalizeWord(const std::string& word, CaseMode mode = CaseMode::NotDepend);

/* Приведение слов к нижнему регистру */
std::string Lower(const std::string& word);

/* Разбиение по страницам */
PaginationResult SplitIntoPages(const std::string& text, int pageSize, PageUnit unit);

/* Подсчет размера слова */
int CountChars(const std::string& word);

/* Получение символа слова */
std::string GetSymbol(const std::string& word, size_t& bytePosition);

/* Создание указателя на основе массива */
ArrayIndex BuildArrayIndex(const DynamicArray<KeyValue<std::string, DynamicArray<int>>>& pages, CaseMode mode = CaseMode::NotDepend);

/* Создание указателя на основе хеш-таблицы */
HashIndex BuildHashIndex(const DynamicArray<KeyValue<std::string, DynamicArray<int>>>& pages, int bucketCount, CaseMode mode = CaseMode::NotDepend);

#endif /* ALPHABETICAL_INDEX_HPP */