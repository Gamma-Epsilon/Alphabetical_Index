#ifndef SORTED_SEQUENCE_HPP
#define SORTED_SEQUENCE_HPP

#include "ISortedSequence.hpp"
#include "../BaseSequences/DynamicArray.hpp"

template <class T>
class SortedSequence : public ISortedSequence<T> {
private:
    DynamicArray<T> data;

    /* Разновидность бинарного поиска. Возвращаем позицию от 0 до size */
    int LowerBound(const T& item, SearchMode mode) const;
public: 
    /* Создание объекта */
    SortedSequence();
    SortedSequence(const DynamicArray<T>& items);            

    /* Копирование */
    SortedSequence(const SortedSequence<T>& sortedSequence)      = default; 
    SortedSequence<T>& operator=(const SortedSequence<T>& other) = default;

    /* Перемещение */
    SortedSequence(SortedSequence<T>&& other)               noexcept = default;
    SortedSequence<T>& operator=(SortedSequence<T>&& other) noexcept = default;

    /* Удаление объекта */
    ~SortedSequence() = default; 

    /* Доступ к существующим элементам */
    const T& operator[](int index) const override;
    T& operator[](int index) override;
    
    /* Операции */
    int  GetLength() const  override;

    int Index(const T& item, SearchMode mode) const override; 

    void Add(const T& item)    override;
};

#include "../../sources/ComplexSequences/SortedSequence.tpp"

#endif /* SORTED_SEQUENCE_HPP */