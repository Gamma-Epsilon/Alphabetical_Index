#ifndef SORTED_ARRAY_HPP
#define SORTED_ARRAY_HPP

#include "ISortedSequence.hpp"
#include "../BaseSequences/DynamicArray.hpp"

template <class T>
class SortedArray : public ISortedSequence<T> {
private:
    DynamicArray<T> data;

    /* Разновидность бинарного поиска. Возвращаем позицию от 0 до size */
    int LowerBound(const T& item, SearchMode mode) const;
public: 
    /* Создание объекта */
    SortedArray();
    SortedArray(const DynamicArray<T>& items);            

    /* Копирование */
    SortedArray(const SortedArray<T>& sortedArray)      = default; 
    SortedArray<T>& operator=(const SortedArray<T>& other) = default;

    /* Перемещение */
    SortedArray(SortedArray<T>&& other)               noexcept = default;
    SortedArray<T>& operator=(SortedArray<T>&& other) noexcept = default;

    /* Удаление объекта */
    ~SortedArray() = default; 

    /* Доступ к существующим элементам */
    const T& operator[](int index) const override;
    T& operator[](int index) override;
    
    /* Операции */
    int  GetLength() const  override;

    int Index(const T& item, SearchMode mode) const override; 

    void Add(const T& item)    override;
};

#include "../../sources/ComplexSequences/SortedArray.tpp"

#endif /* SORTED_ARRAY_HPP */