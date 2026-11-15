#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "../Exceptions.hpp"

/* Для обхода структур в едином интерфейсе */

template <class T>
class Iterator {
public:
    virtual ~Iterator() = default;

    virtual bool HasNext() const = 0;
    virtual const T& Next() = 0;
};

#endif /* ITERATOR_HPP */
