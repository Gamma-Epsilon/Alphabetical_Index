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


/* Умные указатели */
void UniquePtrTest() {
    /* Создание и досту */ 
    UniquePtr<int> ptr1(new int(67));

    assert(*ptr1.get() == 67);

    /* Перемещение */
    UniquePtr<int> moved(std::move(ptr1));

    assert(*moved.get() == 67);
    assert(ptr1.get() == nullptr); 

    /* Reset */
    UniquePtr<int> ptr2(new int(42));
    ptr2.reset(new int(67));

    assert(*ptr2.get() == 67);

    /* Release */
    UniquePtr<int> ptr3(new int(42));
    int* released_ptr = ptr3.release();

    assert(ptr3.get() == nullptr);
    assert(*released_ptr == 42);

    /* Swap */
    UniquePtr<int> ptr4(new int(111));
    UniquePtr<int> ptr5(new int(222));
    ptr4.swap(ptr5); 

    assert(*ptr4.get() == 222);
    assert(*ptr5.get() == 111);

    /* UniquePtr<int[]> */
    UniquePtr<int[]> ptr6(new int[3]{67, 42, 666});

    assert(ptr6[0] == 67 && ptr6[1] == 42 && ptr6[2] == 666);

    std::cout << "UniquePtrTest: [OK]" << std::endl;
} 

void SharedPtrTest() {

}


/* Итераторы на базовые структуры */
void ArrayIteratorTest() {
    int many[5] = {1, 2, 3, 4, 5};
    int one[1]  = {67};

    DynamicArray<int> array_1(many, 5);
    DynamicArray<int> array_2(one, 1);
    DynamicArray<int> empty;

    UniquePtr<Iterator<int>> iterator_1 = array_1.GetIterator();
    ArrayIterator<int> iterator_2(array_2);
    auto iterator_3 = empty.GetIterator();

    /* Пустой контейнер */
    bool exception_1 = false; 

    try {
        iterator_3->Next();
    } catch(const IndexOutOfRange&) {
        exception_1 = true; 
    }

    assert(exception_1);

    /* Один элемент */
    assert(iterator_2.HasNext()); 
    assert(iterator_2.Next() == 67);
    
    bool exception_2 = false;
    try {
        iterator_2.Next();
    } catch(const IndexOutOfRange&) {
        exception_2 = true;
    }

    assert(exception_2);

    /* Несколько элементов */
    bool exception_3 = false;
    
    try {
        while(iterator_1->HasNext()) {
            iterator_1->Next();
        }
    } catch(const IndexOutOfRange&) {
        exception_3 = true;
    }

    assert(!exception_3);

    /* Конец последовательности */
    bool exception_4 = false; 

    try {
        iterator_1->Next();
    } catch(const IndexOutOfRange&) {
        exception_4 = true; 
    }

    assert(exception_4);

    std::cout << "ArrayIteratorTest: [OK]" << std::endl;
}

void ListIteratorTest() {
    int many[5] = {1, 2, 3, 4, 5};
    int one[1]    = {67};

    LinkedList<int> list_1(many, 5);
    LinkedList<int> list_2(one, 1);
    LinkedList<int> empty;

    UniquePtr<Iterator<int>> iterator_1 = list_1.GetIterator();
    ListIterator<int> iterator_2(list_2);
    auto iterator_3 = empty.GetIterator();

    /* Пустой контейнер */
    bool exception_1 = false; 

    try {
        iterator_3->Next();
    } catch(const IndexOutOfRange&) {
        exception_1 = true; 
    }

    assert(exception_1);

    /* Один элемент */
    assert(iterator_2.HasNext()); 
    assert(iterator_2.Next() == 67);
    
    bool exception_2 = false;
    try {
        iterator_2.Next();
    } catch(const IndexOutOfRange&) {
        exception_2 = true;
    }

    assert(exception_2);

    /* Несколько элементов */
    bool exception_3 = false;
    
    try {
        while(iterator_1->HasNext()) {
            iterator_1->Next();
        }
    } catch(const IndexOutOfRange&) {
        exception_3 = true;
    }

    assert(!exception_3);

    /* Конец последовательности */
    bool exception_4 = false; 

    try {
        iterator_1->Next();
    } catch(const IndexOutOfRange&) {
        exception_4 = true; 
    }

    assert(exception_4);

    std::cout << "ListIteratorTest: [OK]" << std::endl;
} 


/* Запуск всех тестов */
int RunAllCorrectnessTests() {
    DynamicArrayTest();
    LinkedListTest();

    ArrayIteratorTest(); 
    ListIteratorTest();

    UniquePtrTest(); 
    SharedPtrTest();

    std::cout << "All tests passed!\n" << std::endl;
    
    return 0;
}