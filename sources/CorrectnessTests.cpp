#include <utility>
#include <cassert>
#include <iostream>
#include <iomanip>

#include "../headers/CorrectnessTests.hpp"


static const int totalWidth = 30;

using alphIndex = DynamicArray<DynamicArray<std::string>>;
using Dict = HashDictionary<std::string, int>;
using Entry = KeyValue<std::string, DynamicArray<int>>;

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

    std::cout << std::left << std::setw(totalWidth - 4) << "DynamicArrayTest:" << "[OK]" << std::endl;
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

    std::cout << std::left << std::setw(totalWidth - 4) << "LinkedListTest:" << "[OK]" << std::endl;
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
    delete released_ptr;

    /* Swap */
    UniquePtr<int> ptr4(new int(111));
    UniquePtr<int> ptr5(new int(222));
    ptr4.swap(ptr5); 

    assert(*ptr4.get() == 222);
    assert(*ptr5.get() == 111);

    /* UniquePtr<int[]> */
    UniquePtr<int[]> ptr6(new int[3]{67, 42, 666});

    assert(ptr6[0] == 67 && ptr6[1] == 42 && ptr6[2] == 666);

    std::cout << std::left << std::setw(totalWidth - 4) << "UniquePtrTest:" << "[OK]" << std::endl;
} 

void SharedPtrTest() {
    /* Создание и доступ */
    SharedPtr<int> ptr1(new int(67));

    assert(*ptr1.get() == 67);
    assert(ptr1.use_count() == 1);

    /* Копирование и множественное владение */
    SharedPtr<int> ptr2(ptr1);
    SharedPtr<int> ptr3 = ptr1;

    assert(ptr1.get() == ptr2.get());
    assert(ptr2.get() == ptr3.get());

    assert(ptr1.use_count() == 3);
    assert(ptr2.use_count() == 3);
    assert(ptr3.use_count() == 3);

    /* Изменение общего объекта */
    *ptr2 = 42;

    assert(*ptr1 == 42);
    assert(*ptr3 == 42);

    /* Перемещение */
    SharedPtr<int> moved(std::move(ptr3));

    assert(*moved.get() == 42);
    assert(ptr3.get() == nullptr);
    assert(ptr3.use_count() == 0);
    assert(moved.use_count() == 3);

    /* Reset и уменьшение ref_count */
    ptr2.reset();

    assert(ptr2.get() == nullptr);
    assert(ptr2.use_count() == 0);
    assert(ptr1.use_count() == 2);
    assert(moved.use_count() == 2);

    /* Reset с новым объектом */
    SharedPtr<int> ptr4(new int(111));
    ptr4.reset(new int(67));

    assert(*ptr4.get() == 67);
    assert(ptr4.use_count() == 1);

    /* Swap */
    SharedPtr<int> ptr5(new int(111));
    SharedPtr<int> ptr6(new int(222));

    ptr5.swap(ptr6);

    assert(*ptr5.get() == 222);
    assert(*ptr6.get() == 111);

    assert(ptr5.use_count() == 1);
    assert(ptr6.use_count() == 1);

    /* SharedPtr<int[]> */
    SharedPtr<int[]> ptr7(new int[3]{67, 42, 666});

    assert(ptr7[0] == 67 && ptr7[1] == 42 && ptr7[2] == 666);
    assert(ptr7.use_count() == 1);

    /* Множественное владение массивом */
    SharedPtr<int[]> ptr8(ptr7);

    assert(ptr7.get() == ptr8.get());
    assert(ptr7.use_count() == 2);
    assert(ptr8.use_count() == 2);

    ptr8[0] = 100;

    assert(ptr7[0] == 100);

    ptr8.reset();

    assert(ptr7.use_count() == 1);
    assert(ptr8.use_count() == 0);

    std::cout << std::left << std::setw(totalWidth - 4) << "SharedPtrTest:" << "[OK]" << std::endl;
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

    std::cout << std::left << std::setw(totalWidth - 4) << "ArrayIteratorTest:" << "[OK]" << std::endl;
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

    std::cout << std::left << std::setw(totalWidth - 4) << "ListIteratorTest:" << "[OK]" << std::endl;
} 


