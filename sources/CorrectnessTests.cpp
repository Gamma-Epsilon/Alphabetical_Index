#include <utility>
#include <cassert>
#include <iostream>
#include <iomanip>

#include "../headers/CorrectnessTests.hpp"


static const int totalWidth = 30;

using Dict         = HashDictionary<std::string, int>;
using Entry        = KeyValue<std::string, DynamicArray<int>>;
using DictIterator = UniquePtr<Iterator<KeyValue<std::string, int>>>;


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


/* Итераторы */
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
    } catch(const IteratorDoesNotHaveNext&) {
        exception_1 = true; 
    }

    assert(exception_1);

    /* Один элемент */
    assert(iterator_2.HasNext()); 
    assert(iterator_2.Next() == 67);
    
    bool exception_2 = false;
    try {
        iterator_2.Next();
    } catch(const IteratorDoesNotHaveNext&) {
        exception_2 = true;
    }

    assert(exception_2);

    /* Несколько элементов */
    bool exception_3 = false;
    
    try {
        while(iterator_1->HasNext()) {
            iterator_1->Next();
        }
    } catch(const IteratorDoesNotHaveNext&) {
        exception_3 = true;
    }

    assert(!exception_3);

    /* Конец последовательности */
    bool exception_4 = false; 

    try {
        iterator_1->Next();
    } catch(const IteratorDoesNotHaveNext&) {
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
    } catch(const IteratorDoesNotHaveNext&) {
        exception_1 = true; 
    }

    assert(exception_1);

    /* Один элемент */
    assert(iterator_2.HasNext()); 
    assert(iterator_2.Next() == 67);
    
    bool exception_2 = false;
    try {
        iterator_2.Next();
    } catch(const IteratorDoesNotHaveNext&) {
        exception_2 = true;
    }

    assert(exception_2);

    /* Несколько элементов */
    bool exception_3 = false;
    
    try {
        while(iterator_1->HasNext()) {
            iterator_1->Next();
        }
    } catch(const IteratorDoesNotHaveNext&) {
        exception_3 = true;
    }

    assert(!exception_3);

    /* Конец последовательности */
    bool exception_4 = false; 

    try {
        iterator_1->Next();
    } catch(const IteratorDoesNotHaveNext&) {
        exception_4 = true; 
    }

    assert(exception_4);

    std::cout << std::left << std::setw(totalWidth - 4) << "ListIteratorTest:" << "[OK]" << std::endl;
} 

