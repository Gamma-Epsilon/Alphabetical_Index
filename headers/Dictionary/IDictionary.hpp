#ifndef IDICTIONARY_HPP
#define IDICTIONARY_HPP

template <class K, class V> 
class IDictionary {
public:
    virtual ~IDictionary() = default; 

    /* Операции */
    virtual V&       Get(const K& key) = 0;
    virtual const V& Get(const K& key) const = 0;
    
    virtual int GetCount() const = 0;
    virtual bool ContainsKey(const K& key) const = 0; 
    
    virtual void Add(const K& key, const V& value) = 0; 
    virtual void Remove(const K& key) = 0; 
};

#endif /* IDICTIONARY_HPP */