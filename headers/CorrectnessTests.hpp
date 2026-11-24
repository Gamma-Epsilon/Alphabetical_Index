#ifndef CORRECTNESS_TESTS_HPP
#define CORRECTNESS_TESTS_HPP


#include "BaseSequences/DynamicArray.hpp"
#include "BaseSequences/LinkedList.hpp"

#include "Iterators/ArrayIterator.hpp"
#include "Iterators/ListIterator.hpp"
#include "Iterators/HashIterator.hpp"

#include "Pointers/UniquePtr.hpp"
#include "Pointers/SharedPtr.hpp"

#include "KeyValue.hpp"

#include "ComplexSequences/SortedSequence.hpp"

#include "Dictionary/HashDictionary.hpp"

#include "AlphabeticalIndex.hpp"


/* Базовые структуры  */
void DynamicArrayTest(); 
void LinkedListTest(); 

/* Умные указатели */
void UniquePtrTest(); 
void SharedPtrTest(); 

/* Итераторы на базовые структуры */
void ArrayIteratorTest(); 
void ListIteratorTest(); 
void HashIteratorTest();

/* Структура ключ-значение */
void KeyValueTest();

/* Отсортированный массив */
void SortedSequenceTest();

/* Hash-словарь */
void HashDictionaryTest();

/* Алфавитный указатель */
void AlphabeticalIndexTest();
void ArrayIndexTest();
void HashIndexTest();
void NormalizeWordTest();

/* Запуск всех тестов */
int RunAllCorrectnessTests();


#endif /* CORRECTNESS_TESTS_HPP */