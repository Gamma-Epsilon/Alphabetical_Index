#ifndef ISORTED_SEQUENCE_HPP
#define ISORTED_SEQUENCE_HPP

enum class SearchMode {
    Half,
    Golden
};

template <class T> 
class ISortedSequence {
public: 
    /* Удаление объекта */
    virtual ~ISortedSequence() = default; 

    /* Доступ к существующим элементам */
    virtual const T& operator[](int index) const = 0;
    virtual T& operator[](int index) = 0;

    /* Операции */
    virtual int GetLength() const = 0;
    virtual int Index(const T& item, SearchMode mode) const = 0; 
    
    virtual void Add(const T& item) = 0;
};

#endif /* ISORTED_SEQUENCE_HPP */