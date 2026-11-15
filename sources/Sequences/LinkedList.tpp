#ifndef LINKED_LIST_TPP
#define LINKED_LIST_TPP

#include "../../headers/Sequences/LinkedList.hpp"
#include "../../headers/Iterators/ListIterator.hpp"
#include "../../headers/Exceptions.hpp"
#include <limits>
#include <utility>

/* СОЗДАНИЕ */
template <class T>
LinkedList<T>::LinkedList(const T* items, const int count) : LinkedList() {
    if(count < 0) throw IndexOutOfRange("LinkedList::LinkedList - negative count.");
    if(count > 0 && items == nullptr) throw std::invalid_argument("LinkedList - null items.");

    for(int i = 0; i < count; i++) Append(items[i]);
}

template <class T>
LinkedList<T>::LinkedList() : head(nullptr), tail(nullptr), length(0) {}

// Конструктор копирования: сначала создаём пустой список, затем новые узлы
// с копиями значений. Полученная цепочка не разделяет узлы с исходной
template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : LinkedList() {
    const Node* current = other.head.get();
    while(current != nullptr){
        Append(current->value);
        current = current->next.get();
    }
}

// Конструктор перемещения: передаём владение всей цепочкой через head за O(1)
// Узлы остаются на месте, поэтому невладеющий tail по-прежнему указывает на конец
template <class T>
LinkedList<T>::LinkedList(LinkedList<T>&& other) noexcept
    : head(std::move(other.head)), tail(other.tail), length(other.length) {
    // Перемещение UniquePtr уже обнулило other.head; делаем источник пустым
    other.tail = nullptr;
    other.length = 0;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T>&& other) noexcept {
    if(this != &other) {
        Clear();
        head = std::move(other.head);
        tail = other.tail;
        length = other.length;
        other.tail = nullptr;
        other.length = 0;
    }
    return *this;
}

template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other){
    if(this == &other) return *this;

    LinkedList<T> copy(other);

    head.swap(copy.head);
    std::swap(tail, copy.tail);
    std::swap(length, copy.length);

    return *this;
}

/* УДАЛЕНИЕ */
template <class T>
LinkedList<T>::~LinkedList(){
    Clear();
}

template <class T>
void LinkedList<T>::Clear() noexcept {
    while(head){
        UniquePtr<Node> next = std::move(head->next);
        head = std::move(next);
    }

    tail   = nullptr;
    length = 0;
}

/* ПЕРЕГРУЗКА [] */
template <class T>
T& LinkedList<T>::operator[](const int index){
    if(index < 0 || index >= length) throw IndexOutOfRange("LinkedList::operator[] - index out of range.");
    Node* current = head.get(); 
    for(int i = 0; i < index; i++) current = current->next.get();
    return current->value;
}

template <class T>
const T& LinkedList<T>::operator[](const int index) const{
    if(index < 0 || index >= length) throw IndexOutOfRange("LinkedList::operator[] - index out of range.");
    Node* current = head.get(); 
    for(int i = 0; i < index; i++) current = current->next.get();
    return current->value;
}

/* ИТЕРАТОР */
template <class T>
UniquePtr<Iterator<T>> LinkedList<T>::GetIterator() const {
    return UniquePtr<Iterator<T>>(new ListIterator<T>(*this));
}


/* ОПЕРАЦИИ */
template <class T>
const T& LinkedList<T>::GetFirst() const{
    if(length == 0) throw IndexOutOfRange("LinkedList::GetFirst - empty list.");
    return head.get()->value;
}

template <class T>
const T& LinkedList<T>::GetLast() const{
    if(length == 0) throw IndexOutOfRange("LinkedList::GetLast - empty list.");
    return tail->value;
}

template <class T>
int LinkedList<T>::GetLength() const{
    return length;
}

/* Добавление/удаление */
template <class T>
void LinkedList<T>::InsertAt(const T& item, const int index) {
    if (index < 0 || index > length) throw IndexOutOfRange("LinkedList::InsertAt - Invalid index.");
    if (index == length) {
        Append(item);
        return;
    }
    if (length == (std::numeric_limits<int>::max)())
        throw std::length_error("LinkedList::InsertAt - maximum size reached.");

    UniquePtr<Node> newNode(new Node(item));

    if (index == 0) {
        newNode->next = std::move(head);
        head = std::move(newNode);
    }
    else {
        Node* previous = head.get();

        for (int i = 0; i < index - 1; ++i)
            previous = previous->next.get();

        newNode->next = std::move(previous->next);

        previous->next = std::move(newNode);
    }

    ++length;
}

template <class T>
void LinkedList<T>::Append(const T& item){
    if (length == (std::numeric_limits<int>::max)())
        throw std::length_error("LinkedList::Append - maximum size reached.");
    UniquePtr<Node> newNode(new Node(item));
    Node* inserted = newNode.get();
    if (tail) tail->next = std::move(newNode);
    else head = std::move(newNode);
    tail = inserted;
    ++length;
}

template <class T>
void LinkedList<T>::Prepend(const T& item){
    InsertAt(item, 0);
}

template <class T>
void LinkedList<T>::RemoveAt(const int index) {
    if (index < 0 || index >= length)
        throw IndexOutOfRange("Invalid index.");

    if (index == 0) {
        UniquePtr<Node> next = std::move(head->next);
        head = std::move(next);

        if (length == 1)
            tail = nullptr;
    }
    else {
        Node* previous = head.get();

        for (int i = 0; i < index - 1; ++i)
            previous = previous->next.get();

        UniquePtr<Node> removed =
            std::move(previous->next);

        previous->next = std::move(removed->next);

        if (index == length - 1)
            tail = previous;
    }

    --length;
}

#endif /* LINKED_LIST_TPP */
