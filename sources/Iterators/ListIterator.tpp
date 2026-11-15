#ifndef LIST_ITERATOR_TPP
#define LIST_ITERATOR_TPP

#include "../../headers/Iterators/ListIterator.hpp"

template <class T>
ListIterator<T>::ListIterator(const LinkedList<T>& source)
    : current(source.head.get()) {}

template <class T>
bool ListIterator<T>::HasNext() const {
    return current != nullptr;
}

template <class T>
const T& ListIterator<T>::Current() const {
    if(!HasNext()) throw IndexOutOfRange("ListIterator::Current - no element.");
    return current->value;
}

template <class T>
void ListIterator<T>::Next() {
    if(!HasNext()) throw IndexOutOfRange("ListIterator::Next - no next element.");
    current = current->next.get();
}

#endif /* LIST_ITERATOR_TPP */
