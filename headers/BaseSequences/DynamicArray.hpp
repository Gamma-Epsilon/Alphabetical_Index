#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include "ISequence.hpp"

/* Предварительное объявление */
template <class T>
class ArrayIterator;

template <class T>
class DynamicArray : public ISequence<T> {
private: 
    UniquePtr<T[]> data;  
    int size;      // Текущий размер
    int capacity;  // Общий размер

public: 
    /* Создание объекта */
    DynamicArray();
    DynamicArray(const T* items, const int size);           
    explicit DynamicArray(const int size);          

    /* Копирование */
    DynamicArray(const DynamicArray<T>& dynamicArray); 
    DynamicArray<T>& operator=(const DynamicArray<T>& other);

    /* Перемещение */
    DynamicArray(DynamicArray<T>&& other) noexcept;
    DynamicArray<T>& operator=(DynamicArray<T>&& other) noexcept;    

    /* Удаление объекта */
    ~DynamicArray() override = default;

    /* Перегрузка */
    T& operator[](const int index) override;
    const T& operator[](const int index) const override;
    
    /* Итераторы */
    UniquePtr<Iterator<T>> GetIterator() const override;
    
    /* Операции */
    int GetCapacity() const;
    int GetLength()   const override;
    
    const T& GetFirst() const override;
    const T& GetLast()  const override;
    
    void Resize(const int newSize);      // Изменить кол-во элементов
    void Reserve(const int newCapacity); // Изменить общий размер

    void Append(const T& item) override;
    void Prepend(const T& item) override;
    void InsertAt(const T& item, const int index) override;
    void RemoveAt(const int index) override;

};

#include "../../sources/BaseSequences/DynamicArray.tpp"

#endif /* DYNAMIC_ARRAY_HPP */
