
#pragma once

#include "Oak/Core/Memory/AllocateConfig.hpp"
#include "Oak/Core/Memory/STLAllocator.hpp"
#include "Oak/Core/Memory/MemoryManager.hpp"

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

#include <memory>

namespace Oak
{

template <typename Policy = AllocatePolicy>
struct STL
{
/*
    ポリシーごとに名前を変える場合の初期化
    template<>
    const Char* STL<PolicyClass>::s_heapName = "HeapName";
*/
#if OAK_USE_HEAP_TRACKING
    static const Char *s_heapName;
    static Heap *s_pHeap;
#endif // OAK_USE_HEAP_TRACKING

    template <typename T>
    struct Allocator
    {
        using value_type = T;

        template <typename U>
        struct rebind
        {
            typedef Allocator<U> other;
        };

        inline Allocator()
        {
        }

        template <typename U>
        inline Allocator(const Allocator<U> &)
        {
        }

        inline T *allocate(SizeT count)
        {
#if OAK_USE_HEAP_TRACKING

            if (s_pHeap == nullptr)
            {
                s_pHeap = HeapFactory::Get().CreateHeap<Policy>(s_heapName);
            }

            return reinterpret_cast<T *>(s_pHeap->AllocateBytesAligned(
              sizeof(T) * count, alignof(T), __FILE__, __LINE__, __FUNCTION__));

#else // OAK_USE_HEAP_TRACKING

            return reinterpret_cast<T *>(
              Policy::AllocateBytesAligned(sizeof(T) * count, alignof(T)));

#endif // OAK_USE_HEAP_TRACKING
        }

        inline Void deallocate(T *ptr, SizeT)
        {
#if OAK_USE_HEAP_TRACKING

            s_pHeap->DeallocateBytesAligned(reinterpret_cast<Void *>(ptr),
                                            alignof(T));

#else // OAK_USE_HEAP_TRACKING

            Policy::DeallocateBytesAligned(reinterpret_cast<Void *>(ptr),
                                           alignof(T));

#endif // OAK_USE_HEAP_TRACKING
        }
    };

    // vector
    template <typename T>
    using vector = std::vector<T, Allocator<T>>;

    // list
    template <typename T>
    using list = std::list<T, Allocator<T>>;

    // forward list
    template <typename T>
    using forward_list = std::forward_list<T, Allocator<T>>;

    // map
    template <typename Key, typename T, typename Compare = std::less<Key>>
    using map = std::map<Key, T, Compare, Allocator<std::pair<const Key, T>>>;

    // multimap
    template <typename Key, typename T, typename Compare = std::less<Key>>
    using multimap =
      std::multimap<Key, T, Compare, Allocator<std::pair<const Key, T>>>;

    // unordered_map
    template <typename Key, typename T, typename Hasher = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>>
    using unordered_map = std::unordered_map<
      Key, T, Hasher, KeyEqual, Allocator<std::pair<const Key, T>>>;

    // unordered_multimap
    template <typename Key, typename T, typename Hasher = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>>
    using unordered_multimap = std::unordered_multimap<
      Key, T, Hasher, KeyEqual, Allocator<std::pair<const Key, T>>>;

    // set
    template <typename Key, typename Compare = std::less<Key>>
    using set = std::set<Key, Compare, Allocator<Key>>;

    // multiset
    template <typename Key, typename Compare = std::less<Key>>
    using multiset = std::multiset<Key, Compare, Allocator<Key>>;

    // unordered_set
    template <typename Key, typename Hasher = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>>
    using unordered_set =
      std::unordered_set<Key, Hasher, KeyEqual, Allocator<Key>>;

    // unordered_set
    template <typename Key, typename Hasher = std::hash<Key>,
              typename KeyEqual = std::equal_to<Key>>
    using unordered_multiset =
      std::unordered_multiset<Key, Hasher, KeyEqual, Allocator<Key>>;

    // deque
    template <typename T>
    using deque = std::deque<T, Allocator<T>>;

    // queue
    template <typename T, typename Container = deque<T>>
    using queue = std::queue<T, Container>;

    // priority_queue
    template <typename T, typename Container = vector<T>,
              typename Compare = std::less<typename Container::value_type>>
    using priority_queue = std::priority_queue<T, Container, Compare>;

    // stack
    template <typename T, typename Container = deque<T>>
    using stack = std::stack<T, Container>;

    // カスタムデリータ unique_ptr用
    template <typename T>
    struct CustomDeleter
    {
        CustomDeleter() = default;

