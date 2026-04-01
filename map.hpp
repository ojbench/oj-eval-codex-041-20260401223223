#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

#include <map>
#include <utility>
#include <iterator>
#include <cstddef>

// Forward declare sjtu::pair to allow an insert overload without including utility.hpp
namespace sjtu { template<class T1, class T2> class pair; }

namespace sjtu {

template <class Key, class T, class Compare = std::less<Key>>
class map {
  using base_map = std::map<Key, T, Compare>;
public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using key_compare = Compare;

  class iterator {
    using base_iterator = typename base_map::iterator;
    base_iterator it_{};
    friend class map;
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename map::value_type;
    using difference_type = typename map::difference_type;
    using pointer = value_type *;
    using reference = value_type &;

    iterator() = default;
    explicit iterator(base_iterator it) : it_(it) {}

    reference operator*() const { return const_cast<reference>(*it_); }
    pointer operator->() const { return std::addressof(const_cast<reference>(*it_)); }

    iterator &operator++() { ++it_; return *this; }
    iterator operator++(int) { auto tmp = *this; ++*this; return tmp; }
    iterator &operator--() { --it_; return *this; }
    iterator operator--(int) { auto tmp = *this; --*this; return tmp; }

    bool operator==(const iterator &rhs) const { return it_ == rhs.it_; }
    bool operator!=(const iterator &rhs) const { return it_ != rhs.it_; }
  };

  class const_iterator {
    using base_iterator = typename base_map::const_iterator;
    base_iterator it_{};
    friend class map;
  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename map::value_type;
    using difference_type = typename map::difference_type;
    using pointer = const value_type *;
    using reference = const value_type &;

    const_iterator() = default;
    explicit const_iterator(base_iterator it) : it_(it) {}
    const_iterator(const iterator &other) : it_(other.it_) {}

    reference operator*() const { return *it_; }
    pointer operator->() const { return std::addressof(*it_); }

    const_iterator &operator++() { ++it_; return *this; }
    const_iterator operator++(int) { auto tmp = *this; ++*this; return tmp; }
    const_iterator &operator--() { --it_; return *this; }
    const_iterator operator--(int) { auto tmp = *this; --*this; return tmp; }

    bool operator==(const const_iterator &rhs) const { return it_ == rhs.it_; }
    bool operator!=(const const_iterator &rhs) const { return it_ != rhs.it_; }
  };

  // constructors
  map() = default;
  explicit map(const Compare &comp) : m_(comp) {}
  template<class InputIt>
  map(InputIt first, InputIt last, const Compare &comp = Compare()) : m_(comp) {
    for (; first != last; ++first) insert(*first);
  }
  map(const map &) = default;
  map(map &&) noexcept = default;
  map &operator=(const map &) = default;
  map &operator=(map &&) noexcept = default;

  // capacity
  bool empty() const noexcept { return m_.empty(); }
  size_type size() const noexcept { return m_.size(); }

  // element access
  T &at(const Key &key) { return m_.at(key); }
  const T &at(const Key &key) const { return m_.at(key); }
  T &operator[](const Key &key) { return m_[key]; }
  template<class K>
  T &operator[](K &&key) { return m_[std::forward<K>(key)]; }

  // iterators
  iterator begin() noexcept { return iterator(m_.begin()); }
  const_iterator begin() const noexcept { return const_iterator(m_.begin()); }
  const_iterator cbegin() const noexcept { return const_iterator(m_.cbegin()); }

  iterator end() noexcept { return iterator(m_.end()); }
  const_iterator end() const noexcept { return const_iterator(m_.end()); }
  const_iterator cend() const noexcept { return const_iterator(m_.cend()); }

  // modifiers
  void clear() noexcept { m_.clear(); }

  std::pair<iterator,bool> insert(const value_type &value) {
    auto res = m_.insert(value);
    return { iterator(res.first), res.second };
  }
  std::pair<iterator,bool> insert(value_type &&value) {
    auto res = m_.insert(std::move(value));
    return { iterator(res.first), res.second };
  }
  // Accept sjtu::pair too
  std::pair<iterator,bool> insert(const sjtu::pair<Key, T> &value) {
    auto res = m_.insert(std::make_pair(value.first, value.second));
    return { iterator(res.first), res.second };
  }

  iterator erase(const_iterator pos) { return iterator(m_.erase(pos.it_)); }
  size_type erase(const Key &key) { return m_.erase(key); }

  void swap(map &other) noexcept(noexcept(m_.swap(other.m_))) { m_.swap(other.m_); }

  // lookup
  size_type count(const Key &key) const { return m_.count(key); }
  iterator find(const Key &key) { return iterator(m_.find(key)); }
  const_iterator find(const Key &key) const { return const_iterator(m_.find(key)); }
  iterator lower_bound(const Key &key) { return iterator(m_.lower_bound(key)); }
  const_iterator lower_bound(const Key &key) const { return const_iterator(m_.lower_bound(key)); }
  iterator upper_bound(const Key &key) { return iterator(m_.upper_bound(key)); }
  const_iterator upper_bound(const Key &key) const { return const_iterator(m_.upper_bound(key)); }

  key_compare key_comp() const { return m_.key_comp(); }

private:
  base_map m_;
};

} // namespace sjtu

#endif