/* Структура ключ-значение */
void KeyValueTest() {
    Entry entry;

    /* Заполнение полей */
    entry.key   = "planet"; 
    entry.value.Append(42);
    entry.value.Append(67);

    assert(entry.value.GetLength() == 2 && entry.value.GetCapacity() == 2);

    /* Проверка ключа */
    assert(entry.key == "planet");

    /* Проверка копирования */
    auto copy = entry;

    assert(copy.key == "planet");
    assert(copy.value[0] == 42);
    assert(copy.value[1] == 67);

    /* Проверка независимости копий */
    copy.value[0] = 111;

    assert(entry.value[0] == 42);
    assert(copy.value[0]  == 111);

    /* Проверка сравнений */
    Entry target1; 
    Entry target2; 

    target1.key = "a"; 
    target2.key = "b"; 

    target1.value.Append(222);
    target2.value.Append(111); 

    assert(target1 < target2);
    assert(!(target1 == target2));

    std::cout << std::left << std::setw(totalWidth - 4) << "KeyValueTest:" << "[OK]" << std::endl;
}

  
/* Отсортированный массив */
void SortedSequenceTest() {
    SortedSequence<int> sorted;

    /* Добавление элементов */
    sorted.Add(5); 
    sorted.Add(2); 
    sorted.Add(8);
    sorted.Add(1);
    sorted.Add(4);

    assert(sorted[0] == 1 && sorted[1] == 2 && sorted[2] == 4 && sorted[3] == 5 && sorted[4] == 8);

    /* Поиск элементов */
    int indx1 = sorted.Index(1, SearchMode::Half);
    int indx2 = sorted.Index(4, SearchMode::Half);
    int indx3 = sorted.Index(8, SearchMode::Half);

    assert(indx1 == 0 && indx2 == 2 && indx3 == 4);

    int indx4 = sorted.Index(7, SearchMode::Half); 
    
    assert(indx4 == -1);

    /* Копирование */
    SortedSequence<int> copied(sorted);

    copied.Add(0);

    assert(copied[0] == 0 && sorted[0] == 1);

    /* Перемещение */
    SortedSequence<int> moved(std::move(sorted));

    assert(sorted.GetLength() == 0 && moved.GetLength() == 5);

    /* Последовательность из неотсортированного массива */
    int arr[5] = {3, 2, 1};
    DynamicArray<int>   array(arr, 3);
    SortedSequence<int> sorted_array(array);

    assert(sorted_array[0] == 1 && sorted_array[1] == 2 && sorted_array[2] == 3);

    /* Проверка с KeyValue */
    SortedSequence<Entry> sortedWords;

    Entry first; 
    first.key = "science";
    first.value.Append(1);
    first.value.Append(3);

    Entry second; 
    second.key = "algorithm";
    second.value.Append(1);
    second.value.Append(67);

    Entry third; 
    third.key = "planet";
    third.value.Append(1);
    third.value.Append(42);

    /* Добавление элементов */
    sortedWords.Add(first); 
    sortedWords.Add(second); 
    sortedWords.Add(third);

    assert(sortedWords[0].key == "algorithm" && sortedWords[1].key == "planet" && sortedWords[2].key == "science");
    assert(sortedWords[0].value[0] == 1);

    /* Поиск элементов */
    Entry target1; 
    Entry target2;
    target1.key = "planet"; 
    target2.key = "borik";

    int indx5 = sortedWords.Index(target1, SearchMode::Half);
    int indx6 = sortedWords.Index(target2, SearchMode::Half); 

    assert(indx5 == 1);
    assert(indx6 == -1);

    /* Проверка бинарного поиска с золотым сечением */
    SortedSequence<int> sortedGolden;

    sortedGolden.Add(10);
    sortedGolden.Add(20);
    sortedGolden.Add(30);
    sortedGolden.Add(40);
    sortedGolden.Add(50);

    assert(sortedGolden.Index(10, SearchMode::Golden) == 0);
    assert(sortedGolden.Index(30, SearchMode::Golden) == 2);
    assert(sortedGolden.Index(50, SearchMode::Golden) == 4);
    assert(sortedGolden.Index(35, SearchMode::Golden) == -1);
    assert(sortedGolden.Index(5, SearchMode::Golden) == -1);
    assert(sortedGolden.Index(60, SearchMode::Golden) == -1);

    std::cout << std::left << std::setw(totalWidth - 4) << "SortedSequenceTest:" << "[OK]" << std::endl;
}


