#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <functional>

namespace aisdi
{

template <typename KeyType, typename ValueType>
class HashMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    class SinglyLinkedList
    {
      class Node
      {
      public:
        Node *next;
        Node() : next(nullptr) {};
        virtual ~Node() {}
      };
      class DataNode : public Node
      {
      public:
        value_type data;
        DataNode(const key_type &key) : Node(), data({key, mapped_type{}}) {}

      };
      Node *head;
      Node *tail;
    public:
      SinglyLinkedList() : head(new Node())
      {
        tail = head;
      }
      mapped_type& append(const key_type &key)
      {
        DataNode *newNode = new DataNode(key);
        tail->next = newNode;
        tail = newNode;
        return newNode->data.second;
      }
      bool isEmpty() const
      {
        return head->next == nullptr;
      }
      value_type& getDataForKey(const key_type &key) const
      {
        for(auto it = cbegin(); it != cend(); ++it)
          if((*it).first == key)
            return *it;
        throw std::invalid_argument("element with given key does not exist");
      }
     /*void remove(Iterator &iteratorToRemElem)
      {
        auto iteratorBeforeRemElem = begin();
        while(iteratorBeforeRemElem.currentNode->next != iteratorToRemElem.currentNode)
          iteratorBeforeRemElem++;
        if(iteratorToRemElem.currentNode == tail) {
          tail = iteratorToRemElem.currentNode;
          iteratorToRemElem.currentNode->next = nullptr;
        } else
          iteratorToRemElem.currentNode->next = iteratorToRemElem.currentNode->next;

        delete iteratorToRemElem.currentNode;
      }*/

      class BucketConstIterator
      {
        Node *currentNode;
      public:
        explicit BucketConstIterator(Node *startNode) : currentNode(startNode) {}
        operator++()
        {
          if(currentNode != nullptr)
            currentNode = currentNode->next;
          else
            throw std::out_of_range("current node is nullptr");
        }
        value_type& operator*() const
        {
          if(currentNode == nullptr)
            throw std::invalid_argument("element does not exist");
          if(DataNode *currentData = dynamic_cast<DataNode*>(currentNode))
            return currentData->data;
          // TODO protection against non data node dereference
        }
        operator!=(const BucketConstIterator& other) const
        {
          return currentNode == other.currentNode;
        }
      };
     /* Iterator begin()
      {
        return Iterator(head->next);
      }
      Iterator end()
      {
        return Iterator(tail);
      }*/
      BucketConstIterator cbegin() const
      {
        return BucketConstIterator(head->next);
      }
      BucketConstIterator cend() const
      {
        return BucketConstIterator(tail);
      }
/*
      class Iterator : public BucketConstIterator
      {
        explicit Iterator() {}
        Iterator(const BucketConstIterator& other)
        { }

      };*/
    };

  key_type getSmallestKey() const
  {
    key_type smallestKey = 0;
    bool wasAnyKeyFound = false;
    if(isEmpty())
      throw std::invalid_argument("empty map has no smallest element");

    for(int i = 0; i < NO_OF_BUCKETS; i++)
     if(!buckets[i].isEmpty()) {
       for(auto bucketElement = buckets[i].cbegin(); bucketElement != buckets[i].cend(); ++bucketElement) {
        if(!wasAnyKeyFound) {
          smallestKey = (*bucketElement).first;
          wasAnyKeyFound = true;
        } else if((*bucketElement).first < smallestKey)
          smallestKey = (*bucketElement).first;
       }
     }
    return smallestKey;
  }

  key_type getNext(const key_type &key) const
  {
    key_type nextKey = key;
    bool greaterKeyWasFound = false;
    if(isEmpty())
      throw std::invalid_argument("empty map has no elements");
      for(int i = 0; i < NO_OF_BUCKETS; i++) {
        if(!buckets[i].isEmpty()) {
          for(auto bucketElement = buckets[i].cbegin(); bucketElement != buckets[i].cend(); ++bucketElement) {
            if(!greaterKeyWasFound)
            {
              if((*bucketElement).first > key) {
                nextKey = (*bucketElement).first;
                greaterKeyWasFound = true;
              }
            }
            else
              if((*bucketElement).first > key && (*bucketElement).first < nextKey)
                nextKey = (*bucketElement).first;
          }
        }
      }
     return nextKey;
  }

