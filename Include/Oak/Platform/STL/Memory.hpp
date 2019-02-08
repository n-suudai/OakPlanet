
#pragma once

#include "Oak/Platform/STL/STLAllocator.hpp"
#include <memory>


namespace Oak {
namespace STL {
namespace Detail {


// カスタムデリータ unique_ptr用
template<typename T>
struct CustomDeleter
{
    CustomDeleter() = default;

    template<
        typename T2,
        std::enable_if_t<std::is_convertible_v<T2 *, T *>, Int32> = 0>
        CustomDeleter(const CustomDeleter<T2>&)
    {// construct from another default_delete
    }

    Void operator()(T* ptr) const
    {
        static_assert(
            0 < sizeof(T),
            "can't delete an incomplete type"
            );

        delete ptr;
    }
};

// カスタムデリータ 配列型のunique_ptr用
template<typename T>
struct CustomDeleter<T[]>
{
    CustomDeleter() = default;

    template<typename T2,
        std::enable_if_t<std::is_convertible_v<T2(*)[], T(*)[]>, Int32> = 0>
        CustomDeleter(const CustomDeleter<T2[]>&)
    {// construct from another default_delete
    }

    template<typename T2,
        std::enable_if_t<std::is_convertible_v<T2(*)[], T(*)[]>, Int32> = 0>
        Void operator()(T2 * ptr) const
    {// delete a pointer
        static_assert(
            0 < sizeof(T2),
            "can't delete an incomplete type"
            );

        delete[] ptr;
    }
};


} // namespace Detail

// shared_ptr
template<typename T>
using shared_ptr = std::shared_ptr<T>;

// weak_ptr
template<typename T>
using weak_ptr = std::weak_ptr<T>;

// unique_ptr
template<
    typename T,
    typename Deleter = Detail::CustomDeleter<T>
>
using unique_ptr = std::unique_ptr<T, Deleter>;


// ※ T::allocator_type が無い場合は、直接指定する
template<typename T, typename Allocator = T::allocator_type, typename ... Arguments>
inline shared_ptr<T> make_shared(Arguments && ... arguments)
{
    static Allocator allocator = Allocator();
    return std::allocate_shared<T>(allocator, std::forward<Arguments>(arguments)...);
}

template<
    typename T,
    typename... Arguments,
    std::enable_if_t<!std::is_array_v<T>, Int32> = 0
>
inline unique_ptr<T> make_unique(Arguments && ... arguments)
{
    static Detail::CustomDeleter<T> deleter = Detail::CustomDeleter<T>();

    return unique_ptr<T>(new T(std::forward<Arguments>(arguments)...), deleter);
}

template<
    typename T,
    std::enable_if_t<std::is_array_v<T> && std::extent<T>::value == 0, Int32> = 0
>
inline unique_ptr<T> make_unique(SizeT size)
{
    typedef std::remove_extent_t<T> Element;

    static Detail::CustomDeleter<T> deleter = Detail::CustomDeleter<T>();

    return (unique_ptr<T>(new Element[size](), deleter));
}

template<typename T,
    class... Arguments,
    std::enable_if_t<std::extent<T>::value != 0, Int32> = 0
>
Void make_unique(Arguments&&...) = delete;


} // namespace STL
} // namespace Oak

