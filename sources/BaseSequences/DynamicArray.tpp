#ifndef DYNAMIC_ARRAY_TPP
#define DYNAMIC_ARRAY_TPP

#include "../../headers/BaseSequences/DynamicArray.hpp"
#include "../../headers/Iterators/ArrayIterator.hpp"


/* СОЗДАНИЕ ОБЪЕКТА */
template <class T>
DynamicArray<T>::DynamicArray() : data(nullptr), size(0), capacity(0) {}

template <class T>
DynamicArray<T>::DynamicArray(const T* items, const int size) : size(size), capacity(size) {
    if(size < 0) throw IndexOutOfRange("DynamicArray::DynamicArray - negative count.");
    if(size > 0 && items == nullptr) throw std::invalid_argument("DynamicArray - null items.");
    
    data = UniquePtr<T[]>(new T[capacity]{});
    for(int i = 0; i < size; i++){
        data[i] = items[i];
    }
}

template <class T>
DynamicArray<T>::DynamicArray(const int size) : size(size), capacity(size) {
    if(size < 0) throw IndexOutOfRange("DynamicArray::DynamicArray - negative size.");

    data = UniquePtr<T[]>(new T[capacity]{});
}

// Конструктор копирования: выделяем собственный массив той же вместимости
// Копируем только логические элементы; исходный массив остаётся независимым
template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : data(new T[other.capacity]{}), size(other.size), capacity(other.capacity) {
    for(int i = 0; i < size; i++){
        data[i] = other[i];
    }
}

// Конструктор перемещения: передаём владение памятью без копирования элементов
// std::move позволяет вызвать перемещение UniquePtr, которое обнуляет other.data
template <class T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) noexcept
    : data(std::move(other.data)), size(other.size), capacity(other.capacity) {
    // Согласуем размер и вместимость источника с его теперь пустым хранилищем
    other.size = 0;
    other.capacity = 0;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T>&& other) noexcept {
    if(this != &other) {
        data = std::move(other.data);
        size = other.size;
        capacity   = other.capacity;
        other.size = 0;
        other.capacity = 0;
    }

    return *this;
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other){
    if(this == &other) return *this;

    DynamicArray<T> copy(other); 

    data.swap(copy.data);
    std::swap(size, copy.size);
    std::swap(capacity, copy.capacity);

    return *this;
}


/* ПЕРЕГРУЗКА [] */
template <class T> 
T& DynamicArray<T>::operator[](const int index){
    if(index < 0 || index >= size) throw IndexOutOfRange("DynamicArray::operator[] - index out of range.");
    return data[index];
}

template <class T> 
const T& DynamicArray<T>::operator[](const int index) const {
    if(index < 0 || index >= size) throw IndexOutOfRange("DynamicArray::operator[] - index out of range.");
    return data[index];
}

/* ИТЕРАТОР */
template <class T>
UniquePtr<Iterator<T>> DynamicArray<T>::GetIterator() const {
    return UniquePtr<Iterator<T>>(new ArrayIterator<T>(*this));
}


/* ОПЕРАЦИИ */
template <class T>
int DynamicArray<T>::GetLength() const { return size; }

template <class T>
const T& DynamicArray<T>::GetFirst() const { return (*this)[0]; }

template <class T>
const T& DynamicArray<T>::GetLast() const { return (*this)[size - 1]; }

template <class T> 
int DynamicArray<T>::GetCapacity() const {
    return capacity;
}

/* Изменение размера */
template <class T>
void DynamicArray<T>::Resize(const int newSize) {
    if(newSize < 0) throw IndexOutOfRange("DynamicArray::Resize - negative size.");

    if(newSize > capacity){
        Reserve(newSize);
    }

    for(int i = newSize; i < size; i++){
        data[i] = T{}; // !!
    }

    for(int i = size; i < newSize; i++){
        data[i] = T{}; // !!
    }

    size = newSize;
}
/* Resize меняет size и при необходимости меняет capacity */

template <class T> 
void DynamicArray<T>::Reserve(const int newCapacity) {
    if(newCapacity < 0) throw IndexOutOfRange("DynamicArray::Reserve - negative capacity.");
    
    if(newCapacity <= capacity) return; 

    UniquePtr<T[]> newData = UniquePtr<T[]>(new T[newCapacity]{});

    for(int i = 0; i < size; i++){
        newData[i] = std::move(data[i]);
    }

    data     = std::move(newData); 
    capacity = newCapacity;  // !!
}
/* Reserve меняет capacity, сохраняя size */


/* Добавление/удаление */
template <class T> 
void DynamicArray<T>::Append(const T& item) {
    InsertAt(item, size);
}

template <class T> 
void DynamicArray<T>::Prepend(const T& item) {
    InsertAt(item, 0);
}

template <class T>
void DynamicArray<T>::InsertAt(const T& item, int index) {
    if (index < 0 || index > size) throw IndexOutOfRange("Invalid index.");

    T saved = item;

    if (size == capacity)
        Reserve(capacity == 0 ? 1 : capacity * 2);

    for (int i = size; i > index; --i)
        data[i] = std::move(data[i - 1]);

    data[index] = std::move(saved);
    ++size;
}

template <class T> 
void DynamicArray<T>::RemoveAt(const int index) {
    if(index < 0 || index >= size) throw IndexOutOfRange("DynamicArray::RemoveAt - invalid index.");

    for(int i = index; i < size - 1; i++){
        data[i] = std::move(data[i + 1]);
    }
    
    data[size - 1] = T{};
    size--;
}

#endif /* DYNAMIC_ARRAY_TPP */
