#ifndef SORTED_ARRAY_TPP
#define SORTED_ARRAY_TPP

#include "../../headers/ComplexSequences/SortedArray.hpp"

static const double GOLDEN_RATIO = 0.61803398875; // Коэффициент золотого сечения

/* Создание объекта */
template <class T> 
SortedArray<T>::SortedArray() : data() {}

template <class T> 
SortedArray<T>::SortedArray(const DynamicArray<T>& items) : data() {
    for(int i = 0; i < items.GetLength(); i++) { Add(items[i]); }
}


/* Доступ к существующим элементам */
template <class T> 
const T& SortedArray<T>::operator[](int index) const {
    return data[index];
}

/* Пожалуйста, не изменяйте ключи, иначе сортировки не будет. Перегрузка нужна для построения алфавитного указателя */
template <class T> 
T& SortedArray<T>::operator[](int index) {
    return data[index];
}

/* Операции */
template <class T> 
int SortedArray<T>::LowerBound(const T& item, SearchMode mode) const {
    int left  = 0; 
    int right = data.GetLength(); // Вставить можно и после последнего элемента
    
    // Ищем позицию эл-та, НЕ МЕНЬШЕ исходного 
    while(left < right) {
        int mid; 

        if(mode == SearchMode::Half) { mid = left + (right - left) / 2; }
        else { mid = left + static_cast<int>((right - left) * GOLDEN_RATIO); }

        if(data[mid] < item) { left  = mid + 1; }
        else                 { right = mid; }
        // data[mid] >= item выполняется right = mid, т.к. найденный элемент может быть правильной позицией
    }
    
    return left;
}

template <class T> 
int SortedArray<T>::GetLength() const { return data.GetLength(); }

/* Вставка в отсортированном порядке (бинарная вставка) */
template <class T> 
void SortedArray<T>::Add(const T& item) {
    int index = LowerBound(item, SearchMode::Half);
    data.InsertAt(item, index);
}

/* Бинарный поиск O(logn) */
template <class T> 
int SortedArray<T>::Index(const T& item, SearchMode mode) const {
    int index = LowerBound(item, mode);

    if(index >= 0 && index <= data.GetLength() - 1) {
        if(data[index] == item) { return index; }
    }

    return -1; 
}

#endif /* SORTED_ARRAY_TPP */