        template <typename T2,
                  std::enable_if_t<std::is_convertible_v<T2 *, T *>, Int32> = 0>
        CustomDeleter(const CustomDeleter<T2> &)
        {   // construct from another
            // default_delete
        }

        Void operator()(T *ptr) const
        {
            static_assert(0 < sizeof(T), "can't delete an incomplete type");

            OAK_DELETE ptr;
        }
    };

    // カスタムデリータ 配列型のunique_ptr用
    template <typename T>
    struct CustomDeleter<T[]>
    {
        CustomDeleter() = default;

        template <
          typename T2,
          std::enable_if_t<std::is_convertible_v<T2 (*)[], T (*)[]>, Int32> = 0>
        CustomDeleter(const CustomDeleter<T2[]> &)
        {   // construct from another
            // default_delete
        }

        template <
          typename T2,
          std::enable_if_t<std::is_convertible_v<T2 (*)[], T (*)[]>, Int32> = 0>
        Void operator()(T2 *ptr) const
        { // delete a pointer
            static_assert(0 < sizeof(T2), "can't delete an incomplete type");

            OAK_DELETE[] ptr;
        }
    };

    // shared_ptr
    template <typename T>
    using shared_ptr = std::shared_ptr<T>;

    // weak_ptr
    template <typename T>
    using weak_ptr = std::weak_ptr<T>;

    // unique_ptr
    template <typename T, typename Deleter = CustomDeleter<T>>
    using unique_ptr = std::unique_ptr<T, Deleter>;

    // make_shared
    // ※s_pHeap を利用して確保される
    template <typename T, typename... Arguments>
    static inline shared_ptr<T> make_shared(Arguments &&... arguments)
    {
        static Allocator<T> allocator = Allocator<T>();
        return std::allocate_shared<T>(allocator,
                                       std::forward<Arguments>(arguments)...);
    }

    // make_unique
    // ※s_pHeap を利用して確保されない (OAK_NEWを使用)
    template <typename T, typename... Arguments,
              std::enable_if_t<!std::is_array_v<T>, Int32> = 0>
    static inline unique_ptr<T> make_unique(Arguments &&... arguments)
    {
        static CustomDeleter<T> deleter = CustomDeleter<T>();

        return unique_ptr<T>(OAK_NEW T(std::forward<Arguments>(arguments)...),
                             deleter);
    }

    // make_unique (array)
    // ※s_pHeap を利用して確保されない (OAK_NEWを使用)
    template <typename T,
              std::enable_if_t<std::is_array_v<T> &&std::extent<T>::value == 0,
                               Int32> = 0>
    static inline unique_ptr<T> make_unique(SizeT size)
    {
        typedef std::remove_extent_t<T> Element;

        static CustomDeleter<T> deleter = CustomDeleter<T>();

        return (unique_ptr<T>(OAK_NEW Element[size](), deleter));
    }

    // make_unique (=delete)
    template <typename T, class... Arguments,
              std::enable_if_t<std::extent<T>::value != 0, Int32> = 0>
    static inline Void make_unique(Arguments &&...) = delete;

    // basic_string
    template <typename Element, typename Traits = std::char_traits<Element>>
    using basic_string = std::basic_string<Element, Traits, Allocator<Element>>;

    // string
    typedef basic_string<Char> string;
    typedef basic_string<WChar> wstring;
    typedef basic_string<TChar> tstring;

    // basic_stringstream
    template <typename Element, typename Traits = std::char_traits<Element>>
    using basic_stringstream =
      std::basic_stringstream<Element, Traits, Allocator<Element>>;

    // stringstream
    typedef basic_stringstream<Char> stringstream;
    typedef basic_stringstream<WChar> wstringstream;
    typedef basic_stringstream<TChar> tstringstream;
};

template <typename Policy, typename T, typename U>
inline Bool operator==(const STL<Policy>::Allocator<T> &,
                       const STL<Policy>::Allocator<U> &)
{
    return true;
}

template <typename Policy, typename T, typename U>
inline Bool operator!=(const STL<Policy>::Allocator<T> &,
                       const STL<Policy>::Allocator<U> &)
{
    return false;
}

#if OAK_USE_HEAP_TRACKING

template <typename Policy>
Heap *STL<Policy>::s_pHeap = nullptr;

template <typename Policy>
const Char *STL<Policy>::s_heapName = "STL";

#endif // OAK_USE_HEAP_TRACKING

} // namespace Oak
