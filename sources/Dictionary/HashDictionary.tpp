#ifndef HASH_DICTIONARY_TPP
#define HASH_DICTIONARY_TPP

#include <utility>
#include <cstddef>

#include "../../headers/Dictionary/HashDictionary.hpp"
#include "../../headers/Iterators/HashIterator.hpp"

static const int HASH_MULTIPLIER = 31; // Коэффициент хеширования

/* Конструкторы */
template <class K, class V>
HashDictionary<K, V>::HashDictionary(const int bucketCount) : data(), count(0) {
    if(bucketCount <= 0) throw IndexOutOfRange("HashDictionary - invalid bucket count"); // = 0 нельзя, иначе деление на ноль в хеш-функции

    // bucket - ячейка словаря, где находятся элементы с одинаковым индексом

    data.Resize(bucketCount);
} 

/* Перемещение */
template <class K, class V>
HashDictionary<K, V>::HashDictionary(HashDictionary<K, V>&& other) noexcept : data(std::move(other.data)), count(other.count) {
    other.count = 0;
} 

template <class K, class V>
HashDictionary<K, V>& HashDictionary<K, V>::operator=(HashDictionary<K, V>&& other) noexcept {
    if(this != &other) {
        data  = std::move(other.data);
        count = other.count;
        other.count = 0; 
    }

    return *this;
} 

/* Итератор */
template <class K, class V>
UniquePtr<Iterator<KeyValue<K, V>>> HashDictionary<K, V>::GetIterator() const {
    return UniquePtr<Iterator<KeyValue<K, V>>>(new HashIterator<K, V>(data));
}

/* Операции */
/* Хеш-функция */
template <class K, class V>
int HashDictionary<K, V>::GetBucketIndex(const K& key) const {
    size_t hash = 0; 

    for(unsigned char c : key) {
        hash = hash * HASH_MULTIPLIER + c; 
    }

    return static_cast<int>(hash % static_cast<size_t>(data.GetLength()));
}

template <class K, class V>
V& HashDictionary<K, V>::Get(const K& key) {
    int   index  = GetBucketIndex(key);
    auto& bucket = data[index];
    // auto - копия, auto& - ссылка на существующий список
    // Неконстантный [] возвращает T&, а не const T&, как итератор

    auto iterator = bucket.GetIterator();
    int  position = 0;

     
    while(iterator->HasNext()) {
        if(iterator->Next().key == key) { return bucket[position].value; }
        position++;
    }

    throw KeyDoesNotExist("HashDictionary::Get - This key is absent in this dictionary");
}

template <class K, class V>
const V& HashDictionary<K, V>::Get(const K& key) const {
    int  index    = GetBucketIndex(key);
    auto iterator = data[index].GetIterator(); 
    while(iterator->HasNext()) { // Next() возвращает const KeyValue<K,V>&. Auto создает копию
        const auto& element = iterator->Next();
        if(element.key == key) { return element.value; }
    }

    throw KeyDoesNotExist("HashDictionary::Get - This key is absent in this dictionary");
}

template <class K, class V>
int HashDictionary<K, V>::GetCount() const { return count; }

template <class K, class V>
bool HashDictionary<K, V>::ContainsKey(const K& key) const {
    int index    = GetBucketIndex(key); 

    auto iterator = data[index].GetIterator(); 
    while(iterator->HasNext()) {
        if(iterator->Next().key == key) { return true; }
    }

    return false;
} 

template <class K, class V>
void HashDictionary<K, V>::Add(const K& key, const V& value) {
    if(ContainsKey(key)) throw KeyAlreadyExists("HashDictionary::Add - This key already is in this dictionary");

    int index = GetBucketIndex(key); 

    KeyValue<K, V> keyValue; 
    keyValue.key   = key; 
    keyValue.value = value;  

    data[index].Append(keyValue); 
    count++;
}

template <class K, class V>
void HashDictionary<K, V>::Remove(const K& key) {
    if(!ContainsKey(key)) throw KeyDoesNotExist("HashDictionary::Remove - This key is absent in this dictionary");

    int dictIndex = GetBucketIndex(key); 
    int listIndex = -1; 

    auto iterator = data[dictIndex].GetIterator(); 
    while(iterator->HasNext()) {
        listIndex++;
        if(iterator->Next().key == key) { break; }
    }

    data[dictIndex].RemoveAt(listIndex);
    count--;
}


#endif /* HASH_DICTIONARY_TPP */