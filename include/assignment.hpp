/***************************************************************************
 *            assignment.hpp
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

/*! \file assignment.hpp
 *  \brief Assignment expressions
 */

#ifndef SYMBOLICORE_ASSIGNMENT_HPP
#define SYMBOLICORE_ASSIGNMENT_HPP

#include <cstdarg>
#include <iostream>
#include <string>


#include "macros.hpp"
#include "container.hpp"
#include "stlio.hpp"
#include "string.hpp"
#include "variable.hpp"
#include "expression.hpp"

namespace SymboliCore {

class StateSpace;

class Identifier;
typedef Set<UntypedVariable> VariableSet;
typedef Set<Variable<Real>> RealVariableSet;


//! \ingroup SymbolicModule
//! \brief An assignment statement of the form \f$v:=e\f$.
//!   \tparam V The variable to be assigned to (left-hand-side). May be an extended variable (e.g. <code>dot(v)</code>)
//!   \tparam E The expression to be assigned (right-hand-side).
//! \see Variable, Expression
template<class V, class E>
class Assignment
{
  public:
    typedef V VariableType;
    typedef E ExpressionType;

    //! \brief Construct the assignment \c v=e.
    Assignment(const V& v, const E& e) : lhs(v), rhs(e) { }
    //! \brief The variable on the left-hand side.
    const V& variable() const { return this->lhs; }
    //! \brief The expression on the right-hand side.
    const E& expression() const { return this->rhs; }
    const V& left_hand_side() const { return this->lhs; }
    const E& right_hand_side() const { return this->rhs; }
    V lhs; E rhs;
};
template<class V, class E> Bool operator<(const Assignment<V,E>& a1, const Assignment<V,E>& a2) {
    return a1.lhs < a2.lhs;
}
template<class V, class E> inline OutputStream& operator<<(OutputStream& os, const Assignment<V,E>& a) {
    return os<<a.lhs<<"="<<a.rhs;
}

template<class V, class E> typename V::BaseType left_hand_side(const Assignment<V,E>& assignment) {
    return assignment.lhs.base();
}
template<class V, class E> List<typename V::BaseType> left_hand_sides(const List<Assignment<V,E>>& assignments) {
    return elementwise([](Assignment<V,E>const&a){return a.lhs.base();},assignments);
}
template<class V, class E> List<E> right_hand_sides(const List<Assignment<V,E>>& assignments) {
    return elementwise([](Assignment<V,E>const&a){return a.rhs;},assignments);
}


template<class T>
class Assignment<Variable<T>,T>
{
  public:
    Assignment(const Variable<T>& l, const T& r) : lhs(l), rhs(r) { }
    const Variable<T>& variable() const { return this->lhs; }
    const T& value() const { return this->rhs; }
    const Expression<T>& expression() const { return Expression<T>(this->rhs); }
    const Variable<T>& left_hand_side() const { return this->lhs; }
    const T& right_hand_side() const { return this->rhs; }
    inline operator Valuation<T,T> () const;
    Variable<T> lhs; T rhs;
};
template<class T> inline Assignment<Variable<T>,T>
Variable<T>::operator=(const T& val) const {
    return Assignment<Variable<T>,T>(*this,val);
}



template<class T> inline Assignment<Variable<T>,Expression<T>>
LetVariable<T>::operator=(const Expression<T>& expr) const {
    return Assignment<Variable<T>,Expression<T>>(this->base(),Expression<T>(expr)); }

template<class T> inline Assignment<PrimedVariable<T>,Expression<T>>
PrimedVariable<T>::operator=(const Expression<T>& expr) const {
    return Assignment<PrimedVariable<T>,Expression<T>>(*this,Expression<T>(expr)); }

template<class T> inline Assignment<DottedVariable<T>,Expression<T>>
DottedVariable<T>::operator=(const Expression<T>& expr) const {
    return Assignment<DottedVariable<T>,Expression<T>>(*this,expr); }

template<class T> inline List<Assignment<Variable<T>,Expression<T>>> LetVariables<T>::operator=(const List<Expression<T>>& rhs) {
    return elementwise([](Variable<T>const&l,Expression<T>const&r){return let(l)=r;},this->_lhs,rhs);
}

template<class T> inline List<Assignment<PrimedVariable<T>,Expression<T>>> PrimedVariables<T>::operator=(const List<Expression<T>>& rhs) {
    return elementwise([](Variable<T>const&l,Expression<T>const&r){return prime(l)=r;},this->_lhs,rhs);
}

template<class T> inline List<Assignment<DottedVariable<T>,Expression<T>>> DottedVariables<T>::operator=(const List<Expression<T>>& rhs) {
    return elementwise([](Variable<T>const&l,Expression<T>const&r){return dot(l)=r;},this->_lhs,rhs);
}


//! \relates Assignment
//! \brief Sort a list of assignments so that an assigned-to variable is only used after it is assigned.
List<RealAssignment> algebraic_sort(const List<RealAssignment>& assignments);

//! \relates Assignment
//! \brief Make a function on a Euclidean domain given an ordered list of assignments,
//! so that assigned variables can be used in the definition of other variables.
EffectiveVectorMultivariateFunction make_function(const List<RealAssignment>& assignments, const RealSpace& arguments);

} // namespace SymboliCore

#include "symbolic/valuation.hpp"
namespace SymboliCore {
template<class T> inline Assignment<Variable<T>,T>::operator Valuation<T> () const { Valuation<T> r; r.insert(this->lhs,this->rhs); return r; }
} // namespace SymboliCore


#endif // SYMBOLICORE_ASSIGNMENT_HPP
