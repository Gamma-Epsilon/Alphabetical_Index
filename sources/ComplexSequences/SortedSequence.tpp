#ifndef SORTED_SEQUENCE_TPP
#define SORTED_SEQUENCE_TPP

#include "../../headers/ComplexSequences/SortedSequence.hpp"

/* Создание объекта */
template <class T> 
SortedSequence<T>::SortedSequence() : data() {}

template <class T> 
SortedSequence<T>::SortedSequence(const DynamicArray<T>& items) : data() {
    for(int i = 0; i < items.GetLength(); i++) { Add(items[i]); }
}


/* Доступ к существующим элементам */
template <class T> 
const T& SortedSequence<T>::operator[](int index) const {
    return data[index];
}


/* Операции */
template <class T> 
int SortedSequence<T>::LowerBound(const T& item) const {
    int left  = 0; 
    int rigth = data.GetLength(); // Вставить можно и после последнего элемента
    
    // Ищем позицию эл-та, НЕ МЕНЬШЕ исходного 
    while(left < rigth) {
        int mid = left + (rigth - left) / 2; 

        if(data[mid] < item) { left = mid + 1; }
        else                 { rigth = mid; }
        // data[mid] >= item выполняется rigth = mid, т.к. найденный элемент может быть правильной позицией
    }
    
    return left;
}

template <class T> 
int SortedSequence<T>::GetLength() const { return data.GetLength(); }

/* Вставка в отсортированном порядке (бинарная вставка) */
template <class T> 
void SortedSequence<T>::Add(const T& item) {
    int index = LowerBound(item);
    data.InsertAt(item, index);
}

/* Бинарный поиск O(logn) */
template <class T> 
int SortedSequence<T>::Index(const T& item) const {
    int index = LowerBound(item);

    if(index >= 0 && index <= data.GetLength() - 1) {
        if(data[index] == item) { return index; }
    }

    return -1; 
}

#endif /* SORTED_SEQUENCE_TPP */