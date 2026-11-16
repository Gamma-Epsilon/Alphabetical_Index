#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include "ISequence.hpp"

/* Предварительное объявление */
template <class T>
class ListIterator;

template <class T>
class LinkedList : public ISequence<T> {
private:
    friend class ListIterator<T>;  // Для использования итератором

    struct Node {
        T value;
        UniquePtr<Node> next;
        explicit Node(const T& val) : value(val), next(nullptr) {}
    };

    UniquePtr<Node> head;
    Node* tail;  // Невладеющий указатель. Можно обращаться, но не отвечает за освобождение памяти
    int   length;
    
/* head владеет первым узлом, а tail лишь указывает */

public: 
    /* Создание объекта */
    LinkedList(const T* items, const int count);       
    LinkedList();                 

    /* Конструктор копирования */
    LinkedList(const LinkedList<T>& list); 

    /* Конструктор перемещения */
    LinkedList(LinkedList<T>&& other) noexcept;
    
    /* Удаление объекта */
    ~LinkedList() override;

    /* Перегрузка [] */
    T& operator[](const int index) override;
    const T& operator[](const int index) const override;

    /* Копирующее присваивание */
    LinkedList<T>& operator=(const LinkedList<T>& other);

    /* Перемещающее присваивание */
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept;

    /* Итераторы */
    UniquePtr<Iterator<T>> GetIterator() const override;

    /* Операции */
    const T& GetFirst() const override;
    const T& GetLast()  const override;
    int GetLength()     const override;
    
    void Append(const T& item)  override;
    void Prepend(const T& item) override;
    void InsertAt(const T& item, const int index) override;
    void RemoveAt(const int index) override;
    
};

#include "../../sources/BaseSequences/LinkedList.tpp"

#endif /* LINKED_LIST_HPP */
