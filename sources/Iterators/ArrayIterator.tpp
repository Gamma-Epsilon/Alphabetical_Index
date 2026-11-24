#ifndef ARRAY_ITERATOR_TPP
#define ARRAY_ITERATOR_TPP

#include "../../headers/Iterators/ArrayIterator.hpp"

template <class T>
ArrayIterator<T>::ArrayIterator(const DynamicArray<T>& source)
    : data(source), nextIndex(0) {}

template <class T>
bool ArrayIterator<T>::HasNext() const {
    return nextIndex < data.GetLength();
}

template <class T>
const T& ArrayIterator<T>::Next() {
    if(!HasNext()) throw IteratorDoesNotHaveNext("ArrayIterator::Next - no next element.");
    
    return data[nextIndex++];
}

#endif /* ARRAY_ITERATOR_TPP */
