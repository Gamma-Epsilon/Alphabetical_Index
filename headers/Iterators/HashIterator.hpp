#ifndef HASH_ITERATOR_HPP
#define HASH_ITERATOR_HPP

#include "Iterator.hpp"
#include "../KeyValue.hpp"
#include "../BaseSequences/DynamicArray.hpp"
#include "../BaseSequences/LinkedList.hpp"

template <class K, class V> 
class HashIterator : public Iterator<KeyValue<K, V>> {
private: 
    const DynamicArray<LinkedList<KeyValue<K, V>>>& arrayData; 
    int bucketIndex; // Номер текущей обрабатываемой корзины
    UniquePtr<Iterator<KeyValue<K, V>>> listIterator; // Элементы в корзине
    
    void MoveToNonEmptyBucket();  // Переход к следующей непустой корзине
public: 
    HashIterator(const DynamicArray<LinkedList<KeyValue<K, V>>>& source);

    bool HasNext() const override;
    const KeyValue<K, V>& Next() override; 

};

#include "../../sources/Iterators/HashIterator.tpp"

#endif /* HASH_ITERATOR_HPP */