void HashIteratorTest() {

    /* Пустая хеш-таблица */
    Dict empty(5); 
    DictIterator iteratorEmpty = empty.GetIterator();

    assert(!iteratorEmpty->HasNext());

    bool emptyException = false; 

    try {
        iteratorEmpty->Next();
    } catch (const IteratorDoesNotHaveNext&) {
        emptyException = true;
    }

    assert(emptyException);

    /* Один элемент */
    Dict one(5); 
    one.Add("planet", 42);

    DictIterator oneIterator = one.GetIterator();
    assert(oneIterator->HasNext());

    const KeyValue<std::string, int>& oneEntry = oneIterator->Next(); 
    assert(oneEntry.key   == "planet");
    assert(oneEntry.value == 42);
    assert(!oneIterator->HasNext());

    /* Несколько элементов в одной корзине */
    Dict collisions(1); 

    collisions.Add("planet", 10);
    collisions.Add("science", 20);
    collisions.Add("algorithm", 30);

    DictIterator collisionIterator = collisions.GetIterator(); 

    assert(collisionIterator->HasNext());
    assert(collisionIterator->Next().key == "planet");
    
    assert(collisionIterator->HasNext());
    assert(collisionIterator->Next().key == "science");
    
    assert(collisionIterator->HasNext());
    assert(collisionIterator->Next().key == "algorithm");

    assert(!collisionIterator->HasNext());

    /* Выход за пределы последовательности */
    bool exceptionEnd = false; 

    try {
        collisionIterator->Next();
    } catch(const IteratorDoesNotHaveNext&) {
        exceptionEnd = true;
    }

    assert(exceptionEnd);

    /* Несколько корзин вместе с пустыми */
    Dict many(7); 

    const std::string keys[5] = {"planet", "science", "algorithm", "cosmos", "star"};
    for(int i = 0; i < 5; i++) many.Add(keys[i], (i + 1) * 10);

    DictIterator iterator = many.GetIterator();

    bool seen[5] = {false, false, false, false, false};
    int visited = 0; 

    while(iterator->HasNext()) {
        const KeyValue<std::string, int>& entry = iterator->Next(); 
        int position = -1; 

        for(int i = 0; i < 5; i++) {
            if(keys[i] == entry.key) {
                position = i;
                break;
            }
        }

        assert(position != -1);  // Элемент получен
        assert(!seen[position]); // Элемент не возвращается дважды
        seen[position] = true;

        assert(entry.value == (position + 1) * 10);

        visited++; 
        assert(visited <= 5);
    }

    /* Все элементы посещены */
    assert(visited == many.GetCount());
    assert(visited == 5);

    for(int i = 0; i < 5; i++) assert(seen[i]);

    std::cout << std::left << std::setw(totalWidth - 4) << "HashIteratorTest:" << "[OK]" << std::endl;    
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
    PaginationResult wordPages = SplitIntoPages("one two three four five", 4, PageUnit::Words);

    assert(wordPages.pages.GetLength()    == 2);
    assert(wordPages.pages[0].GetLength() == 2);
    assert(wordPages.pages[1].GetLength() == 3);

    assert(wordPages.pages[0][0] == "one");
    assert(wordPages.pages[0][1] == "two");
    assert(wordPages.pages[1][0] == "three");
    assert(wordPages.pages[1][1] == "four");
    assert(wordPages.pages[1][2] == "five");

    /* Разбиение по символам */
    PaginationResult charPages = SplitIntoPages("cat dog bird fish tree sun", 10, PageUnit::Characters);

    assert(charPages.pages.GetLength()    == 4);
    assert(charPages.pages[0].GetLength() == 2);
    assert(charPages.pages[1].GetLength() == 3);
    assert(charPages.pages[2].GetLength() == 3);
    assert(charPages.pages[3].GetLength() == 1);

    assert(charPages.pages[0][0] == "cat");
    assert(charPages.pages[0][1] == "d");
    assert(charPages.pages[1][0] == "og");
    assert(charPages.pages[1][1] == "bird");
    assert(charPages.pages[1][2] == "fi");
    assert(charPages.pages[2][0] == "sh");
    assert(charPages.pages[2][1] == "tree");
    assert(charPages.pages[2][2] == "su");
    assert(charPages.pages[3][0] == "n");

    /* Пустой текст */
    PaginationResult empty = SplitIntoPages("", 10, PageUnit::Characters);

    assert(empty.pages.GetLength() == 0);

    /* Слово разделяется по страницам */
    PaginationResult bigWord = SplitIntoPages("котик милый", 8, PageUnit::Characters);

    assert(bigWord.pages.GetLength()    == 2);
    assert(bigWord.pages[0].GetLength() == 1);
    assert(bigWord.pages[1].GetLength() == 2);
    assert(bigWord.pages[0][0] == "коти");
    assert(bigWord.pages[1][0] == "к");
    assert(bigWord.pages[1][1] == "милый");

    PaginationResult longWord = SplitIntoPages("тетрагидропиранилциклопентилтетрагидропиридопиридиновые кот", 10, PageUnit::Characters);

    assert(longWord.pages.GetLength() == 7);
    assert(longWord.pages[0][0] == "тетра");
    assert(longWord.pages[5][0] == "иридиновые");
    assert(longWord.pages[6][0] == "кот");

    /* Некорректный размер страницы */
    bool exceptionPage = false; 

    try {
        SplitIntoPages("I LOVE YOU", 0, PageUnit::Words);
    } catch(const SizeIsIncorrect&) {
        exceptionPage = true;
    }

    assert(exceptionPage);

    std::cout << std::left << std::setw(totalWidth - 4) << "AlphabeticalIndexTest:" << "[OK]" << std::endl;
}


