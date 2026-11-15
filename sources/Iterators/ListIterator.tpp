#ifndef LIST_ITERATOR_TPP
#define LIST_ITERATOR_TPP

#include "../../headers/Iterators/ListIterator.hpp"

template <class T>
ListIterator<T>::ListIterator(const LinkedList<T>& source)
    : nextNode(source.head.get()) {}

template <class T>
bool ListIterator<T>::HasNext() const {
    return nextNode != nullptr;
}

template <class T>
const T& ListIterator<T>::Next() {
    if(!HasNext()) throw IndexOutOfRange("ListIterator::Next - no next element.");
    
    const T& result = nextNode->value;
    nextNode = nextNode->next.get(); 

    return result;
}

#endif /* LIST_ITERATOR_TPP */
