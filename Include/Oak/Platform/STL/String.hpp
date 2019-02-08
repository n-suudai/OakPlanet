
#pragma once

#include "Oak/Platform/STL/STLAllocator.hpp"

#include <string>
#include <sstream>


namespace Oak {
namespace STL {


// basic_string
template<
    typename Element,
    typename Traits = std::char_traits<Element>,
    typename Allocator = StandardAllocator
>
using  basic_string = std::basic_string<Element, Traits, StdAllocator<Element, Allocator>>;

// string
typedef basic_string<Char>  string;
typedef basic_string<WChar> wstring;
typedef basic_string<TChar> tstring;


// basic_stringstream
template<
    typename Element,
    typename Traits = std::char_traits<Element>,
    typename Allocator = StandardAllocator
>
using basic_stringstream = std::basic_stringstream<Element, Traits, StdAllocator<Element, Allocator>>;

// stringstream
typedef basic_stringstream<Char>  stringstream;
typedef basic_stringstream<WChar> wstringstream;
typedef basic_stringstream<TChar> tstringstream;


} // namespace STL
} // namespace Oak

