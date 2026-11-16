#ifndef ISORTED_SEQUENCE_HPP
#define ISORTED_SEQUENCE_HPP

#include "../Exceptions.hpp"

template <class T> 
class ISortedSequence {
public: 
    /* Удаление объекта */
    virtual ~ISortedSequence() = default; 

    /* Доступ к существующим элементам */
    virtual const T& operator[](int index) const = 0;

    /* Операции */
    virtual int GetLength() const = 0;

    virtual void Add(const T& item) = 0;
    virtual int  Index(const T& item) const = 0;
    
};

#endif /* ISORTED_SEQUENCE_HPP */