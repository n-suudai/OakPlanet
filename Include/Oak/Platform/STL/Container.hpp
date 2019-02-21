
#pragma once

#include "Oak/Platform/STL/STLAllocator.hpp"

#include <vector>
#include <list>
#include <forward_list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>
#include <queue>
#include <stack>

namespace Oak
{
namespace STL
{

// vector
template <typename T, typename Allocator = StandardAllocator>
using vector = std::vector<T, StdAllocator<T, Allocator>>;

// list
template <typename T, typename Allocator = StandardAllocator>
using list = std::list<T, StdAllocator<T, Allocator>>;

// forward list
template <typename T, typename Allocator = StandardAllocator>
using forward_list = std::forward_list<T, StdAllocator<T, Allocator>>;

// map
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = StandardAllocator>
using map =
  std::map<Key, T, Compare, StdAllocator<std::pair<const Key, T>, Allocator>>;

// multimap
template <typename Key, typename T, typename Compare = std::less<Key>,
          typename Allocator = StandardAllocator>
using multimap = std::multimap<
  Key, T, Compare, StdAllocator<std::pair<const Key, T>, Allocator>>;

// unordered_map
template <typename Key, typename T, typename Hasher = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = StandardAllocator>
using unordered_map = std::unordered_map<
  Key, T, Hasher, KeyEqual, StdAllocator<std::pair<const Key, T>, Allocator>>;

// unordered_multimap
template <typename Key, typename T, typename Hasher = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = StandardAllocator>
using unordered_multimap = std::unordered_multimap<
  Key, T, Hasher, KeyEqual, StdAllocator<std::pair<const Key, T>, Allocator>>;

// set
template <typename Key, typename Compare = std::less<Key>,
          typename Allocator = StandardAllocator>
using set = std::set<Key, Compare, StdAllocator<Key, Allocator>>;

// multiset
template <typename Key, typename Compare = std::less<Key>,
          typename Allocator = StandardAllocator>
using multiset = std::multiset<Key, Compare, StdAllocator<Key, Allocator>>;

// unordered_set
template <typename Key, typename Hasher = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = StandardAllocator>
using unordered_set =
  std::unordered_set<Key, Hasher, KeyEqual, StdAllocator<Key, Allocator>>;

// unordered_set
template <typename Key, typename Hasher = std::hash<Key>,
          typename KeyEqual = std::equal_to<Key>,
          typename Allocator = StandardAllocator>
using unordered_multiset =
  std::unordered_multiset<Key, Hasher, KeyEqual, StdAllocator<Key, Allocator>>;

// deque
template <typename T, typename Allocator = StandardAllocator>
using deque = std::deque<T, StdAllocator<T, Allocator>>;

// queue
template <typename T, typename Allocator = StandardAllocator,
          typename Container = deque<T, Allocator>>
using queue = std::queue<T, Container>;

// priority_queue
template <typename T, typename Allocator = StandardAllocator,
          typename Container = vector<T, Allocator>,
          typename Compare = std::less<typename Container::value_type>>
using priority_queue = std::priority_queue<T, Container, Compare>;

// stack
template <typename T, typename Allocator = StandardAllocator,
          typename Container = deque<T, Allocator>>
using stack = std::stack<T, Container>;

} // namespace STL
} // namespace Oak
