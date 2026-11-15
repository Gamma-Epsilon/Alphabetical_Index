#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "../Exceptions.hpp"

template <class T>
class Iterator {
public:
    virtual ~Iterator() = default;

    // HasNext: текущий элемент доступен; Next: переход, в том числе за последний
    // Current и Next в конечном состоянии выбрасывают IndexOutOfRange
    virtual bool HasNext() const = 0;
    virtual const T& Current() const = 0;
    virtual void Next() = 0;
};

#endif /* ITERATOR_HPP */
