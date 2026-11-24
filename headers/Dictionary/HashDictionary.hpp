#ifndef HASH_DICTIONARY_HPP
#define HASH_DICTIONARY_HPP

#include "IDictionary.hpp"
#include "../BaseSequences/LinkedList.hpp"
#include "../Iterators/ListIterator.hpp"
#include "../BaseSequences/DynamicArray.hpp"
#include "../KeyValue.hpp"
#include "../Exceptions.hpp"


template <class K, class V> 
class HashDictionary : public IDictionary<K, V> {
private: 
    DynamicArray<LinkedList<KeyValue<K, V>>> data; 
    int count;  // Кол-во значений

    int GetBucketIndex(const K& key) const;

public: 
    /* Создание объекта */
    explicit HashDictionary(const int bucketCount);
     
    /* Копирование */
    HashDictionary(const HashDictionary<K, V>& other) = default;
    HashDictionary<K, V>& operator=(const HashDictionary<K, V>& other) = default; 

    /* Перемещение */
    HashDictionary(HashDictionary<K, V>&& other) noexcept;
    HashDictionary<K, V>& operator=(HashDictionary<K, V>&& other) noexcept; 

    /* Удаление объекта */
    ~HashDictionary() = default; 

    /* Итератор */
    UniquePtr<Iterator<KeyValue<K, V>>> GetIterator() const override;

    /* Операции */
    V& Get(const K& key) override;
    const V& Get(const K& key) const override;

    int  GetCount() const override;
    bool ContainsKey(const K& key) const override; 
    
    void Add(const K& key, const V& value) override; 
    void Remove(const K& key) override; 
};

#include "../../sources/Dictionary/HashDictionary.tpp"

#endif /* HASH_DICTIONARY_HPP */