#ifndef UNIQUE_PTR_TPP
#define UNIQUE_PTR_TPP

#include "../../headers/Pointers/UniquePtr.hpp"

/* UniquePtr для одиночного объекта */
/* Создание и удаление */
template <class T>
UniquePtr<T>::UniquePtr(T* p) : ptr(p) {}

template <class T>
UniquePtr<T>::~UniquePtr() {
    delete ptr;
}

/* Перемещение */
template <class T>
UniquePtr<T>::UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
}

template <class T>
UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
        delete ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }
    return *this;
}

/* Доступ к объекту */
template <class T>
T& UniquePtr<T>::operator*() const {
    return *ptr;
}

template <class T>
T* UniquePtr<T>::operator->() const {
    return ptr;
}

template <class T>
T* UniquePtr<T>::get() const {
    return ptr;
}

template <class T>
UniquePtr<T>::operator bool() const {
    return ptr != nullptr;
}

/* Управление владением */
template <class T>
T* UniquePtr<T>::release() {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
}

template <class T>
void UniquePtr<T>::reset(T* p) {
    if (ptr != p) {
        delete ptr;
        ptr = p;
    }
}

template <class T>
void UniquePtr<T>::swap(UniquePtr& other) {
    T* tmp    = ptr;
    ptr       = other.ptr;
    other.ptr = tmp;
}

/* UniquePtr для динамического массива */
/* Создание и удаление */
template <class T>
UniquePtr<T[]>::UniquePtr(T* p) : ptr(p) {}

template <class T>
UniquePtr<T[]>::~UniquePtr() {
    delete[] ptr;
}

/* Перемещение */
template <class T>
UniquePtr<T[]>::UniquePtr(UniquePtr&& other) noexcept : ptr(other.ptr) {
    other.ptr = nullptr;
}

template <class T>
UniquePtr<T[]>& UniquePtr<T[]>::operator=(UniquePtr&& other) noexcept {
    if (this != &other) {
        delete[] ptr;
        ptr = other.ptr;
        other.ptr = nullptr;
    }
    return *this;
}

/* Доступ к элементам массива */
template <class T>
T& UniquePtr<T[]>::operator[](std::size_t index) const {
    return ptr[index];
}

template <class T>
T* UniquePtr<T[]>::get() const {
    return ptr;
}

template <class T>
UniquePtr<T[]>::operator bool() const {
    return ptr != nullptr;
}

/* Управление владением */
template <class T>
T* UniquePtr<T[]>::release() {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
}

template <class T>
void UniquePtr<T[]>::reset(T* p) {
    if (ptr != p) {
        delete[] ptr;
        ptr = p;
    }
}

template <class T>
void UniquePtr<T[]>::swap(UniquePtr& other) {
    T* tmp = ptr;
    ptr = other.ptr;
    other.ptr = tmp;
}

#endif /* UNIQUE_PTR_TPP */