/* Hash-словарь */
void HashDictionaryTest() {

    Dict dictionary(1); // bucket = 1 для проверки коллизии 

    /* Пустой словарь */
    assert(dictionary.GetCount() == 0);
    assert(!dictionary.ContainsKey("GTA VI"));

    /* Некорректный bucketIndex */
    bool invalidIndex = false; 

    try {
        Dict invalid(0);
    } catch(const IndexOutOfRange&) {
        invalidIndex = true;
    }

    assert(invalidIndex);

    /* Добавление элементов */
    dictionary.Add("planet", 10);
    dictionary.Add("science", 20);
    dictionary.Add("algorithm", 30);

    assert(dictionary.GetCount() == 3);

    assert(dictionary.ContainsKey("planet"));
    assert(dictionary.ContainsKey("science"));
    assert(dictionary.ContainsKey("algorithm"));
    assert(!dictionary.ContainsKey("GTA VI"));

    /* Получение значений */
    assert(dictionary.Get("planet") == 10);
    assert(dictionary.Get("science") == 20);
    assert(dictionary.Get("algorithm") == 30);

    /* Изменение через Get() */
    dictionary.Get("planet") = 42;

    assert(dictionary.Get("planet") == 42);
    assert(dictionary.GetCount() == 3);

    /* Константная перегрузка Get() */
    const Dict& constDictionary = dictionary; 

    assert(constDictionary.Get("science") == 20);

    /* Повторное добавление ключа */
    bool duplicateKey = false; 

    try {
        dictionary.Add("planet", 67);
    } catch(const KeyAlreadyExists&) {
        duplicateKey = true;
    }

    assert(duplicateKey);

    /* Удаление элемента */
    dictionary.Remove("science");

    assert(dictionary.GetCount() == 2);
    assert(!dictionary.ContainsKey("science"));

    /* Получение отсутствующего ключа */
    bool missing = false; 

    try {
        dictionary.Get("science");
    } catch(const KeyDoesNotExist&) {
        missing = true;
    }

    assert(missing);

    /* Копирование */
    Dict copied(dictionary);

    assert(copied.GetCount() == 2 && dictionary.GetCount() == 2);
    
    copied.Get("planet") = 111; 

    assert(copied.Get("planet") == 111 && dictionary.Get("planet") == 42);

    /* Перемещение */
    Dict moved(std::move(dictionary));

    assert(moved.GetCount() ==  2 && dictionary.GetCount() == 0);

    std::cout << std::left << std::setw(totalWidth - 4) << "HashDictionaryTest:" << "[OK]" << std::endl;
}