/* Указатель, построенный на основе массива */
void ArrayIndexTest() {
    
    /* Указатель на уже готовых страницах */
    PaginationResult pages;
    const int firstPage[]  = {1};
    const int secondPage[] = {2};
    const int thirdPage[]  = {3};
    
    pages.wordPages.Append({"!!!PLANeT", {firstPage, 1}});
    pages.wordPages.Append({"SCience!!", {firstPage, 1}});
    pages.wordPages.Append({"planet", {firstPage, 1}}); // Повтор на той же странице
    pages.wordPages.Append({"ALGORITHM", {secondPage, 1}});
    pages.wordPages.Append({"PlaneT", {secondPage, 1}});
    pages.wordPages.Append({"algorithm", {secondPage, 1}});
    pages.wordPages.Append({"science", {thirdPage, 1}});
    pages.wordPages.Append({"planet!!!", {thirdPage, 1}});

    /* Указатель, не зависящий от регистра */
    ArrayIndex arrayNotDepend = BuildArrayIndex(pages.wordPages);

    assert(arrayNotDepend.GetLength() == 3);  // 3 уникальных слова

    assert(arrayNotDepend[0].key == "algorithm");
    assert(arrayNotDepend[1].key == "planet"); 
    assert(arrayNotDepend[2].key == "science");

    assert(arrayNotDepend[1].value.GetLength() == 3 && arrayNotDepend[1].value[0] == 1 && arrayNotDepend[1].value[1] == 2 && arrayNotDepend[1].value[2] == 3);
    assert(arrayNotDepend[0].value[0] == 2);

    /* Указатель, не зависящий от регистра */
    ArrayIndex arrayDepend = BuildArrayIndex(pages.wordPages, CaseMode::Depend);

    assert(arrayDepend.GetLength() == 7);  // 3 уникальных слова

    assert(arrayDepend[0].key == "ALGORITHM");
    assert(arrayDepend[0].value.GetLength() == 1);
    assert(arrayDepend[0].value[0] == 2);
    
    assert(arrayDepend[1].key == "PLANeT");
    assert(arrayDepend[1].value.GetLength() == 1);
    assert(arrayDepend[1].value[0] == 1);
    
    assert(arrayDepend[2].key == "PlaneT");
    assert(arrayDepend[2].value.GetLength() == 1);
    assert(arrayDepend[2].value[0] == 2);
    
    assert(arrayDepend[3].key == "SCience");
    assert(arrayDepend[3].value.GetLength() == 1);
    assert(arrayDepend[3].value[0] == 1);
    
    assert(arrayDepend[4].key == "algorithm");
    assert(arrayDepend[4].value.GetLength() == 1);
    assert(arrayDepend[4].value[0] == 2);
    
    assert(arrayDepend[5].key == "planet");
    assert(arrayDepend[5].value.GetLength() == 2);
    assert(arrayDepend[5].value[0] == 1);
    assert(arrayDepend[5].value[1] == 3);
    
    assert(arrayDepend[6].key == "science");
    assert(arrayDepend[6].value.GetLength() == 1);
    assert(arrayDepend[6].value[0] == 3);  

    /* Пустой массив страниц */
    PaginationResult empty; 

    ArrayIndex empty_index = BuildArrayIndex(empty.wordPages);

    assert(empty_index.GetLength() == 0);

    /* Пустые страницы */
    PaginationResult someEmptyPages;
    const int second[] = {2};
    const int fourth[] = {4};

    someEmptyPages.wordPages.Append({"planet", {second, 1}});
    someEmptyPages.wordPages.Append({"planet", {fourth, 1}});
    someEmptyPages.wordPages.Append({"science", {fourth, 1}});

    ArrayIndex littleEmptyIndex = BuildArrayIndex(someEmptyPages.wordPages);

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
    PaginationResult pages;
    const int firstPage[]  = {1};
    const int secondPage[] = {2};
    const int thirdPage[]  = {3};

    pages.wordPages.Append({"!!!Planet", {firstPage, 1}});
    pages.wordPages.Append({"??SCIENCE", {firstPage, 1}});
    pages.wordPages.Append({"planet", {firstPage, 1}}); // Повтор на той же странице
    pages.wordPages.Append({"ALGorIthm", {secondPage, 1}});
    pages.wordPages.Append({"PLAnet??!!", {secondPage, 1}});
    pages.wordPages.Append({"algorithm", {secondPage, 1}});
    pages.wordPages.Append({"science", {thirdPage, 1}});
    pages.wordPages.Append({"PLANET", {thirdPage, 1}});

    /* Указатель, не зависящий от регистра */
    HashIndex dictionaryNotDepend = BuildHashIndex(pages.wordPages, 1);
    
    assert(dictionaryNotDepend.GetCount() == 3);  // 3 уникальных слова

    assert(dictionaryNotDepend.ContainsKey("algorithm"));
    assert(dictionaryNotDepend.ContainsKey("planet"));
    assert(dictionaryNotDepend.ContainsKey("science"));

    assert(dictionaryNotDepend.Get("algorithm")[0] == 2);
    assert(dictionaryNotDepend.Get("planet")[0] == 1 && dictionaryNotDepend.Get("planet")[1] == 2 && dictionaryNotDepend.Get("planet")[2] == 3);
    assert(dictionaryNotDepend.Get("science")[0] == 1 && dictionaryNotDepend.Get("science")[1] == 3);

    /* Указатель, зависящий от регистра */
    HashIndex dictionaryDepend = BuildHashIndex(pages.wordPages, 1, CaseMode::Depend);
    
    assert(dictionaryDepend.GetCount() == 8);  // 8 уникальных слов

    assert(dictionaryDepend.ContainsKey("algorithm"));
    assert(dictionaryDepend.ContainsKey("Planet"));
    assert(dictionaryDepend.ContainsKey("SCIENCE"));
    assert(dictionaryDepend.ContainsKey("planet"));
    assert(dictionaryDepend.ContainsKey("science"));
    assert(dictionaryDepend.ContainsKey("PLANET"));
    assert(dictionaryDepend.ContainsKey("PLAnet"));
    assert(dictionaryDepend.ContainsKey("ALGorIthm"));

    assert(dictionaryDepend.Get("algorithm")[0] == 2);
    assert(dictionaryDepend.Get("planet")[0] == 1 && dictionaryDepend.Get("PLANET")[0] == 3 && dictionaryDepend.Get("PLAnet")[0] == 2);
    assert(dictionaryDepend.Get("science")[0] == 3 && dictionaryDepend.Get("SCIENCE")[0] == 1);

    /* Пустой массив страниц */
    PaginationResult empty; 

    HashIndex empty_index = BuildHashIndex(empty.wordPages, 1);

    assert(empty_index.GetCount() == 0);

    /* Пустые страницы */
    PaginationResult someEmptyPages;
    const int second[] = {2};
    const int fourth[] = {4};

    someEmptyPages.wordPages.Append({"planet", {second, 1}});
    someEmptyPages.wordPages.Append({"planet", {fourth, 1}});
    someEmptyPages.wordPages.Append({"science", {fourth, 1}});

    HashIndex littleEmptyIndex = BuildHashIndex(someEmptyPages.wordPages, 1);

    assert(littleEmptyIndex.GetCount() == 2);

    assert(littleEmptyIndex.ContainsKey("planet"));
    assert(littleEmptyIndex.ContainsKey("science"));

    assert(littleEmptyIndex.Get("planet")[0] == 2 && littleEmptyIndex.Get("planet")[1] == 4);
    assert(littleEmptyIndex.Get("science")[0] == 4);

    std::cout << std::left << std::setw(totalWidth - 4) << "HashIndexTest:" << "[OK]" << std::endl;
}


