/***************************************************************************
 *            paradigm.hpp
 *
 *  Copyright  2013-20  Pieter Collins
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

/*! \file paradigm.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_PARADIGM_HPP
#define SYMBOLICORE_PARADIGM_HPP

#include <cstdint>
#include "helper/metaprogramming.hpp"

namespace SymboliCore {

using Helper::Convertible;

class ParadigmError { };

typedef std::uint16_t  ParadigmCodeType;

enum class ParadigmCode : ParadigmCodeType {
    APPROXIMATE=1,
    VALIDATED=2,
    EFFECTIVE=3,
    EXACT=4
};

//! \ingroup ParadigmSubModule
//! \brief The <em>computational paradigm</em> supported by the object.
//! User paradigms are ExactTag, EffectiveTag, ValidatedTag,ValidatedBoundedTag, ValidatedUpperTag, ValidatedLowerTag or ApproximateTag.
//! Internal paradigms are BuiltinTag and RawTag.
template<class T> using Paradigm = typename T::Paradigm;

//! \ingroup ParadigmSubModule
//! \brief The <em>computational paradigm</em> supported by the object. Equivalent to Paradigm<T>.
template<class T> using ParadigmTag = typename T::Paradigm;

//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object is of a builtin type. Such objects should be converted to internal types before use.
struct BuiltinTag { };

//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object decribes raw data. Such objects should not be used in high-level code, as they probably do not provide safe guarantees on their values.
struct RawTag { };


//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object provides an approximation to a quantity with no guarantees on the error.
struct ApproximateTag { static constexpr ParadigmCode code() { return ParadigmCode::APPROXIMATE; } };

//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object represents an approximation to a quantity with a bound on the error in some metric, or lower and upper bounds on a quantity.
struct ValidatedTag : public ApproximateTag { static constexpr ParadigmCode code() { return ParadigmCode::VALIDATED; } };

//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object represents a quantity exactly, but equality is undecidable.
struct EffectiveTag : public ValidatedTag { static constexpr ParadigmCode code() { return ParadigmCode::EFFECTIVE; } };

//! \ingroup ParadigmSubModule
//! \brief A tag meaning that the object represents a quantity exactly, and equality is decidable.
//! Only available for discrete types i.e. elements of countable spaces, or for computational types such as floating-point numbers.
struct ExactTag : public EffectiveTag { static constexpr ParadigmCode code() { return ParadigmCode::EXACT; } };

struct MetricTag { MetricTag(){}; MetricTag(ValidatedTag){}; static constexpr ParadigmCode code() { return ParadigmCode(7); }  };
struct OrderTag { OrderTag(){}; OrderTag(ValidatedTag){}; static constexpr ParadigmCode code() { return ParadigmCode(3); }  };
struct UpperTag { UpperTag(){}; UpperTag(ValidatedTag){}; static constexpr ParadigmCode code() { return ParadigmCode(2); } };
struct LowerTag { LowerTag(){}; LowerTag(ValidatedTag){}; static constexpr ParadigmCode code() { return ParadigmCode(1); } };
struct ApproximationTag { ApproximationTag(){}; ApproximationTag(ValidatedTag){}; static constexpr ParadigmCode code() { return ParadigmCode(0); } };
using BoundedTag = OrderTag;

class NegatedSierpinskian;
class LowerKleenean;
template<class P> using EqualityInformation = P;
template<class P> using LessThanInformation = P;

template<class P1, class P2=P1> struct ParadigmTraits;


template<> struct ParadigmTraits<ExactTag,ExactTag> {
    using Weaker = ExactTag;
    using Stronger = ExactTag;
    using NextWeaker = EffectiveTag;
};

template<> struct ParadigmTraits<EffectiveTag,EffectiveTag> {
    using Weaker = EffectiveTag;
    using Stronger = EffectiveTag;
    using NextWeaker = ValidatedTag;
};

template<> struct ParadigmTraits<ValidatedTag,ValidatedTag> {
    using Weaker = ValidatedTag;
    using Stronger = ValidatedTag;
    using NextWeaker = ApproximateTag;
};

template<> struct ParadigmTraits<ApproximateTag,ApproximateTag> {
  private:
    typedef ApproximateTag P;
  public:
    using Weaker = ApproximateTag;
    using Stronger = ApproximateTag;
    using NextWeaker = void;
};

template<> struct ParadigmTraits<ExactTag,EffectiveTag> {
    using Stronger = ExactTag; using Weaker = EffectiveTag; };
template<> struct ParadigmTraits<ExactTag,ValidatedTag> {
    using Stronger = ExactTag; using Weaker = ValidatedTag; };
template<> struct ParadigmTraits<ExactTag,ApproximateTag> {
    using Stronger = ExactTag; using Weaker = ApproximateTag; };
template<> struct ParadigmTraits<EffectiveTag,ExactTag> {
    using Stronger = ExactTag; using Weaker = EffectiveTag; };
template<> struct ParadigmTraits<EffectiveTag,ValidatedTag> {
    using Stronger = EffectiveTag; using Weaker = ValidatedTag; };
template<> struct ParadigmTraits<EffectiveTag,ApproximateTag> {
    using Stronger = EffectiveTag; using Weaker = ApproximateTag; };
template<> struct ParadigmTraits<ValidatedTag,ExactTag> {
    using Stronger = ExactTag; using Weaker = ValidatedTag; };
template<> struct ParadigmTraits<ValidatedTag,EffectiveTag> {
    using Stronger = EffectiveTag; using Weaker = ValidatedTag; };
template<> struct ParadigmTraits<ValidatedTag,ApproximateTag> {
    using Stronger = ValidatedTag; using Weaker = ApproximateTag; };
template<> struct ParadigmTraits<ApproximateTag,ExactTag> {
    using Stronger = ExactTag; using Weaker = ApproximateTag; };
template<> struct ParadigmTraits<ApproximateTag,EffectiveTag> {
    using Stronger = EffectiveTag; using Weaker = ApproximateTag; };
template<> struct ParadigmTraits<ApproximateTag,ValidatedTag> {
    using Stronger = ValidatedTag; using Weaker = ApproximateTag; };

//! \brief A tag meaning that the object provides a positive upper bound for a quantity.
struct ErrorTag { };

//! \brief The weaker of \a P1 and \a P2.
template<class P1, class P2> using Weaker = typename ParadigmTraits<P1,P2>::Weaker;


//! \brief Inherits from \c TrueType if paradigm \a P1 is weaker than \a P2.
template<class P1, class P2> struct IsWeaker : std::is_convertible<P2,P1> { };

//! \brief Inherits from \c TrueType if paradigm \a P1 is stronger than \a P2.
template<class P1, class P2> struct IsStronger : IsWeaker<P2,P1> { };

template<class P1, class P2> concept WeakerThan = IsWeaker<P1,P2>::value;
template<class P1, class P2> concept StrongerThan = IsWeaker<P2,P1>::value;


template<class T> concept IsParadigm = Convertible<T,ApproximateTag>;

} // namespace SymboliCore

#endif /* SYMBOLICORE_PARADIGM_HPP */