  key_type getPrevious(const key_type &key) const
  {
    key_type previousKey = key;
    bool smallerKeyWasFound = false;
    if(isEmpty())
      throw std::invalid_argument("empty map has no elements");
    for(int i = 0; i < NO_OF_BUCKETS; i++) {
        if(!buckets[i].isEmpty()) {
          for(auto bucketElement = buckets[i].cbegin(); bucketElement != buckets[i].cend(); ++bucketElement) {
            if(!smallerKeyWasFound)
            {
              if((*bucketElement).first < key) {
                previousKey = (*bucketElement).first;
                smallerKeyWasFound = true;
              }
            }
            else
              if((*bucketElement).first < key && (*bucketElement).first > previousKey)
                previousKey = (*bucketElement).first;
          }
        }
      }
     return previousKey;
  }


  int getHash(const key_type &key) const
  {
    //const int keysHash = (std::hash<key_type>(key)) % NO_OF_BUCKETS;// TODO use std::hash
    return key % NO_OF_BUCKETS;
  }

  static const int NO_OF_BUCKETS = 16000;
  SinglyLinkedList buckets[NO_OF_BUCKETS];
  size_type size;


public:
  HashMap() : size(0)
  {}

  HashMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    // TODO what if current key alreade in HashMap ?
    size++;
    return buckets[getHash(key)].append(key);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    if(isEmpty())
      return cend();
    auto it = ConstIterator(key, getHash(key), *this);
    try
    {
     *it; // todo verify
    }
    catch(std::invalid_argument e)
    {
     return cend();
    }
    return it;
  }

  iterator find(const key_type& key)
  {
    if(isEmpty())
      return end();
    auto it = ConstIterator(key, getHash(key), *this);
    try
    {
     *it; // todo verify
    }
    catch(std::invalid_argument e)
    {
     return end();
    }
    return it;
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return cbegin();
  }

  iterator end()
  {
    return cend();
  }

  const_iterator cbegin() const
  {
    if(isEmpty())
      return ConstIterator(0, -1, *this);
    else {
      key_type key = getSmallestKey();
      return ConstIterator(key, getHash(key), *this); // TODO better solution for empty list ?
    }
  }

  const_iterator cend() const
  {
    return ConstIterator(-1, -1, *this);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

private:
  key_type currentKey;
  int currentKeyBucket;
  const HashMap& iteratorsHashMap;

public:
  explicit ConstIterator(key_type currentKey, int currentKeyBucket, const HashMap& iteratorsHashMap)
   : currentKey(currentKey), currentKeyBucket(currentKeyBucket), iteratorsHashMap(iteratorsHashMap)
  {}
/*
  ConstIterator(const ConstIterator& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }
*/
  ConstIterator& operator++()
  {
    if(currentKeyBucket == -1 && currentKey == -1)
       throw std::out_of_range("cannot increment end");
    key_type previousKey = currentKey;
    currentKey = iteratorsHashMap.getNext(currentKey);
    if(currentKey == previousKey){ // no greater key found, so current element is the last one
      currentKey = -1;
      currentKeyBucket = -1;
    } else
      currentKeyBucket = iteratorsHashMap.getHash(currentKey);
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator oldIterator(*this);
    operator++();
    return oldIterator;
  }

  ConstIterator& operator--()
  {
    if(currentKeyBucket == -1 && currentKey != -1)
       throw std::out_of_range("cannot decrement begin, empty list");

    if(currentKeyBucket == -1 && currentKeyBucket == -1) {
      ConstIterator beginIt = iteratorsHashMap.cbegin();
      this->currentKeyBucket = beginIt.currentKeyBucket;
      this->currentKey = beginIt.currentKey;
      return *this;
    }

    key_type previousKey = currentKey;
    currentKey = iteratorsHashMap.getPrevious(currentKey);
    if(currentKey == previousKey){ // no smaller key found, so current element is the first one
      throw std::out_of_range("cannot decrement begin, nonempty list");
    } else
      currentKeyBucket = iteratorsHashMap.getHash(currentKey);
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator oldIterator(*this);
    operator--();
    return oldIterator;
  }

  reference operator*() const
  {
    if(currentKeyBucket == -1 && currentKey == -1)
      throw std::out_of_range("cannot dereference end");
    return iteratorsHashMap.buckets[currentKeyBucket].getDataForKey(currentKey);
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(currentKeyBucket == -1 && currentKeyBucket == other.currentKeyBucket) // iterators for empty list
      return true;

    return currentKey == other.currentKey;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