void NormalizeWordTest() {

    std::string str1 = NormalizeWord("Hello", CaseMode::NotDepend);
    std::string str2 = NormalizeWord("!!!Hello", CaseMode::NotDepend);
    std::string str3 = NormalizeWord("Hello!!!", CaseMode::NotDepend);

    std::string str4 = NormalizeWord("Привет", CaseMode::NotDepend);
    std::string str5 = NormalizeWord("!!!Привет!", CaseMode::NotDepend);

    std::string str6 = NormalizeWord("!!!", CaseMode::NotDepend);
    std::string str7 = NormalizeWord("кто-нибудь,", CaseMode::NotDepend);
    std::string str8 = NormalizeWord("...HELLO!!", CaseMode::Depend);

    std::string str9 = NormalizeWord("", CaseMode::NotDepend);

    std::string str10 = NormalizeWord("Кот–Пёс", CaseMode::NotDepend); 
    std::string str11 = NormalizeWord("Кот–Пёс", CaseMode::Depend);

    assert(str1 == "hello");
    assert(str2 == "hello");
    assert(str3 == "hello");

    assert(str4 == "привет");
    assert(str5 == "привет");

    assert(str6 == "");
    assert(str7 == "кто-нибудь");
    assert(str8 == "HELLO");

    assert(str9 == "");

    assert(str10 == "кот–пёс");
    assert(str11 == "Кот–Пёс");

    std::cout << std::left << std::setw(totalWidth - 4) << "NormalizeWordTest:" << "[OK]" << std::endl;
}


/* Запуск всех тестов */
int RunAllCorrectnessTests() {
    DynamicArrayTest();
    LinkedListTest();

    ArrayIteratorTest(); 
    ListIteratorTest();
    HashIteratorTest();

    UniquePtrTest(); 
    SharedPtrTest();

    KeyValueTest();

    SortedSequenceTest();

    HashDictionaryTest();

    AlphabeticalIndexTest();
    ArrayIndexTest();
    HashIndexTest();
    NormalizeWordTest();

    std::cout << "\nAll tests passed! Congratulations! You're a good programmer" << std::endl;
    
    return 0;
}