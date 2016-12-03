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
    //key_type key;
    //mapped_type value;
    //value_type data; TODO how to use const key_type?
    std::pair<key_type, mapped_type> data;
    //BinaryNode(BinaryNode *left, BinaryNode *right, BinaryNode *parent) : left(left), right(right), parent(parent) {} // todo is used?
    BinaryNode() {}
  };
  BinaryNode *head;
  size_type size;

  void setup()
  {
    head = new BinaryNode();
    head->left = head;
    head->right = head;
    size = 0;
  }


public:
  TreeMap()
  {
    setup();
  }

  TreeMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  TreeMap(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap(TreeMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap& operator=(const TreeMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  TreeMap& operator=(TreeMap&& other)
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
    (void)key;
    if(size == 0) {
        head->left = new BinaryNode;
        head->left->parent = head;
        head->data = std::make_pair(key, mapped_type{});
        head->right = nullptr;
        size++;
        return head->data.second;
    }
    // TODO implement inserting on ther pos than head
    //BinaryNode *newNode = new BinaryNode(nullptr,nullptr);
    throw std::runtime_error("TODO");
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
    (void)key;
    throw std::runtime_error("TODO");
  }

  iterator find(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
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

  bool operator==(const TreeMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const TreeMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  iterator begin()
  {
    return ConstIterator(head->left);
  }

  iterator end()
  {
    return ConstIterator(head);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(head->left);
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
    throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
    return currentNode->parent->data;
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
