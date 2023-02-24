/***************************************************************************
 *            range.hpp
 *
 *  Copyright  2023  Pieter Collins
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

/*! \file range.hpp
 *  \brief Index ranges.
 */

#ifndef SYMBOLICORE_RANGE_HPP
#define SYMBOLICORE_RANGE_HPP

#include "typedefs.hpp"

namespace SymboliCore {

//! \ingroup LinearAlgebraModule
//! \brief A range of integer values from a \em start value up to, but not including, a \em stop value.
class Range {
    SizeType _start; SizeType _stop;
  public:
    Range(SizeType start, SizeType stop) : _start(start), _stop(stop) { } //!< .
    SizeType operator[](SizeType i) const { return _start+i; } //!< .
    SizeType size() const { return this->_stop-this->_start; } //!< .
    SizeType start() const { return this->_start; } //!< .
    SizeType stop() const { return this->_stop; } //!< .
    SizeType stride() const { return 1u; } //!< Always equal to \a 1.
};
inline Range range(SizeType stop) { return Range(0u,stop); } //!< \relates Range
inline Range range(SizeType start, SizeType stop) { return Range(start,stop); } //!< \relates Range

struct RangeIterator {
    explicit inline RangeIterator(SizeType i) : _i(i) { }
    inline RangeIterator& operator++() { ++this->_i; return *this; }
    inline SizeType operator*() const { return this->_i; }
    friend inline bool operator!=(RangeIterator iter1, RangeIterator iter2) { return iter1._i != iter2._i; }
  private:
    SizeType _i;
};
inline RangeIterator begin(Range rng) { return RangeIterator(rng.start()); }
inline RangeIterator end(Range rng) { return RangeIterator(rng.stop()); }

} // namespace SymboliCore

#endif // SYMBOLICORE_RANGE_HPP
