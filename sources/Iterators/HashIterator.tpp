#ifndef HASH_ITERATOR_TPP
#define HASH_ITERATOR_TPP

#include "../../headers/Iterators/HashIterator.hpp"

template <class K, class V> 
void HashIterator<K, V>::MoveToNonEmptyBucket() {
    while(bucketIndex < arrayData.GetLength() && arrayData[bucketIndex].GetLength() == 0) bucketIndex++;

    if(bucketIndex < arrayData.GetLength()) {
        listIterator = arrayData[bucketIndex].GetIterator();
    } else {
        listIterator.reset();
    }
}


template <class K, class V> 
HashIterator<K, V>::HashIterator(const DynamicArray<LinkedList<KeyValue<K, V>>>& source) : arrayData(source), bucketIndex(0), listIterator(nullptr) {
    MoveToNonEmptyBucket();
}

template <class K, class V> 
bool HashIterator<K, V>::HasNext() const {
    return listIterator && listIterator->HasNext();
}

/* Условие-инвариант: 
- После создания HashIterator и после каждого вызова Next()
listIterator всегда указывает на список, где есть элемент, либо становится пустым
*/

template <class K, class V> 
const KeyValue<K, V>& HashIterator<K, V>::Next() {
    if(!HasNext()) throw IteratorDoesNotHaveNext("HashIterator::Next - Iterator has no next element.");

    const KeyValue<K, V>& result = listIterator->Next();

    /* Уничтожение итератора не уничтожает узел, так как указатель невладеющий 
    => ссылка result всегда действительная
    */

    if(!listIterator->HasNext()) {
        bucketIndex++; 
        MoveToNonEmptyBucket();
    }

    return result;
}


#endif /* HASH_ITERATOR_TPP */