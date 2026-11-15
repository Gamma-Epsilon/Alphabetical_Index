#ifndef LIST_ITERATOR_HPP
#define LIST_ITERATOR_HPP

#include "Iterator.hpp"
#include "../Sequences/LinkedList.hpp"

template <class T>
class ListIterator : public Iterator<T> {
private:
    const typename LinkedList<T>::Node* nextNode;

public:
    ListIterator(const LinkedList<T>& source);

    bool HasNext()  const override;
    const T& Next() override;
};

#include "../../sources/Iterators/ListIterator.tpp"

#endif /* LIST_ITERATOR_HPP */
