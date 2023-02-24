/***************************************************************************
 *            logical.decl.hpp
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

/*! \file logical.decl.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_LOGICAL_DECL_HPP
#define SYMBOLICORE_LOGICAL_DECL_HPP

#include "paradigm.hpp"

namespace SymboliCore {

typedef void Void;
typedef bool Bool;

class Indeterminate;
extern const Indeterminate indeterminate;

class Boolean;
class Sierpinskian;
class Kleenean;

template<class L> class Negate;
template<class L> class Lower;
template<class L> class Upper;
template<class L> class Validated;
template<class L> class Approximate;

class NegatedSierpinskian;
class LowerKleenean;
class UpperKleenean;
class ValidatedSierpinskian;
class ValidatedNegatedSierpinskian;
class ValidatedKleenean;
class ValidatedLowerKleenean;
class ValidatedUpperKleenean;
class ApproximateKleenean;

using Fuzzy = ApproximateKleenean;

template<class P> struct LogicalTypedef;
template<class P> using LogicalType = typename LogicalTypedef<P>::Type;
template<> struct LogicalTypedef<ExactTag> { typedef Boolean Type; };
template<> struct LogicalTypedef<EffectiveTag> { typedef Kleenean Type; };
template<> struct LogicalTypedef<ValidatedTag> { typedef ValidatedKleenean Type; };
template<> struct LogicalTypedef<ApproximateTag> { typedef ApproximateKleenean Type; };

template<class P> struct LowerLogicalTypedef;
template<class P> using LowerLogicalType = typename LowerLogicalTypedef<P>::Type;
template<> struct LowerLogicalTypedef<EffectiveTag> { typedef LowerKleenean Type; };
template<> struct LowerLogicalTypedef<ValidatedTag> { typedef ValidatedLowerKleenean Type; };

template<class P> struct UpperLogicalTypedef;
template<class P> using UpperLogicalType = typename UpperLogicalTypedef<P>::Type;
template<> struct UpperLogicalTypedef<EffectiveTag> { typedef UpperKleenean Type; };
template<> struct UpperLogicalTypedef<ValidatedTag> { typedef ValidatedUpperKleenean Type; };


using Decidable = Boolean;
using Quasidecidable = Kleenean;
using Verifyable = Sierpinskian;
using Falsifyable = NegatedSierpinskian;

Boolean operator!(Boolean const&);
NegatedSierpinskian operator!(Sierpinskian const&);
Kleenean operator!(Kleenean const&);


template<class P> struct ApartnessTraits;
template<class P> using ApartnessType = typename ApartnessTraits<P>::Type;
template<> struct ApartnessTraits<ExactTag> { typedef Boolean Type; };
template<> struct ApartnessTraits<EffectiveTag> { typedef Sierpinskian Type; };
template<> struct ApartnessTraits<ValidatedTag> { typedef ValidatedSierpinskian Type; };
template<> struct ApartnessTraits<ApproximateTag> { typedef ApproximateKleenean Type; };

template<class P> using EqualityLogicalType = decltype(not declval<ApartnessType<P>>());
template<class P> using InequalityLogicalType = ApartnessType<P>;


namespace Detail {

#if (defined __arm || defined __aarch64__)
typedef short ComparableEnumerationType;
#else
typedef char ComparableEnumerationType;
#endif    

enum class LogicalValue : ComparableEnumerationType;
}
using Detail::LogicalValue;

}

#endif
