#ifndef ARRAY_ITERATOR_TPP
#define ARRAY_ITERATOR_TPP

#include "../../headers/Iterators/ArrayIterator.hpp"

template <class T>
ArrayIterator<T>::ArrayIterator(const DynamicArray<T>& source)
    : data(source), index(0) {}

template <class T>
bool ArrayIterator<T>::HasNext() const {
    return index < data.GetLength();
}

template <class T>
const T& ArrayIterator<T>::Current() const {
    if(!HasNext()) throw IndexOutOfRange("ArrayIterator::Current - no element.");
    return data[index];
}

template <class T>
void ArrayIterator<T>::Next() {
    if(!HasNext()) throw IndexOutOfRange("ArrayIterator::Next - no next element.");
    index++;
}

#endif /* ARRAY_ITERATOR_TPP */