/* Алфавитный указатель */
void AlphabeticalIndexTest() {

    /* GetPageLimit */
    assert(GetPageLimit(1, 10)  == 5);
    assert(GetPageLimit(10, 15) == 11);
    assert(GetPageLimit(5, 15)  == 15);

    /* CountChars */
    assert(CountChars("cat") == 3);
    assert(CountChars("кот") == 3);
    assert(CountChars("🚀🦊🍕") == 3);

    /* Разбиение по словам */
    alphIndex wordPages = SplitIntoPages("one two three four five", 4, PageUnit::Words);

    assert(wordPages.GetLength()    == 2);
    assert(wordPages[0].GetLength() == 2);
    assert(wordPages[1].GetLength() == 3);

    assert(wordPages[0][0] == "one");
    assert(wordPages[0][1] == "two");
    assert(wordPages[1][0] == "three");
    assert(wordPages[1][1] == "four");
    assert(wordPages[1][2] == "five");

    /* Разбиение по символам */
    alphIndex charPages = SplitIntoPages("cat dog bird fish tree sun", 10, PageUnit::Characters);

    assert(charPages.GetLength()    == 4);
    assert(charPages[0].GetLength() == 1);
    assert(charPages[1].GetLength() == 2);
    assert(charPages[2].GetLength() == 2);
    assert(charPages[3].GetLength() == 1);

    assert(charPages[0][0] == "cat");
    assert(charPages[1][0] == "dog");
    assert(charPages[1][1] == "bird");
    assert(charPages[2][0] == "fish");
    assert(charPages[2][1] == "tree");
    assert(charPages[3][0] == "sun");

    /* Пустой текст */
    alphIndex empty = SplitIntoPages("", 10, PageUnit::Characters);

    assert(empty.GetLength() == 0);

    /* Слово не помещается на первой странице, но помещается на второй */
    alphIndex bigWord = SplitIntoPages("котик милый", 8, PageUnit::Characters);

    assert(bigWord.GetLength()    == 3);
    assert(bigWord[0].GetLength() == 0);
    assert(bigWord[1][0] == "котик");
    assert(bigWord[2][0] == "милый");

    /* Слово не помещается ни на одной странице */
    alphIndex skippedWord = SplitIntoPages("тетрагидропиранилциклопентилтетрагидропиридопиридиновые кот", 10, PageUnit::Characters);

    assert(skippedWord.GetLength() == 1);
    assert(skippedWord[0][0] == "кот");

    /* Некорректный размер страницы */
    bool exceptionPage = false; 

    try {
        SplitIntoPages("I LOVE YOU", 0, PageUnit::Words);
    } catch(const SizeIsNegative&) {
        exceptionPage = true;
    }

    assert(exceptionPage);

    std::cout << std::left << std::setw(totalWidth - 4) << "AlphabeticalIndexTest:" << "[OK]" << std::endl;
}


/* Указатель, построенный на основе массива */
void ArrayIndexTest() {
    
    /* Указатель на уже готовых страницах */
    alphIndex pages;
    DynamicArray<std::string> firstPage;
    firstPage.Append("planet");
    firstPage.Append("science");
    firstPage.Append("planet"); // Повтор на той же странице

    DynamicArray<std::string> secondPage;
    secondPage.Append("algorithm");
    secondPage.Append("planet");
    secondPage.Append("algorithm");

    DynamicArray<std::string> thirdPage;
    thirdPage.Append("science");
    thirdPage.Append("planet");
    
    pages.Append(firstPage);
    pages.Append(secondPage);    
    pages.Append(thirdPage);

    ArrayIndex array = BuildArrayIndex(pages);

    assert(array.GetLength() == 3);  // 3 уникальных слова

    assert(array[0].key == "algorithm");
    assert(array[1].key == "planet"); 
    assert(array[2].key == "science");

    assert(array[1].value.GetLength() == 3 && array[1].value[0] == 1 && array[1].value[1] == 2 && array[1].value[2] == 3);
    assert(array[0].value[0] == 2);

    /* Пустой массив страниц */
    alphIndex empty; 

    ArrayIndex empty_index = BuildArrayIndex(empty);

    assert(empty_index.GetLength() == 0);

    /* Пустые страницы */
    alphIndex someEmptyPages; 

    DynamicArray<std::string> first;

    DynamicArray<std::string> second; 
    second.Append("planet");

    DynamicArray<std::string> third; 
    
    DynamicArray<std::string> fourth; 
    fourth.Append("planet");
    fourth.Append("science");

    someEmptyPages.Append(first);
    someEmptyPages.Append(second);
    someEmptyPages.Append(third);
    someEmptyPages.Append(fourth);

    assert(someEmptyPages.GetLength() == 4);

    ArrayIndex littleEmptyIndex = BuildArrayIndex(someEmptyPages);

    assert(littleEmptyIndex.GetLength() == 2);

    assert(littleEmptyIndex[0].key == "planet");
    assert(littleEmptyIndex[1].key == "science");

    assert(littleEmptyIndex[0].value[0] == 2 && littleEmptyIndex[0].value[1] == 4);
    assert(littleEmptyIndex[1].value[0] == 4);

    std::cout << std::left << std::setw(totalWidth - 4) << "ArrayIndexTest:" << "[OK]" << std::endl;
}


