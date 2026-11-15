#ifndef CORRECTNESS_TESTS_HPP
#define CORRECTNESS_TESTS_HPP


#include "../headers/Sequences/DynamicArray.hpp"
#include "../headers/Sequences/LinkedList.hpp"

#include "../headers/Iterators/ArrayIterator.hpp"
#include "../headers/Iterators/ListIterator.hpp"

#include "../headers/Pointers/UniquePtr.hpp"
#include "../headers/Pointers/SharedPtr.hpp"

/* Базовые структуры  */
void DynamicArrayTest(); 
void LinkedListTest(); 

/* Умные указатели */
void UniquePtrTest(); 
void SharedPtrTest(); 

/* Итераторы на базовые структуры */
void ArrayIteratorTest(); 
void ListIteratorTest(); 

/* Запуск всех тестов */
int RunAllCorrectnessTests();

#endif /* CORRECTNESS_TESTS_HPP */