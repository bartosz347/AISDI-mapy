#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iostream> // TODO REMOVE

namespace aisdi
{

template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<key_type, mapped_type>; // TODO removed const from key_type
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private: // TODO move to lower private section
  class BinaryNode {
  public:
    BinaryNode *left;
    BinaryNode *right;
    BinaryNode *parent;

    //value_type data; TODO how to use const key_type?
    std::pair<key_type, mapped_type> data;
    //BinaryNode(BinaryNode *left, BinaryNode *right, BinaryNode *parent) : left(left), right(right), parent(parent) {} // todo is used?
    BinaryNode() {}
  };
  BinaryNode *head; // super head
  size_type size;

  void setup()
  {
    head = new BinaryNode();
    head->left = head; // todo required to detect empty list
    head->right = head; // used for detecting illegal --begin() with empty collection
    head->parent = nullptr;
    size = 0;
  }


public:
  TreeMap()
  {
    setup();
  }

  TreeMap(std::initializer_list<value_type> list)
  {
    setup();
    for(auto element : list)
      operator[](element.first) = element.second;
  }

  TreeMap(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODOs");
  }

  TreeMap(TreeMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODOi");
  }

  TreeMap& operator=(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODOzz");
  }

  TreeMap& operator=(TreeMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODOzzz");
  }

  bool isEmpty() const
  {
    return size == 0;
  }

  mapped_type& operator[](const key_type& key)
  {
    BinaryNode *newNode = new BinaryNode();
    newNode->right = nullptr;
    newNode->left = nullptr;
    newNode->data = std::make_pair(key, mapped_type{});
    if(isEmpty()) {
        newNode->parent = head;


        head->left = newNode; // list no longer empty

        head->right = nullptr; // important for check against decrementing begin() in empty map
        size++;
        return newNode->data.second;
    }
    BinaryNode *next = head->left;
    BinaryNode *current;
    while(next != nullptr) {
        current = next;
        if(key == current->data.first) { //node with this key already exists
            delete newNode;
            return current->data.second;
        }
        if(key < current->data.first)
            next = current->left;
        else
            next = current->right;
    }
    newNode->parent = current;
    if(key < current->data.first)
      current->left = newNode;
    else
      current->right = newNode;
    size++;
    return newNode->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    if(isEmpty())
      throw std::out_of_range("map is empty");
    const_iterator position = find(key);
    if(position == cend())
      throw std::out_of_range("key does not exist");
    return position->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    if(isEmpty())
      throw std::out_of_range("map is empty");
    iterator position = find(key);
    if(position == end())
      throw std::out_of_range("key does not exist");
    return position->second;
  }

  const_iterator find(const key_type& key) const
  {
    if(head == head->left || size == 0)
      return cend();
    BinaryNode *current = head->left;

    while(current != nullptr) {
        if(key == current->data.first)
            return const_iterator(current);
        if(key < current->data.first)
            current = current->left;
        else
            current = current->right;
    }
    return cend();
  }

  iterator find(const key_type& key) // todo combine with const_iterator find
  {
   if(head == head->left || size == 0)
      return end();
    BinaryNode *current = head->left;

    while(current != nullptr) {
        if(key == current->data.first)
            return const_iterator(current);
        if(key < current->data.first)
            current = current->left;
        else
            current = current->right;
    }
    return end();
  }

  void remove(const key_type& key)
  {
    // TODO REWRITE

    if(isEmpty())
      throw std::out_of_range("cannot remove, empty list");
    auto nodeBeingRemoved = find(key).currentNode;
    if(nodeBeingRemoved == head)
      throw std::out_of_range("cannot remove, element does not exist");


    // No children
    if(nodeBeingRemoved->left == nullptr && nodeBeingRemoved->right == nullptr)
    {
      if(nodeBeingRemoved->parent->left == nodeBeingRemoved)
        nodeBeingRemoved->parent->left = nullptr;
      else
        nodeBeingRemoved->parent->right = nullptr;
      if(nodeBeingRemoved->parent == head) head->left = head; // todo required to detect empty list...
    }

    // 1 child
    if(nodeBeingRemoved->left == nullptr ) {
       if(nodeBeingRemoved->parent->left == nodeBeingRemoved)
        nodeBeingRemoved->parent->left = nodeBeingRemoved->right;
        else
        nodeBeingRemoved->parent->right = nodeBeingRemoved->right;

    } else if(nodeBeingRemoved->right == nullptr) {
     if(nodeBeingRemoved->parent->left == nodeBeingRemoved)
        nodeBeingRemoved->parent->left = nodeBeingRemoved->left;
    else
        nodeBeingRemoved->parent->right = nodeBeingRemoved->left;

    }





    delete nodeBeingRemoved;
    size--;
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO2");
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const // todo is there a better way?
  {
    if(size != other.size)
      return false;
    auto otherIt = other.cbegin();
    auto ownIt = cbegin();

    for(size_type i = 0; i < size; i++)
    {
        if(!(otherIt->first == ownIt->first && otherIt->second == ownIt->second))
            return false;
        otherIt++;
        ownIt++;
    }
    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !operator==(other);
  }

  iterator begin() // TODO combine with cbegin() ?
  {
    BinaryNode *leftMostNode = head;
    if(head == head->left) return ConstIterator(head); // empty map
    while(leftMostNode->left != nullptr)
        leftMostNode = leftMostNode->left;
    return ConstIterator(leftMostNode);
  }

  iterator end()
  {
    return ConstIterator(head);
  }

  const_iterator cbegin() const
  {
    BinaryNode *leftMostNode = head;
    if(head == head->left) return ConstIterator(head); // empty map
    while(leftMostNode->left != nullptr)
        leftMostNode = leftMostNode->left;
    return ConstIterator(leftMostNode);
  }

  const_iterator cend() const
  {
    return ConstIterator(head);
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;

  BinaryNode *currentNode;

  explicit ConstIterator()
  {}

  explicit ConstIterator(BinaryNode *startNode) : currentNode(startNode)
  {}

  ConstIterator(const ConstIterator& other) : currentNode(other.currentNode)
  {}

  ConstIterator& operator++()
  {
    if(currentNode->parent == nullptr)
        throw std::out_of_range("Cannot increment end");
    if(currentNode->right != nullptr) {
      currentNode = currentNode->right;
      while(currentNode->left != nullptr)
        currentNode = currentNode->left;
      return *this;
    }
    BinaryNode *tmp = currentNode->parent;
    while(tmp != nullptr && currentNode == tmp->right) {
      currentNode = tmp;
      tmp = tmp->parent;
    }
    currentNode = tmp;
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator old(*this);
    operator++();
    return old;
  }

  ConstIterator& operator--()
  {
    if(currentNode->right == currentNode)
        throw std::out_of_range("Cannot decrement begin, empty map");
    if(currentNode->left != nullptr) {
        currentNode = currentNode->left;
        while(currentNode->right != nullptr)
          currentNode = currentNode->right;
        return *this;
    }
    BinaryNode *tmp = currentNode->parent;
    while(tmp != nullptr && currentNode == tmp->left) {
      if(tmp->parent == nullptr)
        throw std::out_of_range("Cannot decrement begin"); //todo
      currentNode = tmp;
      tmp = tmp->parent;
    }
    currentNode = tmp;
    return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator old(*this);
    operator--();
    return old;
  }

  reference operator*() const
  {
    if(currentNode->right == currentNode)
      throw std::out_of_range("Cannot dereference end");
    return currentNode->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return this->currentNode == other.currentNode;
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

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

#endif /* AISDI_MAPS_MAP_H */
