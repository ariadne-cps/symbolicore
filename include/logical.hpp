/***************************************************************************
 *            logical.hpp
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

//! \file logical.hpp
//! \brief Logical classes

#ifndef SYMBOLICORE_LOGICAL_HPP
#define SYMBOLICORE_LOGICAL_HPP

#include "helper/stdlib.hpp"
#include "helper/handle.hpp"
#include "helper/string.hpp"
#include "helper/array.hpp"

#include "sign.hpp"
#include "paradigm.hpp"
#include "logical.decl.hpp"

namespace SymboliCore {

using Helper::String;
using Helper::Array;
using Helper::Handle;
using Helper::ConstructibleFrom;
using std::ostream;
using std::shared_ptr;

template<class X> class Sequence;

template<class T> String class_name();

//! \ingroup LogicalModule
//! \brief The amount of work used in performing a calculation or checking a quasidecidable predicate.
//! \details The Effort should roughly reflect the time needed to perform a computation.
//! \sa Accuracy.
class Effort {
    static unsigned int _default;
    unsigned int _m;
  public:
    //! \brief Get the default effort. Initially set to the minimum possible effort 0.
    static Effort get_default() { return Effort(_default); }
    //! \brief Set the default effort.
    static void set_default(unsigned int m) { _default=m; }
  public:
    //! \brief Construct from a raw positive integer.
    explicit Effort(unsigned int m) : _m(m) { }
    //! \brief Convert to a raw positive integer.
    operator unsigned int() const { return _m; }
    unsigned int work() const { return _m; }
    Effort& operator++() { ++_m; return *this; }
    Effort& operator+=(unsigned int m) { _m+=m; return *this; }
    Effort& operator*=(unsigned int m) { _m*=m; return *this; }
    friend ostream& operator<<(ostream& os, Effort eff) { return os << "Effort(" << eff._m << ")"; }
};
inline Effort operator""_eff(unsigned long long int e) { unsigned int m=static_cast<unsigned int>(e); assert(m==e); return Effort(m); }

namespace Detail {

    //! \ingroup LogicalTypes
    //! \brief An enumeration containing the possible values of a logical variable.
    enum class LogicalValue : ComparableEnumerationType {
        FALSE=-2, //!< Definitely not true.
        UNLIKELY=-1, //!< Considered unlikely to be true.
        INDETERMINATE= 0, //!< Truth is unknown, possibly undecidable.
        LIKELY=+1, //!< Considered likely to be true.
        TRUE=+2 //!< Definitely true.
    };
    inline LogicalValue make_logical_value(bool b) { return b ? LogicalValue::TRUE : LogicalValue::FALSE; }
    inline bool definitely(LogicalValue lv) { return lv==LogicalValue::TRUE; }
    inline bool probably(const LogicalValue& lv) { return lv>=LogicalValue::LIKELY; }
    inline bool decide(LogicalValue lv) { return lv>=LogicalValue::LIKELY; }
    inline bool possibly(LogicalValue lv) { return lv!=LogicalValue::FALSE; }
    inline bool is_determinate(LogicalValue lv) { return lv==LogicalValue::TRUE or lv==LogicalValue::FALSE; }
    inline bool is_indeterminate(LogicalValue lv) { return lv==LogicalValue::INDETERMINATE or lv==LogicalValue::UNLIKELY or lv==LogicalValue::LIKELY; }
    LogicalValue operator==(LogicalValue lv1, LogicalValue lv2);
    inline LogicalValue operator!(LogicalValue lv) { return static_cast<LogicalValue>(-static_cast<ComparableEnumerationType>(lv)); }
    inline LogicalValue operator&&(LogicalValue lv1, LogicalValue lv2) { return std::min(lv1,lv2); }
    inline LogicalValue operator||(LogicalValue lv1, LogicalValue lv2) { return std::max(lv1,lv2); }
    inline LogicalValue operator^(LogicalValue lv1, LogicalValue lv2) { return not (lv1==lv2); }
    ostream& operator<<(ostream& os, LogicalValue l);

    class LogicalInterface {
      public:
        virtual ~LogicalInterface() = default;
        virtual LogicalInterface* clone() const = 0;
        virtual LogicalValue _check(Effort) const = 0;
        virtual ostream& _write(ostream&) const = 0;
    };

    class LogicalHandle : public Handle<LogicalInterface> {
       public:
        using Handle<LogicalInterface>::Handle;
        static LogicalHandle constant(LogicalValue v);
        LogicalValue check(Effort eff) const { return this->pointer()->_check(eff); }
        friend ostream& operator<<(ostream& os, LogicalHandle l) {
            return l.pointer()->_write(os); }
    };

    inline LogicalValue check(LogicalHandle l, Effort e) { return l.check(e); }
    inline bool definitely(LogicalHandle l, Effort e) { return definitely(check(l,e)); }
    inline bool probably(LogicalHandle l, Effort e) { return probably(check(l,e)); }
    inline bool decide(LogicalHandle l, Effort e) { return decide(check(l,e)); }
    inline bool possibly(LogicalHandle l, Effort e) { return possibly(check(l,e)); }
    LogicalHandle operator&&(LogicalHandle v1, LogicalHandle v2);
    LogicalHandle operator||(LogicalHandle v1, LogicalHandle v2);
    LogicalHandle operator==(LogicalHandle v1, LogicalHandle v2);
    LogicalHandle operator^(LogicalHandle v1, LogicalHandle v2);
    LogicalHandle operator!(LogicalHandle v);

    LogicalHandle conjunction(LogicalHandle l1, LogicalHandle l2);
    LogicalHandle disjunction(LogicalHandle l1, LogicalHandle l2);
    LogicalHandle negation(LogicalHandle l);
    LogicalHandle equality(LogicalHandle l1, LogicalHandle l2);
    LogicalHandle exclusive(LogicalHandle l1, LogicalHandle l2);

    LogicalValue logical_value_from_pointer(LogicalInterface* ptr);
    LogicalInterface* new_logical_pointer_from_value(LogicalValue);

    template<class P> LogicalType<P> logical_type_from_pointer(LogicalInterface* ptr) {
        if constexpr (ConstructibleFrom<LogicalType<P>,LogicalHandle>) {
            auto elptr=dynamic_cast<LogicalInterface*>(ptr);
            if (elptr) {
                return LogicalType<P>(LogicalHandle(shared_ptr<LogicalInterface>(elptr)));
            }
        }
        static_assert (ConstructibleFrom<LogicalType<P>,LogicalValue>);
        return LogicalType<P>(logical_value_from_pointer(ptr));
    }


}

using Detail::LogicalValue;
using Detail::LogicalHandle;
using Detail::LogicalInterface;



template<class L, class R> class LogicalBase;

template<class L> class LogicalBase<L,LogicalValue> {
    typedef LogicalValue R;
  protected:
    R _v;
    explicit LogicalBase(R v) : _v(v) { }
    explicit LogicalBase(bool b) = delete;
  public:
    friend bool definitely(L const& l) { return Detail::definitely(l._v); }
    friend bool probably(L const& l) { return Detail::probably(l._v); }
    friend bool decide(L const& l) { return Detail::decide(l._v); }
    friend bool possibly(L const& l) { return Detail::possibly(l._v); }
    friend bool is_determinate(L const& l) { return Detail::is_determinate(l._v); }
    friend bool is_indeterminate(L const& l) { return Detail::is_indeterminate(l._v); }
};

template<class L> class LogicalBase<L,LogicalHandle> {
    typedef LogicalHandle R;
  protected:
    R _v;
    explicit LogicalBase(R v) : _v(v) { }
    explicit LogicalBase(bool b) = delete;
  public:
    friend bool definitely(L const& l, Effort e) { return definitely(l.check(e)); }
    friend bool probably(L const& l, Effort e) { return probably(l.check(e)); }
    friend bool decide(L const& l, Effort e) { return decide(l.check(e)); }
    friend bool possibly(L const& l, Effort e) { return possibly(l.check(e)); }
    friend bool definitely(L const& l) { return definitely(l,Effort::get_default()); }
    friend bool probably(L const& l) { return probably(l,Effort::get_default()); }
    friend bool decide(L const& l) { return decide(l,Effort::get_default()); }
    friend bool possibly(L const& l) { return possibly(l,Effort::get_default()); }
};


template<class L, class R, class NL=L, class XL=L> class Logical : public LogicalBase<L,R> { // FIXME: Use protected inheritence
    template<class LL, class RR, class NNLL, class XXLL> friend class Logical;
  protected:
    explicit Logical(R v) : LogicalBase<L,R>(v) { }
    explicit Logical(bool b) = delete;
  public:
    explicit operator R const&() const { return this->_v; }
    R const& repr() const { return this->_v; }
  public:
    friend NL operator!(L const& l) { return NL(! l._v); }
    friend L operator&&(L const& l1, L const& l2) { return L(l1._v && l2._v); }
    friend L operator||(L const& l1, L const& l2) { return L(l1._v || l2._v); }

    friend XL operator^(L const& l1, L const& l2) { return XL(l1._v ^ l2._v); }
    friend XL operator==(L const& l1, L const& l2) { return XL(l1._v == l2._v); }
    friend XL operator!=(L const& l1, L const& l2) { return XL(l1._v ^ l2._v); }

    friend ostream& operator<<(ostream& os, L const& l) { return os << l._v; }
};

#ifdef DOXYGEN
//! \relatesalso ValidatedKleenean \ingroup LogicalTypes
//! \brief Returns \c true if a finite (non-effective) logical type is definitely true, otherwise \c false.
//! In particular, always returns \c false for a ValidatedUpperKleenean or ApproximateKleenean.
template<class L> bool definitely(L const&);
//! \relatesalso ValidatedKleenean \ingroup LogicalTypes
//! \brief Returns \c false if a finite (non-effective) logical type is definitely false, otherwise \c true.
//! In particular, always returns \c false for a ValidatedLowerKleenean or ApproximateKleenean.
template<class L> bool possibly(L const&);
//! \relatesalso ApproximateKleenean \ingroup LogicalTypes
//! \brief Returns an 'educated guess' to the value of a finite (non-effective) logical type.
//! Returns \c true for logical values of \c TRUE and \c LIKELY, and \c false for \c INDETERMINATE, \c UNLIKELY and \c FALSE.
template<class L> bool probably(L const& l);
//! \relatesalso Kleenean \ingroup LogicalTypes
//! \brief Force a decision of a logical type.
//! Returns \c true for logical values of \c TRUE, and \c LIKELY for a non-effective type, and \c decide(check(l,e)) for the default effort \c e.
template<class L> bool decide(L const&);
#endif

inline bool definitely(bool b) { return b; }
inline bool probably(bool b) { return b; }
inline bool decide(bool b) { return b; }
inline bool possibly(bool b) { return b; }

class Boolean; class Sierpinskian; class Kleenean;
class LowerKleenean; class UpperKleenean;
class ValidatedSierpinskian; class ValidatedNegatedSierpinskian;
class ValidatedKleenean; class ValidatedLowerKleenean; class ValidatedUpperKleenean;
class ApproximateKleenean;

class Indeterminate {
  public:
    operator ValidatedSierpinskian() const;
    operator ValidatedKleenean() const;
    friend bool decide(Indeterminate const& l, Effort e);
    friend bool decide(Indeterminate const& l);
    friend ostream& operator<<(ostream& os, ValidatedKleenean const&);
};

//! \ingroup LogicalModule
//! \brief A logical class representing the result of a decidable predicate.
//! \sa LogicalModule
class Boolean : public Logical<Boolean,LogicalValue> {
    typedef Logical<Boolean,LogicalValue> Base;
  public:
    explicit Boolean(LogicalValue l) : Base(l) { }
  public:
    //! \brief Convert from a built-in boolean value, defaulting to \c true.
    Boolean(bool b=true) : Boolean(Detail::make_logical_value(b)) { }
    //! \brief Convert to a built-in boolean value.
    operator bool() const { return definitely(*this); }

    //! \brief Logical negation.
    friend Boolean operator!(Boolean const& b);
    //! \brief Logical conjunction.
    friend Boolean operator&&(Boolean const& b1, Boolean const& b2);
    //! \brief Logical disjunction. <code>b1 or b2</code> is equivalent to <code>not ( (not b1) and (not b2) )</code>
    friend Boolean operator||(Boolean const& b1, Boolean const& b2);
    //! \brief Logical exclusive or.
    //! <code>b1 xor b2</code> is equivalent to <code>(b1 or b2) and ((not b1) or (not b2))</code> and to <code>(b1 or b2) and not (b1 and b2)</code>
    friend Boolean operator^(Boolean const& b1, Boolean const& b2);
    //! \brief Logical equality.
    friend Boolean operator==(Boolean const& b1, Boolean const& b2);

    friend Boolean operator&&(Boolean const& l1, bool const& l2) { return l1 && Boolean(l2); }
    friend Boolean operator&&(bool const& l1, Boolean const& l2) { return Boolean(l1) && l2; }
    friend Boolean operator||(Boolean const& l1, bool const& l2) { return l1 || Boolean(l2); }
    friend Boolean operator||(bool const& l1, Boolean const& l2) { return Boolean(l1) || l2; }
};


//! \ingroup LogicalModule
//! \brief A logical class representing the result of a verifyable predicate
//! over a continuous (uncountable) data type which has no robustly \c false instances,
//! such as inequality on a Hausdoff space.
class Sierpinskian : public Logical<Sierpinskian,LogicalHandle,NegatedSierpinskian,Kleenean> {
    typedef Logical<Sierpinskian,LogicalHandle,NegatedSierpinskian,Kleenean> Base;
  public:
    explicit Sierpinskian(LogicalValue l) : Base(LogicalHandle::constant(l)) { }
    explicit Sierpinskian(LogicalHandle l) : Base(l) { }
    explicit Sierpinskian(bool b=true) : Sierpinskian(Detail::make_logical_value(b)) { }
    Sierpinskian(Indeterminate) : Sierpinskian(LogicalValue::INDETERMINATE) { }
    //! \brief Check the value using effort \a e.
    ValidatedSierpinskian check(Effort e) const;
    //! \brief Check the value of \a s using effort \a e.
    friend ValidatedSierpinskian check(Sierpinskian const& s, Effort e);

    //! \brief Logical negation.
    friend NegatedSierpinskian operator!(Sierpinskian const& l);
    //! \brief Logical conjunction.
    friend Sierpinskian operator&&(Sierpinskian const& l1, Sierpinskian const& l2);
    //! \brief Logical disjunction.
    friend Sierpinskian operator||(Sierpinskian const& l1, Sierpinskian const& l2);

    friend Sierpinskian const& operator||(Sierpinskian const& l1, Boolean const&) { return l1; }
    friend Sierpinskian const& operator||(Boolean const&, Sierpinskian const& l2) { return l2; }

    friend Kleenean operator&&(Kleenean const&, Kleenean const&);
    friend Kleenean operator||(Kleenean const&, Kleenean const&);
};

//! \brief A logical class representing the result of a falsifyable predicate
//! over a continuous (uncountable) data type which has no robustly \c true instances,
//! such as equality on a Hausdoff space.
class NegatedSierpinskian : public Logical<NegatedSierpinskian,LogicalHandle,Sierpinskian,Kleenean> {
    typedef Logical<NegatedSierpinskian,LogicalHandle,Sierpinskian,Kleenean> Base;
  public:
    explicit NegatedSierpinskian(LogicalValue l) : Base(LogicalHandle::constant(l)) { }
    explicit NegatedSierpinskian(LogicalHandle l) : Base(l) { }
    explicit NegatedSierpinskian(bool b) : NegatedSierpinskian(Detail::make_logical_value(b)) { }
    NegatedSierpinskian(Indeterminate) : NegatedSierpinskian(LogicalValue::INDETERMINATE) { }
    //! \brief Check the value using effort \a e.
    ValidatedNegatedSierpinskian check(Effort e) const;
    //! \brief Check the value of \a s using effort \a e.
    friend ValidatedNegatedSierpinskian check(NegatedSierpinskian const& s, Effort e);

    //! \brief Logical negation.
    friend Sierpinskian operator!(NegatedSierpinskian const& l);
    //! \brief Logical conjunction.
    friend NegatedSierpinskian operator&&(NegatedSierpinskian const& l1, NegatedSierpinskian const& l2);
    //! \brief Logical disjunction.
    friend NegatedSierpinskian operator||(NegatedSierpinskian const& l1, NegatedSierpinskian const& l2);

    friend NegatedSierpinskian const& operator&&(NegatedSierpinskian const& l1, Boolean const&) { return l1; }
    friend NegatedSierpinskian const& operator&&(Boolean const&, NegatedSierpinskian const& l2) { return l2; }
    friend Kleenean operator&&(Kleenean const&, Kleenean const&);
    friend Kleenean operator||(Kleenean const&, Kleenean const&);
};

//! \brief A logical class representing the result of a quasidecidable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is (strict) positivity of a real number,
//! which is undecidable for zero, since positivity cannot be determined from arbitrarily accurate approximations.
class Kleenean : public Logical<Kleenean,LogicalHandle> {
    typedef Logical<Kleenean,LogicalHandle> Base;
  public:
    explicit Kleenean(LogicalHandle const& l) : Base(l) { }
    explicit Kleenean(LogicalValue l) : Base(LogicalHandle::constant(l)) { }  // FIXME: Remove
  public:
    //! \brief Convert from a built-in boolean value, defaulting to \c true.
    Kleenean(bool b=true) : Kleenean(Boolean(b)) { }
    //! \brief Convert from the %Indeterminate constant value.
    Kleenean(Indeterminate) : Kleenean(LogicalValue::INDETERMINATE) { }
    //! \brief Convert from a %Boolean constant value. May be checked with effort 0.
    Kleenean(Boolean b) : Kleenean(b.repr()) { }
    //! \brief Convert from a %Sierpinskian predicate.
    Kleenean(Sierpinskian s) : Kleenean(s.repr()) { }
    //! \brief Convert from a negated %Sierpinskian predicate.
    Kleenean(NegatedSierpinskian ns) : Kleenean(ns.repr()) { }
    //! \brief Convert from a sign \a s.
    Kleenean(Sign s) : Kleenean(s == Sign::NEGATIVE ? LogicalValue::FALSE : (s == Sign::POSITIVE ? LogicalValue::TRUE : LogicalValue::INDETERMINATE)) { }
    //! \brief Check the value using effort \a e.
    ValidatedKleenean check(Effort e) const;
    //! \brief Check the value of \a k using effort \a e.
    friend ValidatedKleenean check(Kleenean const& k, Effort e);

    //! \brief Logical negation.
    friend Kleenean operator!(Kleenean const& k);
    //! \brief Logical conjunction.
    friend Kleenean operator&&(Kleenean const& k1, Kleenean const& k2);
    //! \brief Logical disjunction.
    friend Kleenean operator||(Kleenean const& k1, Kleenean const& k2);
    //! \brief Logical exclusive or.
    friend Kleenean operator^(Kleenean const& k1, Kleenean const& k2);
};

//! \brief A logical class representing the result of a verifyable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is strict positivity of a real number given only lower bounds to its value,
//! since sufficient accurate lower bounds for a strictly positive number are themselves strictly positive,
//! but negativity cannot be deduced given only lower bounds to a negative number.
class LowerKleenean : public Logical<LowerKleenean,LogicalHandle,UpperKleenean> {
    typedef Logical<LowerKleenean,LogicalHandle,UpperKleenean> Base;
  public:
    explicit LowerKleenean(LogicalHandle const& l) : Base(l) { }
    LowerKleenean(bool b) : LowerKleenean(Boolean(b)) { }
    LowerKleenean(Indeterminate) : LowerKleenean(LogicalHandle::constant(LogicalValue::INDETERMINATE)) { }
    LowerKleenean(Boolean b) : LowerKleenean(Kleenean(b)) { }
    LowerKleenean(Sierpinskian s) : LowerKleenean(Kleenean(s)) { }
    //! \brief Convert from a %Kleenean predicate.
    LowerKleenean(Kleenean k) : LowerKleenean(k.repr()) { }
    //! \brief Check the value using effort \a e.
    ValidatedLowerKleenean check(Effort e) const;
    //! \brief Check the value of \a lk using effort \a e.
    friend ValidatedLowerKleenean check(LowerKleenean const& lk, Effort e);

    //! \brief Logical negation.
    friend UpperKleenean operator!(LowerKleenean const& l);
    //! \brief Logical conjunction.
    friend LowerKleenean operator&&(LowerKleenean const& l1, LowerKleenean const& l2);
    //! \brief Logical disjunction.
    friend LowerKleenean operator||(LowerKleenean const& l1, LowerKleenean const& l2);

    //! \brief Disjunction of an infinte sequence of logical values.
    friend LowerKleenean disjunction(Sequence<LowerKleenean> const& k);
};

//! \ingroup LogicalModule
//! \brief A logical class representing the result of a falsifyable predicate
//! over a continuous (uncountable) data type.
class UpperKleenean : public Logical<UpperKleenean,LogicalHandle,LowerKleenean> {
    typedef Logical<UpperKleenean,LogicalHandle,LowerKleenean> Base;
  public:
    explicit UpperKleenean(LogicalHandle const& l) : Base(l) { }
    UpperKleenean(bool b) : UpperKleenean(Kleenean(b)) { }
    UpperKleenean(Indeterminate) : UpperKleenean(LogicalHandle::constant(LogicalValue::INDETERMINATE)) { }
    UpperKleenean(Boolean b) : UpperKleenean(Kleenean(b)) { }
    UpperKleenean(NegatedSierpinskian ns) : UpperKleenean(Kleenean(ns)) { }
    //! \brief Convert from a %Kleenean predicate.
    UpperKleenean(Kleenean k) : UpperKleenean(k.repr()) { }
    //! \brief Check the value using effort \a e.
    ValidatedUpperKleenean check(Effort e) const;
    //! \brief Check the value of \a uk using effort \a e.
    friend ValidatedUpperKleenean check(UpperKleenean const& uk, Effort e);

    //! \brief Logical negation.
    friend LowerKleenean operator!(UpperKleenean const& l);
    //! \brief Logical conjunction.
    friend UpperKleenean operator&&(UpperKleenean const& l1, UpperKleenean const& l2);
    //! \brief Logical disjunction.
    friend UpperKleenean operator||(UpperKleenean const& l1, UpperKleenean const& l2);

    //! \brief Conjunction of an infinte sequence of logical values.
    friend UpperKleenean conjunction(Sequence<UpperKleenean> const& k);
};

LowerKleenean disjunction(Sequence<LowerKleenean> const& lk);
UpperKleenean conjunction(Sequence<UpperKleenean> const& uk);

//! \ingroup LogicalModule
//! \brief A logical class representing the result of a undecidable predicate
//! over a continuous (uncountable) data type.
//! \details A canonical example is (strict) positivity of a real number given only a convergent sequence,
//! since without error bounds we can never know whether a given approximation is sufficiently close to
//! deduce any properties of the number.
class NaiveKleenean : public Logical<NaiveKleenean,LogicalHandle> {
    typedef Logical<NaiveKleenean,LogicalHandle> Base;
  public:
    explicit NaiveKleenean(LogicalHandle const& l) : Base(l) { }
    NaiveKleenean(bool b) : NaiveKleenean(Kleenean(b)) { }
    NaiveKleenean(Indeterminate i) : NaiveKleenean(Kleenean(i)) { }
    NaiveKleenean(Boolean b) : NaiveKleenean(Kleenean(b)) { }
    NaiveKleenean(Sierpinskian s) : NaiveKleenean(Kleenean(s)) { }
    NaiveKleenean(NegatedSierpinskian ns) : NaiveKleenean(Kleenean(ns)) { }
    //! \brief Convert from a %Kleenean predicate.
    NaiveKleenean(Kleenean k) : NaiveKleenean(k.repr()) { }
    //! \brief Convert from a %LowerKleenean predicate.
    NaiveKleenean(LowerKleenean lk) : NaiveKleenean(lk.repr()) { }
    //! \brief Convert from an %UpperKleenean predicate.
    NaiveKleenean(UpperKleenean uk) : NaiveKleenean(uk.repr()) { }
    //! \brief Check the value using effort \a e.
    ApproximateKleenean check(Effort e) const;
    //! \brief Check the value of \a ak using effort \a e.
    friend ApproximateKleenean check(NaiveKleenean const& ak, Effort e);

    //! \brief Logical negation.
    friend NaiveKleenean operator!(NaiveKleenean const& l);
    //! \brief Logical conjunction.
    friend NaiveKleenean operator&&(NaiveKleenean const& l1, NaiveKleenean const& l2);
    //! \brief Logical disjunction.
    friend NaiveKleenean operator||(NaiveKleenean const& l1, NaiveKleenean const& l2);
};



//! \sa LogicalModule
//! \brief A logical class representing the result of a verifyable predicate, checked to some given effort.
//! \sa Sierpinskian
class ValidatedSierpinskian : public Logical<ValidatedSierpinskian,LogicalValue,ValidatedNegatedSierpinskian> {
    typedef Logical<ValidatedSierpinskian,LogicalValue,ValidatedNegatedSierpinskian> Base;
  public:
    explicit ValidatedSierpinskian(LogicalValue lv) : Base(lv || LogicalValue::UNLIKELY) { }
    ValidatedSierpinskian(bool b) : ValidatedSierpinskian(b?LogicalValue::TRUE:LogicalValue::UNLIKELY) { }
    ValidatedSierpinskian(Sierpinskian s, Effort e) : ValidatedSierpinskian(s.check(e)) { }
    friend ValidatedSierpinskian check(Sierpinskian const& s, Effort e) { return s.check(e); }
};

//! \ingroup LogicalModule
//! \brief A logical class representing the result of a falsifyable predicate, checked using some given effort.
//! \sa Sierpinskian, NegatedSierpinskian
class ValidatedNegatedSierpinskian : public Logical<ValidatedNegatedSierpinskian,LogicalValue,ValidatedSierpinskian> {
    typedef Logical<ValidatedNegatedSierpinskian,LogicalValue,ValidatedSierpinskian> Base;
  public:
    explicit ValidatedNegatedSierpinskian(LogicalValue lv) : Base(lv && LogicalValue::LIKELY) { }
    ValidatedNegatedSierpinskian(bool b) : ValidatedNegatedSierpinskian(b?LogicalValue::LIKELY:LogicalValue::FALSE) { }
    ValidatedNegatedSierpinskian(NegatedSierpinskian ns, Effort e) : ValidatedNegatedSierpinskian(ns.check(e)) { }
    friend ValidatedNegatedSierpinskian check(NegatedSierpinskian const& ns, Effort e) { return ns.check(e); }
};

//! \ingroup LogicalModule
//! \brief A logical class representing a value of a quasidecidable predicate,
//! either a Kleenean value \ref Kleenean::check "check"ed using some \ref Effort, or applied directly to a Validated object.
//! \sa Kleenean
class ValidatedKleenean : public Logical<ValidatedKleenean,LogicalValue> {
    typedef Logical<ValidatedKleenean,LogicalValue> Base;
  public:
    explicit ValidatedKleenean() : ValidatedKleenean(true) { } //FIXME: Remove
    explicit ValidatedKleenean(LogicalValue lv) : Base(lv) { }
    ValidatedKleenean(bool b) : ValidatedKleenean(Boolean(b)) { }
    ValidatedKleenean(Boolean b) : ValidatedKleenean(static_cast<LogicalValue>(b)) { }
    explicit ValidatedKleenean(ValidatedSierpinskian s) : ValidatedKleenean(static_cast<LogicalValue>(s)) { }
    explicit ValidatedKleenean(ValidatedNegatedSierpinskian ns) : ValidatedKleenean(static_cast<LogicalValue>(ns)) { }
    ValidatedKleenean(Kleenean k, Effort e) : ValidatedKleenean(k.check(e)) { }
    friend ValidatedKleenean check(Kleenean const& k, Effort e) { return k.check(e); }
};

//! \ingroup LogicalModule
//! \brief A logical class representing the result of checking a verifyable predicate.
//! \sa LowerKleenean
class ValidatedLowerKleenean : public Logical<ValidatedLowerKleenean,LogicalValue,ValidatedUpperKleenean> {
    typedef Logical<ValidatedLowerKleenean,LogicalValue,ValidatedUpperKleenean> Base;
  public:
    explicit ValidatedLowerKleenean(LogicalValue lv) : Base(lv || LogicalValue::UNLIKELY) { }
    ValidatedLowerKleenean(bool b) : ValidatedLowerKleenean(Boolean(b)) { }
    ValidatedLowerKleenean(Boolean b) : ValidatedLowerKleenean(static_cast<LogicalValue>(b)) { }
    ValidatedLowerKleenean(ValidatedSierpinskian s) : ValidatedLowerKleenean(static_cast<LogicalValue>(s)) { }
    ValidatedLowerKleenean(ValidatedKleenean k) : ValidatedLowerKleenean(static_cast<LogicalValue>(k)) { }
    ValidatedLowerKleenean(LowerKleenean lk, Effort e) : ValidatedLowerKleenean(lk.check(e)) { }
    friend ValidatedLowerKleenean check(LowerKleenean const& lk, Effort e) { return lk.check(e); }
};

//! \ingroup LogicalModule
//! \brief A logical class representing the result of checking a falsifyable predicate.
//! \sa UpperKleenean
class ValidatedUpperKleenean : public Logical<ValidatedUpperKleenean,LogicalValue,ValidatedLowerKleenean> {
    typedef Logical<ValidatedUpperKleenean,LogicalValue,ValidatedLowerKleenean> Base;
  public:
    explicit ValidatedUpperKleenean(LogicalValue lv) : Base(lv && LogicalValue::LIKELY) { }
    ValidatedUpperKleenean(bool b) : ValidatedUpperKleenean(Boolean(b)) { }
    ValidatedUpperKleenean(Boolean b) : ValidatedUpperKleenean(static_cast<LogicalValue>(b)) { }
    ValidatedUpperKleenean(ValidatedKleenean k) : ValidatedUpperKleenean(static_cast<LogicalValue>(k)) { }
    ValidatedUpperKleenean(ValidatedNegatedSierpinskian ns) : ValidatedUpperKleenean(static_cast<LogicalValue>(ns)) { }
    ValidatedUpperKleenean(UpperKleenean uk, Effort e) : ValidatedUpperKleenean(uk.check(e)) { }
    friend ValidatedUpperKleenean check(UpperKleenean const& uk, Effort e) { return uk.check(e); }
};

//! \ingroup LogicalModule
//! \brief A logical class representing a value of an undecidable predicate,
//! either a NaiveKleenean value \ref NaiveKleenean::check "check"ed using some Effort, or applied directly to an Approximate object.
//! \details Note that definitely() and possibly() always return \c false and \c true, respectively.
//! \sa NaiveKleenean
class ApproximateKleenean : public Logical<ApproximateKleenean,LogicalValue> {
    typedef Logical<ApproximateKleenean,LogicalValue> Base;
  public:
    explicit ApproximateKleenean(LogicalValue lv) : Base((lv && LogicalValue::LIKELY) || LogicalValue::UNLIKELY) { }
    ApproximateKleenean(bool b) : ApproximateKleenean(Boolean(b)) { }
    ApproximateKleenean(Boolean b) : ApproximateKleenean(static_cast<LogicalValue>(b)) { }
    ApproximateKleenean(ValidatedKleenean k) : ApproximateKleenean(static_cast<LogicalValue>(k)) { }
    ApproximateKleenean(ValidatedLowerKleenean lk) : ApproximateKleenean(static_cast<LogicalValue>(lk)) { }
    ApproximateKleenean(ValidatedUpperKleenean uk) : ApproximateKleenean(static_cast<LogicalValue>(uk)) { }
    ApproximateKleenean(Kleenean k, Effort e) : ApproximateKleenean(k.check(e)) { }
};

inline Indeterminate::operator ValidatedSierpinskian() const {
    return ValidatedSierpinskian(LogicalValue::INDETERMINATE); }
inline Indeterminate::operator ValidatedKleenean() const {
    return ValidatedKleenean(LogicalValue::INDETERMINATE); }
inline bool decide(Indeterminate const& l, Effort e) {
    return decide(ValidatedKleenean(LogicalValue::INDETERMINATE)); }
inline bool decide(Indeterminate const& l) {
    return decide(ValidatedKleenean(LogicalValue::INDETERMINATE)); }

inline ValidatedSierpinskian Sierpinskian::check(Effort eff) const {
    return ValidatedSierpinskian(this->repr().check(eff)); }
inline ValidatedNegatedSierpinskian NegatedSierpinskian::check(Effort eff) const {
    return ValidatedNegatedSierpinskian(this->repr().check(eff)); }
inline ValidatedKleenean Kleenean::check(Effort e) const {
    return ValidatedKleenean(this->repr().check(e)); }
inline ValidatedLowerKleenean LowerKleenean::check(Effort e) const {
    return ValidatedLowerKleenean(this->repr().check(e)); }
inline ValidatedUpperKleenean UpperKleenean::check(Effort e) const {
    return ValidatedUpperKleenean(this->repr().check(e)); }
inline ApproximateKleenean NaiveKleenean::check(Effort e) const {
    return ApproximateKleenean(this->repr().check(e)); }


class NondeterministicBoolean {
    LowerKleenean _pt; LowerKleenean _pf; bool _r;
  public:
    NondeterministicBoolean(LowerKleenean pt, LowerKleenean pf) : _pt(pt), _pf(pf), _r(_choose(pt,pf)) { }
    operator bool() const { return _r; }
  private:
    static bool _choose(LowerKleenean pt, LowerKleenean pf);
};
inline NondeterministicBoolean choose(LowerKleenean pt, LowerKleenean pf) {
    return NondeterministicBoolean(pt,pf);
}

//! \relates LowerKleenean
//! Returns an index \a i such that \c p[i] is definitely \c true, if one exists.
//! Loops infinitely if no \c p[i] can be shown to be definitely \c true.
size_t nondeterministic_choose_index(Array<LowerKleenean> const& p);

template<class P, class T> class Case {
    P _p; T _t;
  public:
    Case(P p, T t) : _p(p), _t(t) { }
    P condition() const { return _p; }
    T term() const { return _t; }
};

}

#endif
