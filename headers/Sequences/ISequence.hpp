#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "../Exceptions.hpp"
#include "../Iterators/Iterator.hpp"
#include "../Pointers/UniquePtr.hpp"

template <class T>
class ISequence{
public: 
    /* Удаление объекта */
    virtual ~ISequence() = default; 

    /* Доступ к существующим элементам */
    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;

    /* Операции */
    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast()  const = 0;
    virtual int GetLength()     const = 0;

    /* Итератор */
    virtual UniquePtr<Iterator<T>> GetIterator() const = 0;

    /* Операции */
    virtual void Append(const T& item)  = 0;
    virtual void Prepend(const T& item) = 0;
    virtual void InsertAt(const T& item, int index) = 0;
    virtual void RemoveAt(int index) = 0;
};


#endif /* SEQUENCE_HPP */
