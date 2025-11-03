#ifndef HASH_MAP_H
#define HASH_MAP_H
#define MAX_NODE_LENGTH 4
#include <cstddef> // size_t

static constexpr int MIN_BUCKET_COUNT = 16;

template <typename K, typename V>
class HashMap
{
// private internal class
private:
    class HMNode
    {
    public:
        HMNode(K _key, V* _value, size_t hash = 0);
        ~HMNode();
        K key;
        V* value;
        size_t hash;
        HMNode* next;
        int Insert(const K& key, V* value, size_t hash);
    };
public:
    HashMap();
    ~HashMap();
    void Clear();
    bool Insert(const K& key, V* value);
    V* Find(const K key);
    void ReHash();
private:
    int bucketCount;
    HMNode** buckets;
};

#include "HashMap.cpp"
#endif