/* Указатель, построенный на основе хеш-таблицы */
void HashIndexTest() {

    /* Указатель на уже готовых страницах */
    alphIndex pages;
    DynamicArray<std::string> firstPage;
    firstPage.Append("planet");
    firstPage.Append("science");
    firstPage.Append("planet"); // Повтор на той же странице

    DynamicArray<std::string> secondPage;
    secondPage.Append("algorithm");
    secondPage.Append("planet");
    secondPage.Append("algorithm");

    DynamicArray<std::string> thirdPage;
    thirdPage.Append("science");
    thirdPage.Append("planet");
    
    pages.Append(firstPage);
    pages.Append(secondPage);    
    pages.Append(thirdPage);

    HashIndex dictionary = BuildHashIndex(pages, 1);
    
    assert(dictionary.GetCount() == 3);  // 3 уникальных слова

    assert(dictionary.ContainsKey("algorithm"));
    assert(dictionary.ContainsKey("planet"));
    assert(dictionary.ContainsKey("science"));

    assert(dictionary.Get("algorithm")[0] == 2);
    assert(dictionary.Get("planet")[0] == 1 && dictionary.Get("planet")[1] == 2 && dictionary.Get("planet")[2] == 3);
    assert(dictionary.Get("science")[0] == 1 && dictionary.Get("science")[1] == 3);

    /* Пустой массив страниц */
    alphIndex empty; 

    HashIndex empty_index = BuildHashIndex(empty, 1);

    assert(empty_index.GetCount() == 0);

    /* Пустые страницы */
    alphIndex someEmptyPages; 

    DynamicArray<std::string> first;

    DynamicArray<std::string> second; 
    second.Append("planet");

    DynamicArray<std::string> third; 
    
    DynamicArray<std::string> fourth; 
    fourth.Append("planet");
    fourth.Append("science");

    someEmptyPages.Append(first);
    someEmptyPages.Append(second);
    someEmptyPages.Append(third);
    someEmptyPages.Append(fourth);

    assert(someEmptyPages.GetLength() == 4);

    HashIndex littleEmptyIndex = BuildHashIndex(someEmptyPages, 1);

    assert(littleEmptyIndex.GetCount() == 2);

    assert(littleEmptyIndex.ContainsKey("planet"));
    assert(littleEmptyIndex.ContainsKey("science"));

    assert(littleEmptyIndex.Get("planet")[0] == 2 && littleEmptyIndex.Get("planet")[1] == 4);
    assert(littleEmptyIndex.Get("science")[0] == 4);

    std::cout << std::left << std::setw(totalWidth - 4) << "HashIndexTest:" << "[OK]" << std::endl;
}


/* Запуск всех тестов */
int RunAllCorrectnessTests() {
    DynamicArrayTest();
    LinkedListTest();

    ArrayIteratorTest(); 
    ListIteratorTest();

    UniquePtrTest(); 
    SharedPtrTest();

    KeyValueTest();

    SortedSequenceTest();

    HashDictionaryTest();

    AlphabeticalIndexTest();
    ArrayIndexTest();
    HashIndexTest();

    std::cout << "\nAll tests passed! Congratulations! You're a good programmer" << std::endl;
    
    return 0;
}