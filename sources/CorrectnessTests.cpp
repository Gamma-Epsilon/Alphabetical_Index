#include <utility>
#include <cassert>
#include <iostream>

#include "../headers/CorrectnessTests.hpp"


/* Базовые структуры */
void DynamicArrayTest() {
    DynamicArray<int> array;

    /* Пустой массив */
    assert(array.GetCapacity() == 0 && array.GetLength() == 0);

    /* Добавление элемента */
    array.Append(6); 
    assert(array[0] == 6);

    array.Prepend(7);
    assert(array[0] == 7);
    assert(array[1] == 6);

    array.InsertAt(10, 1);
    assert(array.GetLength()   == 3);
    assert(array.GetCapacity() == 4);
    assert(array[1] == 10);

    /* Удаление элемента */
    array.RemoveAt(2);
    assert(array.GetLength() == 2);
    assert(array[array.GetLength() - 1] == 10);

    /* Resize/Reserve */
    array.Resize(4);
    assert(array[2] == 0);
    assert(array[3] == 0);

    array.Reserve(8);
    assert(array.GetLength()   == 4);
    assert(array.GetCapacity() == 8);

    /* Копирование и перемещение */
    DynamicArray<int> array_copied(array);
    array_copied[0] = 42; 

    assert(array_copied[0] == 42);
    assert(array[0] == 7);

    DynamicArray<int> array_moved(std::move(array_copied));
    assert(array_copied.GetCapacity() == 0 && array_copied.GetLength() == 0);

    /* Некорректные индексы */
    bool exception = false; 

    try {
        array[15];
    } catch(const IndexOutOfRange&) {
        exception = true;
    }

    assert(exception);

    std::cout << "DynamicArrayTest: [OK]" << std::endl;
}

void LinkedListTest() {
    LinkedList<int> list; 

    /* Пустой список */
    assert(list.GetLength() == 0);

    /* Добавление элемента */
    list.Append(42);
    assert(list[0] == 42);

    list.Prepend(67);
    assert(list[0] == 67);
    assert(list.GetLength() == 2);

    list.InsertAt(111, 1);
    assert(list[1] == 111);
    
    assert(list.GetFirst() == 67);
    assert(list.GetLast()  == 42);

    /* Удаление элемента */
    list.RemoveAt(0);
    assert(list.GetFirst()  == 111);
    assert(list.GetLength() == 2);

    /* Копирование и перемещение */
    LinkedList<int> list_copied(list);
    list_copied[1] = 404; 
    assert(list[1] != list_copied[1]);

    LinkedList<int> moved(std::move(list_copied));
    assert(list_copied.GetLength() == 0 && moved.GetLength() == 2);

    /* Некорректные индексы */
    bool exception = false; 

    try {
        list[42];
    } catch(const IndexOutOfRange&) {
        exception = true;
    }

    assert(exception);

    std::cout << "LinkedListTest: [OK]" << std::endl;
}


/* Итераторы на базовые структуры */



int RunAllCorrectnessTests() {
    DynamicArrayTest();
    LinkedListTest();

    std::cout << "All tests passed!\n" << std::endl;
    
    return 0;
}