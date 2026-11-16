#ifndef CORRECTNESS_TESTS_HPP
#define CORRECTNESS_TESTS_HPP


#include "Sequences/DynamicArray.hpp"
#include "Sequences/LinkedList.hpp"

#include "Iterators/ArrayIterator.hpp"
#include "Iterators/ListIterator.hpp"

#include "Pointers/UniquePtr.hpp"
#include "Pointers/SharedPtr.hpp"

#include "KeyValue.hpp"

/* Базовые структуры  */
void DynamicArrayTest(); 
void LinkedListTest(); 

/* Умные указатели */
void UniquePtrTest(); 
void SharedPtrTest(); 

/* Итераторы на базовые структуры */
void ArrayIteratorTest(); 
void ListIteratorTest(); 

/* Структура ключ-значение */
void KeyValueTest();

/* Запуск всех тестов */
int RunAllCorrectnessTests();

#endif /* CORRECTNESS_TESTS_HPP */