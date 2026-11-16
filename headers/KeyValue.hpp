#ifndef KEY_VALUE_HPP
#define KEY_VALUE_HPP

template <class K, class V>
struct KeyValue {
    K key; 
    V value;

    /* Сравнение */
    bool operator<(const KeyValue<K, V>& other) const {
        return key < other.key;
    }

    bool operator==(const KeyValue<K, V>& other) const {
        return key == other.key;
    }
};


#endif /* KEY_VALUE_HPP */