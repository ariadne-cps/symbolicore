/***************************************************************************
 *            valuation.hpp
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

/*! \file valuation.hpp
 *  \brief Valuations over named variables.
 */

#ifndef SYMBOLICORE_VALUATION_HPP
#define SYMBOLICORE_VALUATION_HPP

#include <cstdarg>
#include <iostream>
#include <string>

#include "helper/macros.hpp"
#include "helper/container.hpp"

#include "integer.hpp"
#include "variable.hpp"
#include "expression.hpp"

namespace SymboliCore {

using Helper::Pair;
using Helper::Map;
using Helper::Set;

// Sequencing operators to make Valuation objects or objects convertible to Valuations.
template<class X> Pair<Variable<X>,X> operator|(const Variable<X>& v, const typename Variable<X>::Type& c) { return Pair<Variable<X>,X>(v,c); }
template<class X> Pair<Variable<X>,X> operator|(const Variable<X>& v, const Constant<X>& c) { return Pair<Variable<X>,X>(v,c.value()); }

template<class T, class X> class Valuation;

typedef Space<Real> RealSpace;
//! \relates Valuation \brief A Valuation taking integer values.
typedef Valuation<Integer> IntegerValuation;
//! \relates Valuation \brief A Valuation taking string values.
typedef Valuation<String> StringValuation;
//! \relates Valuation \brief A Valuation taking real values.
typedef Valuation<Real> RealValuation;

//! \ingroup SymbolicModule
//! \brief A valuation of named variables of mathematical type \a T, with values represented by values of concrete type \a X,
//!   where X defaults to \a T.
//! \par \b Example
//! \snippet tutorials/symbolic_usage.cpp Valuation_usage
//! \see DiscreteValuation, ContinuousValuation, HybridValuation, DiscreteLocation
template<class T, class X>
class Valuation
{
  public:
    typedef typename Map<Identifier,X>::ConstIterator ConstIterator;
    //! \brief The abstract mathematical type represented by variables.
    typedef T Type;
    //! \brief The concrete class of the values of the variables.
    typedef X ValueType;
  public:
    //! \brief Construct a valuation assigning values to no variables.
    Valuation() { }
    //! \brief Construct from a mapping from \em names of variables to values.
    Valuation(const Map<Identifier,ValueType>& m) : _values(m) { }
    Valuation(const Map<Variable<Type>,ValueType>& m) {
        for(auto val : m) { this->_values.insert(val.first.name(),val.second); } }
    Valuation(Array<X> ary, Space<T> const& spc){
        for(size_t i=0; i!=ary.size(); ++i) { this->insert(spc[i],ary[i]); } }
    Valuation(const Assignment<Variable<T>,X>& a);
    Valuation(const List<Assignment<Variable<T>,X> >& la);
    Valuation(const initializer_list<Pair<Variable<T>,X> >& lst);
    void insert(const Variable<Type>& v, const ValueType& s) { this->_values.insert(v.name(),s); }
    //! \brief %Set the value associated with variable \a v to \a s.
    void set(const Variable<Type>& v, const ValueType& s) { this->_values[v.name()]=s; }
    //! \brief Get the value associated with variable \a v.
    const ValueType& get(const Variable<Type>& v) const { return _values[v.name()]; }
    const ValueType& operator[](const Identifier& nm) const { return _values[nm]; }
    ValueType& operator[](const Identifier& nm) { return _values[nm]; }
    //! \brief Extract the value associated with the variable \a v.
    const ValueType& operator[](const Variable<Type>& v) const { return _values[v.name()]; }
    ValueType& operator[](const Variable<Type>& v) { return _values[v.name()]; }
    const Map<Identifier,ValueType>& values() const { return _values; }
    Map<Identifier,ValueType>& values() { return _values; }
    Set<Identifier> defined() const { return _values.keys(); }
    ConstIterator begin() const { return _values.begin(); }
    ConstIterator end() const { return _values.end(); }
    Vector<ValueType> operator[](const Space<Type>& spc) {
        return Vector<ValueType>(spc.dimension(),[&](size_t i){return _values[spc[i]];}); }
  public:
    Map<Identifier,ValueType> _values;
};

template<class T, class X> bool operator==(const Valuation<T,X>& v1, const Valuation<T,X>& v2) {
    bool identical = true;
    const Map<Identifier,X>& v1sm=v1.values();
    const Map<Identifier,X>& v2sm=v2.values();
    typename Map<Identifier,X>::ConstIterator v1iter=v1sm.begin();
    typename Map<Identifier,X>::ConstIterator v2iter=v2sm.begin();

    while(v1iter!=v1sm.end() && v2iter!=v2sm.end()) {
        if(v1iter->first==v2iter->first) {
            if(v1iter->second != v2iter->second) { return false; }
            ++v1iter; ++v2iter;
        } else if(v1iter->first<v2iter->first) {
            identical=false; ++v1iter;
        } else {
            identical=false; ++v2iter;
        }
    }
    if(v1iter!=v1sm.end() || v2iter!=v2sm.end()) { identical=false; }
    if(!identical) { HELPER_THROW(std::runtime_error,"operator==(DiscreteLocation,DiscreteLocation)",
                                   "Valuations "<<v1<<" and "<<v2<<" are not identical, but no values differ."); }
    return true;
}

//! \brief A valuation of named variables taking String or Integer values.
//! \sa DiscreteLocation
class DiscreteValuation
    : public Valuation<String>
    , public Valuation<Integer>
{
  public:
    typedef String StringType;
    typedef Integer IntegerType;
  public:
    DiscreteValuation() { }
    DiscreteValuation(const Map<Identifier,StringType>& sm) : Valuation<String>(sm) { }
    DiscreteValuation(const Map<Identifier,IntegerType>& zm) : Valuation<Integer>(zm) { }
    DiscreteValuation(const Map<Identifier,StringType>& sm,const Map<Identifier,IntegerType>& zm) : Valuation<String>(sm), Valuation<Integer>(zm) { }
    using Valuation<String>::insert; using Valuation<Integer>::insert;
    using Valuation<String>::get; using Valuation<Integer>::get;
    using Valuation<String>::set; using Valuation<Integer>::set;
    //! \brief Extract the value associated with the string variable \a v.
    const String& operator[](const Variable<String>& v) const { return this->Valuation<String>::operator[](v); }
    //! \brief Extract the value associated with the integer variable \a v.
    const Integer& operator[](const Variable<Integer>& v) const { return this->Valuation<Integer>::operator[](v); }
    const Map<Identifier,StringType>& string_values() const { return Valuation<String>::values(); }
    const Map<Identifier,IntegerType>& integer_values() const { return Valuation<Integer>::values(); }
};

inline bool operator==(const DiscreteValuation& v1, const DiscreteValuation& v2) {
    return static_cast<const StringValuation&>(v1)==static_cast<const StringValuation&>(v2)
        && static_cast<const IntegerValuation&>(v1)==static_cast<const IntegerValuation&>(v2); }

//! \brief A valuation of named real variables taking values of concrete class \a X.
template<class X>
class ContinuousValuation
    : public Valuation<Real,X>
{
  public:
    ContinuousValuation() : Valuation<Real,X>() { }
    ContinuousValuation(const Map<Variable<Real>,X>& rm) : Valuation<Real,X>(rm) { }
    typedef X RealType;
};


//! \brief A valuation of named variables of String, Integer and Real quantities, with the real variables taking values in concrete type \a X.
template<class X> class HybridValuation
    : public DiscreteValuation
    , public ContinuousValuation<X>
{
  public:
    HybridValuation(const Map<Identifier,String>& sm, const Map<Identifier,X>& rm) :
        DiscreteValuation(sm), ContinuousValuation<X>(rm) { }
    HybridValuation(const StringValuation& sv, const Map<Identifier,X>& rm) :
        DiscreteValuation(sv.values()), ContinuousValuation<X>(rm) { }
    using DiscreteValuation::set;
    using ContinuousValuation<X>::set;
    using DiscreteValuation::get;
    using ContinuousValuation<X>::get;
    using DiscreteValuation::operator[];
    using ContinuousValuation<X>::operator[];
    const Map<Identifier,X>& real_values() const { return this->ContinuousValuation<X>::values(); }
    Map<Identifier,X>& real_values() { return this->ContinuousValuation<X>::values(); }
};

inline ostream& operator<<(ostream& os, const DiscreteValuation& val) {
    const char open='('; const char mid='|'; char const close=')';
    //const char open='{'; const char mid=':'; char const close='}';
    char sep=open;
    for(Map<Identifier,DiscreteValuation::StringType>::ConstIterator iter=val.string_values().begin(); iter!=val.string_values().end(); ++iter) {
        os << sep << iter->first << mid << iter->second; sep=','; }
    for(Map<Identifier,DiscreteValuation::IntegerType>::ConstIterator iter=val.integer_values().begin(); iter!=val.integer_values().end(); ++iter) {
        os << sep << iter->first << mid << iter->second; }
    return os << close;
}

template<class X> inline ostream& operator<<(ostream& os, const ContinuousValuation<X>& val) {
    return os << val.values();
}

template<class X> inline ostream& operator<<(ostream& os, const Valuation<X>& val) {
    const char open='('; const char mid ='|'; const char close=')'; const char sep=',';
    //const char open='{'; const char mid=':'; char const close='}'; char const sep=",";
    os << open;
    for(typename Map<Identifier,X>::ConstIterator iter=val.values().begin(); iter!=val.values().end(); ++iter) {
        if(iter!=val.values().begin()) { os << sep; }
        os << iter->first << mid << iter->second;
    }
    return os << close;
}

template<class V, class X> Valuation<V,X>::Valuation(const Assignment<Variable<V>,X>& a) { this->insert(a.lhs,a.rhs); }
template<class V, class X> Valuation<V,X>::Valuation(const List<Assignment<Variable<V>,X> >& la) {
    for(size_t i=0; i!=la.size(); ++i) { this->insert(la[i].lhs,la[i].rhs); } }
template<class V, class X> Valuation<V,X>::Valuation(const initializer_list<Pair<Variable<V>,X> >& la) {
    for(auto iter=la.begin(); iter!=la.end(); ++iter) { this->insert(iter->first,iter->second); } }

template<class X> X evaluate(const Expression<Real>& e, const ContinuousValuation<X>&);
template<class X> Boolean evaluate(const Expression<Boolean>&, const ContinuousValuation<X>&);

} // namespace SymboliCore

#endif // SYMBOLICORE_VALUATION_HPP
