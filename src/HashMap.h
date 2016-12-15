#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <functional>
#include <array>

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
  class SinglyLinkedList;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

  HashMap() : size(0)
  {}

  HashMap(std::initializer_list<value_type> list) : size(0)
  {
    for(auto element: list)
      operator[](element.first) = element.second;
  }

  HashMap(const HashMap& other) : size(other.size)
  {
    for(int i = 0; i < NO_OF_BUCKETS; i++)
      buckets[i] = other.buckets[i];
  }

  HashMap(HashMap&& other) : HashMap()
  {
    swap(*this, other);
  }

  HashMap& operator=(HashMap other)
  {
    if(&other == this)
      return *this;
    swap(*this, other);
    return *this;
  }

  void swap(HashMap& first, HashMap& second)
  {
    using std::swap;
    swap(first.buckets, second.buckets);
    swap(first.size, second.size);
  }

  ~HashMap()
  {

  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    /*
     ASK - is code with try&catch slower?

    try {
      return buckets[getHash(key)].getDataForKey(key).second;
    } catch(std::out_of_range e) {
      size++;
      return buckets[getHash(key)].append(key);
    }*/

    if(buckets[getHash(key)].isKeyPresent(key))
      return buckets[getHash(key)].getDataForKey(key).second;
    if(isEmpty() || key < smallestKey)
      smallestKey = key;
    size++;
    return buckets[getHash(key)].append(key);
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if(isEmpty())
      throw std::out_of_range("empty map, cannot get value");

    return buckets[getHash(key)].getDataForKey(key).second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(isEmpty())
      throw std::out_of_range("empty map, cannot get value");

    return buckets[getHash(key)].getDataForKey(key).second;
  }

  const_iterator find(const key_type& key) const
  {
    if(isEmpty())
      return cend();
    return search(key);
  }

  iterator find(const key_type& key)
  {
    if(isEmpty())
      return end();
    return search(key);
  }

  void remove(const key_type& key)
  {
    if(isEmpty())
      throw std::out_of_range("cannot remove, empty list");

    buckets[getHash(key)].remove(key);
    size--;
    if(smallestKey == key && !isEmpty())
      smallestKey = getSmallestKey();
  }

  void remove(const const_iterator& it)
  {
    if(it.currentKeyBucket == -1)
      throw std::out_of_range("cannot remove end");
    remove(it.currentKey);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    if(size != other.size)
      return false;

    for(auto element : other) {
      auto foundEl = find(element.first);
      if(foundEl == cend() || foundEl->second != element.second)
        return false;
    }

    return true;
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
      key_type key = smallestKey;
      return ConstIterator(key, *this);
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

private:
  static const int NO_OF_BUCKETS = 16000;
  std::array<SinglyLinkedList, NO_OF_BUCKETS> buckets;
  size_type size;
  key_type smallestKey;


  key_type getSmallestKey() const
  {
    key_type smallestKey = 0;
    bool wasAnyKeyFound = false;
    if(isEmpty())
      throw std::invalid_argument("empty map has no smallest element");
    for(int i = 0; i < NO_OF_BUCKETS; i++)
     if(!buckets[i].isEmpty()) {
       for(auto bucketElement = buckets[i].begin(); bucketElement != buckets[i].end(); ++bucketElement) {
        if(!wasAnyKeyFound) {
          smallestKey = (*bucketElement).first;
          wasAnyKeyFound = true;
        } else if((*bucketElement).first < smallestKey)
          smallestKey = (*bucketElement).first;
       }
     }
    return smallestKey;
  }

  key_type getBiggestKey() const
  {
    key_type biggestKey = 0;
    bool wasAnyKeyFound = false;
    if(isEmpty())
      throw std::invalid_argument("empty map has no smallest element");
    for(int i = 0; i < NO_OF_BUCKETS; i++)
     if(!buckets[i].isEmpty()) {
       for(auto bucketElement = buckets[i].begin(); bucketElement != buckets[i].end(); ++bucketElement) {
        if(!wasAnyKeyFound) {
          biggestKey = (*bucketElement).first;
          wasAnyKeyFound = true;
        } else if((*bucketElement).first > biggestKey)
          biggestKey = (*bucketElement).first;
       }
     }
    return biggestKey;
  }

  key_type getNext(const key_type &key) const
  {
    if(isEmpty())
      throw std::invalid_argument("empty map has no elements");

    for(int i = getHash(key)+1; i < NO_OF_BUCKETS; i++) {
    (void)i;
      if(!buckets[i].isEmpty())
        return (*buckets[i].begin()).first;

    }
    return key;
  }

  key_type getPrevious(const key_type &key) const
  {
   if(isEmpty())
      throw std::invalid_argument("empty map has no elements");

    for(int i = getHash(key)-1; i > 0; i--) {
      if(!buckets[i].isEmpty())
        return (*buckets[i].getLastElementIterator()).first;

    }
    return key;
  }

  const_iterator search(const key_type& key) const
  {
    auto it = ConstIterator(key, *this);
    try {
      *it;
    } catch(std::out_of_range e) {
      return cend();
    }
    return it;
  }

  int getHash(const key_type &key) const
  {
    return std::hash<key_type>{}(key) % NO_OF_BUCKETS;
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

  friend class HashMap;
private:
  key_type currentKey;
  int currentKeyBucket;
  const HashMap& iteratorsHashMap;

public:
  explicit ConstIterator(key_type currentKey, int currentKeyBucket, const HashMap& iteratorsHashMap)
   : currentKey(currentKey), currentKeyBucket(currentKeyBucket), iteratorsHashMap(iteratorsHashMap)
  {}
  explicit ConstIterator(key_type currentKey, const HashMap& iteratorsHashMap)
   : currentKey(currentKey), currentKeyBucket(iteratorsHashMap.getHash(currentKey)), iteratorsHashMap(iteratorsHashMap)
  {}

  ConstIterator& operator++()
  {
    if(currentKeyBucket == -1)
       throw std::out_of_range("cannot increment end");

    auto it = iteratorsHashMap.buckets[currentKeyBucket].getIteratorForKey(currentKey);
    if(++it != iteratorsHashMap.buckets[currentKeyBucket].end()) {
      currentKey = (*it).first;
      currentKeyBucket = iteratorsHashMap.getHash(currentKey);
      return *this;
    }
    key_type previousKey = currentKey;
    currentKey = iteratorsHashMap.getNext(currentKey);
    if(currentKey == previousKey) { // no greater key found, so current element is the last one
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
    if(*this == iteratorsHashMap.begin())
      throw std::out_of_range("cannot decrement begin, empty list");
    if(currentKeyBucket == -1) {
      key_type biggestKey = iteratorsHashMap.getBiggestKey();
      this->currentKeyBucket = iteratorsHashMap.getHash(biggestKey);
      this->currentKey = biggestKey;
      return *this;
    }

    auto it = iteratorsHashMap.buckets[currentKeyBucket].getIteratorForKey(currentKey);
    if(it != iteratorsHashMap.buckets[currentKeyBucket].begin()) {
      it = iteratorsHashMap.buckets[currentKeyBucket].getIteratorBefore(it);
      currentKey = (*it).first;
      currentKeyBucket = iteratorsHashMap.getHash(currentKey);
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
    if(currentKeyBucket == -1)
      throw std::out_of_range("cannot dereference end");
    return iteratorsHashMap.buckets[currentKeyBucket].getDataForKey(currentKey);
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    if(currentKeyBucket == -1 && currentKeyBucket == other.currentKeyBucket) // empty list
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

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::SinglyLinkedList
{
public:
  class BucketIterator;

  SinglyLinkedList() : head(new Node())
  {
    tail = head;
  }

  SinglyLinkedList(const SinglyLinkedList& other) : SinglyLinkedList()
  {
    auto it = other.begin();
    while(it != other.end())
    {
      append((*it).first) = (*it).second;
      ++it;
    }
  }

  SinglyLinkedList& operator=(SinglyLinkedList other)
  {
   if(&other == this)
      return *this;
    swap(*this, other);
      return *this;
  }

  ~SinglyLinkedList()
  {
    auto it = begin(), prevIt = begin();
    while(it != end()) {
      prevIt = it;
      ++it;
      delete prevIt.currentNode;
    }
    delete head;
  }

  mapped_type& append(const key_type &key)
  {
    DataNode *newNode = new DataNode(key);
    tail->next = newNode;
    tail = newNode;
    return newNode->data.second;
  }

  void remove(const key_type &key)
  {
    bool wasFound = false;
    auto iteratorBeforeRemElem = begin(), iteratorToRemElem = begin();
    for( ; iteratorToRemElem != end(); ++iteratorToRemElem) {
      if((*iteratorToRemElem).first == key) {
        wasFound = true;
        break;
      }
      iteratorBeforeRemElem = iteratorToRemElem;
    }

    if(!wasFound)
      throw std::out_of_range("cannot remove, element does not exist");

    if(iteratorToRemElem.currentNode == tail) {
      tail = iteratorBeforeRemElem.currentNode;
      tail->next = nullptr;
    }
    if(head->next == iteratorToRemElem.currentNode) {
      head->next = iteratorToRemElem.currentNode->next;
      if(head->next == nullptr)
        tail = head;
    } else
      iteratorBeforeRemElem.currentNode->next = iteratorToRemElem.currentNode->next;

    delete iteratorToRemElem.currentNode;
  }

  bool isEmpty() const
  {
    return head->next == nullptr;
  }

  value_type& getDataForKey(const key_type &key) const
  {
    for(auto it = begin(); it != end(); ++it)
      if((*it).first == key)
        return *it;
    throw std::out_of_range("element with given key does not exist");
  }

  bool isKeyPresent(const key_type &key) const
  {
    if(isEmpty())
      return false;
    for(auto it = begin(); it != end(); ++it)
      if((*it).first == key)
        return true;
    return false;
  }

  void swap(SinglyLinkedList& first, SinglyLinkedList& second)
  {
    using std::swap;
    swap(first.head, second.head);
    swap(first.tail, second.tail);
  }

  BucketIterator getIteratorForKey(const key_type &key) const
  {
    for(auto it = begin(); it != end(); ++it)
      if((*it).first == key)
        return it;
    throw std::out_of_range("element with given key does not exist");
  }

  BucketIterator getIteratorBefore(BucketIterator &givenIt) const
  {
    auto prevIt = begin();
    for(auto it = begin(); it != end(); ++it)
    {
      prevIt = it;
      if(it == givenIt)
        return prevIt;
    }
    return prevIt;
  }

  BucketIterator getLastElementIterator() const
  {
    auto prevIt = begin();
    for(auto it = begin(); it != end(); ++it)
      prevIt = it;

    return prevIt;
  }

  BucketIterator begin() const
  {
    return BucketIterator(head->next);
  }

  BucketIterator end() const
  {
    return BucketIterator(nullptr);
  }

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

  class BucketIterator
  {
  public:
    friend class SinglyLinkedList;

    explicit BucketIterator(Node *startNode) : currentNode(startNode) {}

    BucketIterator& operator++()
    {
      if(currentNode != nullptr)
        currentNode = currentNode->next;
      else
        currentNode = nullptr;
      return *this;
    }

    value_type& operator*() const
    {
      if(currentNode == nullptr)
        throw std::invalid_argument("element does not exist");
      if(DataNode *currentData = dynamic_cast<DataNode*>(currentNode))
        return currentData->data;
      throw std::invalid_argument("cannot dereference nondata node");
    }

    bool operator!=(const BucketIterator& other) const
    {
      return currentNode != other.currentNode;
    }


    bool operator==(const BucketIterator& other) const
    {
      return currentNode == other.currentNode;
    }

  private:
    Node *currentNode;

  };




};

}

#endif /* AISDI_MAPS_HASHMAP_H */
