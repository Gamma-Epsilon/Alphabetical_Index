#ifndef ARRAY_ITERATOR_HPP
#define ARRAY_ITERATOR_HPP

#include "Iterator.hpp"
#include "../BaseSequences/DynamicArray.hpp"

template <class T>
class ArrayIterator : public Iterator<T> {
private:
    const DynamicArray<T>& data;
    int nextIndex; // Индекс следующего элемента

public:
    ArrayIterator(const DynamicArray<T>& source);

    bool HasNext()  const override;
    const T& Next() override;
};

#include "../../sources/Iterators/ArrayIterator.tpp"

#endif /* ARRAY_ITERATOR_HPP */
