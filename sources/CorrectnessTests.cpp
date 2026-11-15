#include <utility>

#include "../headers/CorrectnessTests.hpp"

void DynamicArrayTest() {
    DynamicArray<int> array = DynamicArray<int>();

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




int RunAllCorrectnessTests() {
    DynamicArrayTest();

    std::cout << "All tests passed!\n" << std::endl;
    
    return 0;
}