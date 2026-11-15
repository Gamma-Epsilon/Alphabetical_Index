#ifndef ARRAY_ITERATOR_HPP
#define ARRAY_ITERATOR_HPP

#include "Iterator.hpp"
#include "../Sequences/DynamicArray.hpp"

template <class T>
class ArrayIterator : public Iterator<T> {
private:
    const DynamicArray<T>& data;
    int index; // Индекс текущего элемента

public:
    ArrayIterator(const DynamicArray<T>& source);

    bool HasNext() const override;
    const T& Current() const override;
    void Next() override;
};

#include "../../sources/Iterators/ArrayIterator.tpp"

#endif /* ARRAY_ITERATOR_HPP */
