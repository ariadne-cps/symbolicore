/***************************************************************************
 *            typedefs.hpp
 *
 *  Copyright  2023  Luca Geretti
 *
 ****************************************************************************/

/*
 * This file is part of SymboliCore, under the MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*! \file typedefs.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_TYPEDEFS_HPP
#define SYMBOLICORE_TYPEDEFS_HPP

#include <cstdint>
#include <iosfwd>
#include <type_traits>
#include <memory>
#include <map>
#include <list>

namespace SymboliCore {

using uchar = unsigned char;
using uint = unsigned int;

//! \brief Internal name for standard output stream.
using OutputStream = std::ostream;
//! \brief Internal name for standard input stream.
using InputStream = std::istream;
//! Internal name for standard string stream.
using StringStream = std::stringstream;

//! Internal name for void type.
using Void = void;
//! Internal name for builtin boolean type.
using Bool = bool;
//! Internal name for builtin char type.
using Char = char;
//! Internal name for builtin byte type (8 bits).
using Byte = std::int8_t;
//! Internal name for builtin unsigned integers.
using Nat = uint;
//! Internal name for builtin integers.
using Int = int;
//! Internal name for builtin double-precision floats.
using Dbl = double;
//! Internal name for builtin double-precision floats.
using Double = double;

//! Internal name for standard size type, used for sizes of containers.
using SizeType = std::size_t;
//! Internal name for standard difference type of container indices and pointers.
using PointerDifferenceType = std::ptrdiff_t;

//! Internal name for unsigned integer used as a step counter
using CounterType = std::uint32_t;

using std::declval;

//! Internal alias for standard shared pointer.
template<class T> using SharedPointer = std::shared_ptr<T>;
//! Internal alias for standard initializer list.
template<class T> using InitializerList = std::initializer_list<T>;
//! Internal alias for standard pair.
template<class T1, class T2> using Pair = std::pair<T1,T2>;
//! Internal alias for standard tuple.
template<class... TS> using Tuple = std::tuple<TS...>;

//! A class wrapper for C-style arrays.
template<class T> class Array;
//! A thin wrapper around a std::vector.
template<class T> class List;
//! A thin wrapper around a std::set.
template<class T> class Set;
//! A thin wrapper around a std::map.
template<class K, class T> class Map;

//! A tag for the size of a scalar object.
struct SizeOne { operator SizeType() const { return 1u; } };
//! A tag for an index into a scalar object.
struct IndexZero { operator SizeType() const { return 0u; } };

//! The type used for the degree of an index.
typedef std::uint16_t DegreeType;
//! The type used for the dimension of a geometric object.
typedef SizeType DimensionType;

#if (defined __arm || defined __aarch64__)
typedef short ComparableEnumerationType;
#else
typedef char ComparableEnumerationType;
#endif


} // namespace SymboliCore

#endif
