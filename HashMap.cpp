#ifndef HASH_MAP_CPP
#define HASH_MAP_CPP

#include "HashMap.h"
#include <iostream>
#include <string> // std::hash

template <typename K, typename V>
inline HashMap<K, V>::HMNode::HMNode(K _key, V* _value, size_t _hash)
    : key(_key), value(_value), hash(_hash), next(nullptr)
{}

template <typename K, typename V>
HashMap<K, V>::HMNode::~HMNode()
{
    delete value;
}

template <typename K, typename V>
int HashMap<K, V>::HMNode::Insert(const K& key, V* value, size_t hash)
{
    HMNode* current = this;
    int depth = 1;

    while (true)
    {
        if (current->hash == hash && current->key == key)
            return -1;

        if (current->next == nullptr)
            break;

        current = current->next;
        ++depth;
    }

    current->next = new HMNode(key, value, hash);
    return depth;

}


template <typename K, typename V>
HashMap<K, V>::HashMap()
    : bucketCount(MIN_BUCKET_COUNT)
{
    buckets = new HMNode*[MIN_BUCKET_COUNT]();
}

template <typename K, typename V>
HashMap<K, V>::~HashMap()
{
    Clear();
}

template <typename K, typename V>
void HashMap<K, V>::Clear()
{
    for(int i = 0; i < bucketCount; ++i)
    {
        if(buckets[i] == nullptr)
            continue;
        HMNode* current = buckets[i];
        HMNode* last;
        while(current != nullptr)
        {
            last = current;
            current = current->next;
            delete last;
            last = nullptr;
        }
        buckets[i] = nullptr;
    }
    delete[] buckets;
    buckets = nullptr;
}

template <typename K, typename V>
bool HashMap<K, V>::Insert(const K& key, V* value)
{
    size_t h = std::hash<K>{}(key);
    int bucket = h % bucketCount;
    if (buckets[bucket] == nullptr)
    {
        buckets[bucket] = new HMNode(key, value, h);
        return true;
    }
    else
    {
        int nodeLength = buckets[bucket]->Insert(key, value, h);
        if (nodeLength >= MAX_NODE_LENGTH)
        {
            ReHash();
        }
        return nodeLength != -1;
    }
}

template <typename T>
T CreateEmptyObject() {
    return T();
}

template <typename K, typename V>
V* HashMap<K, V>::Find(const K key)
{
    size_t h = std::hash<K>{}(key);
    int bucket = h % bucketCount;
    if(buckets[bucket] == nullptr)
        return nullptr;
    HMNode* current = buckets[bucket];
    if(current->key == key)
        return current->value;
    while(current != nullptr)
    {
        if(current->key == key)
            return current->value;
        current = current->next;
    }
    return nullptr;
}

template <typename K, typename V>
void HashMap<K, V>::ReHash()
{
    int oldSize = bucketCount;
    int newSize = bucketCount * 2;
    HMNode** newBuckets = new HMNode*[newSize] {nullptr};
    for(int i = 0; i < oldSize; ++i)
    {
        if(buckets[i] == nullptr)
            continue;
        HMNode* current = buckets[i];
        while(current != nullptr)
        {
            int bucket = current->hash % newSize;
            if(newBuckets[bucket] == nullptr)
            {
                newBuckets[bucket] = new HMNode(current->key, current->value, current->hash);
            }
            else
            {
                newBuckets[bucket]->Insert(current->key, current->value, current->hash);
            }
            current = current->next;
        }
    }
    Clear();
    bucketCount = newSize;
    buckets = newBuckets;
}


#endif