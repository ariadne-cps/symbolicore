/***************************************************************************
 *            expression.hpp
 *
 *  Copyright  2008-20  Pieter Collins
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

/*! \file expression.hpp
 *  \brief Internal expressions
 */

#ifndef SYMBOLICORE_EXPRESSION_HPP
#define SYMBOLICORE_EXPRESSION_HPP

#include <cstdarg>
#include <iosfwd>
#include <iostream>

#include "macros.hpp"
#include "container.hpp"
#include "writable.hpp"

#include "constant.hpp"
#include "variable.hpp"
#include "valuation.hpp"
#include "operations.hpp"
#include "operators.hpp"
#include "logical.decl.hpp"

namespace SymboliCore {

template<class T> class Set;
template<class T> struct DeclareExpressionOperations;
template<class X> struct ExpressionNode;

template<class T> class PrefixExpressionWriter;
template<class T> class InfixExpressionWriter;

//! \brief A simple expression in named variables.
//!    \tparam T The type represented by the expression
//!
//! The independent variables are given string names, rather than an integer index.
//! Expressions in different variables may be combined; the argument variables of the resulting expression
//! are all variables occurring in all expression.
//! Expressions may be manipulated symbolically.
//!
//! \see Constant,  Variable,  Assignment
template<class T>
class Expression
    : public DeclareExpressionOperations<T>
{
    typedef SharedPointer<const ExpressionNode<T>> Pointer;
    static Writer<Expression<T>> _default_writer;
  public:
    static Void set_default_writer(Writer<Expression<T>> w) { _default_writer=w; }
    static Writer<Expression<T>> default_writer() { return _default_writer; }
  public:
    typedef Real NumericType;
    //! \brief The type represented by the expression.
    typedef T ValueType;
    typedef Constant<T> ConstantType;
    typedef Variable<T> VariableType;
  public:
    // Use template formulation to avoid ambiguity treating Expression(0) as a pointer construction.
    template<ConvertibleTo<SharedPointer<const ExpressionNode<T>>> P>
        explicit Expression(P const& eptr) : _root(eptr) { }
  public:
    //! \brief Default expression is a constant with default value.
    Expression();
    //! \brief Construct an expression from a numerical value.
    Expression(const T& c);
    template<ConvertibleTo<T> U> Expression(const U& c) : Expression(T(c)) { }
    //! \brief Construct an expression from a named constant.
    Expression(const Constant<T>& c);
    //! \brief Construct an expression from a variable.
    Expression(const Variable<T>& v);
    //! \brief Construct a constant expression from a value.
    static Expression<T> constant(const ValueType& c);
    //! \brief Construct a constant expression from a named variable value.
    static Expression<T> constant(const Constant<ValueType>& c);
    //! \brief Construct an expression from a name.
    static Expression<T> variable(const Identifier& c);

    //! \brief Create the zero element.
    Expression<T> create_zero() const { return Expression<T>::constant(T()); }
    //! \brief Create a constant element.
    Expression<T> create_constant(T const& t) const { return Expression<T>::constant(t); }
  public:
    Operator op() const;
    OperatorCode code() const;
    OperatorKind kind() const;
    const ValueType& val() const;
    const Identifier& var() const;
    const Expression<T>& arg() const;
    const Int& num() const;
    const Expression<T>& arg1() const;
    const Expression<T>& arg2() const;
    template<class A> const Expression<A>& cmp1(A* dummy=0) const;
    template<class A> const Expression<A>& cmp2(A* dummy=0) const;
    //! \brief Write to an output stream.
    friend OutputStream& operator<<(OutputStream& os, Expression<T> const& e) { return e._write(os); }
    //! \brief A writer for Expression objects using prefix notation.
    friend class PrefixExpressionWriter<T>;
    //! \brief A write for Expression objects using infix notation.
    friend class InfixExpressionWriter<T>;
  public:
    //! \brief The variables needed to compute the expression.
    Set<UntypedVariable> arguments() const;
  public:
    SharedPointer<const ExpressionNode<T>> node_ptr() const { return _root; }
    const ExpressionNode<T>* node_raw_ptr() const { return _root.operator->(); }
    const ExpressionNode<T>& node_ref() const { return _root.operator*(); }
  private:
    OutputStream& _write(OutputStream& os) const;
  private:
    SharedPointer<const ExpressionNode<T>> _root;
};

//!@{
//! \name Evaluation and related operations.
//! \related Expression

template<class T> LogicType<T> evaluate(const Expression<LogicType<T>>& e, const Valuation<T>& x);
template<class T> T evaluate(const Expression<T>& e, const Valuation<T>& x);

//! \brief Evaluate expression \a e on argument \a x which is a map of variable identifiers to values of type \c A.
template<class T> LogicType<T> evaluate(const Expression<LogicType<T>>& e, const Map<Identifier,T>& x);
//! \brief Evaluate expression \a e on argument \a x which is a map of variable identifiers to values of type \c A.
template<class T> T evaluate(const Expression<T>& e, const Map<Identifier,T>& x);

Boolean evaluate(const Expression<Boolean>& e, const DiscreteValuation& q);
template<class X> X evaluate(const Expression<Real>& e, const ContinuousValuation<X>&);
template<class X> Boolean evaluate(const Expression<Boolean>&, const ContinuousValuation<X>&);

//! \brief Extract the arguments of expression \a e.
template<class T> Set<Identifier> arguments(const Expression<T>& e);

//! \brief Substitute all occurrences of variable \a v of type \c Y with constant value \a c.
template<class T, class Y> Expression<T> substitute(const Expression<T>& e, const Variable<Y>& v, const Y& c);
//! \brief Substitute all occurrences of variable \a v of type \c Y with expression value \a se.
template<class T, class Y> Expression<T> substitute(const Expression<T>& e, const List< Assignment< Variable<Y>,Expression<Y> > >& a);
template<class T, class Y> Vector<Expression<T>> substitute(const Vector<Expression<T>>& e, const List< Assignment< Variable<Y>,Expression<Y> > >& a);

//!@}

//!@{
//! \name Operations on expressions.
//! \related Expression

//! \brief The derivative of the expression \a e with respect to the variable \a v.
Expression<Real> derivative(const Expression<Real>& e, Variable<Real> v);
//!@}


//!@{
//! \name Testing the form of expressions.
//! \related Expression

//! \brief Returns \a true if the expression\a e is syntactically equal to the constant \a c.
template<class T> Bool is_constant(const Expression<T>& e, const SelfType<T>& c);

//! \brief Returns \a true if the expression \a e is syntactically equal to the variable with name \a vn.
template<class T> Bool is_variable(const Expression<T>& e, const Identifier& vn);

//! \brief Returns \a true if the expression \a e is syntactically equal to the variable \a v.
template<class T> Bool is_variable(const Expression<T>& e, const Variable<T>& v);

//! \brief Returns \a true if the expression \a e is syntactically constant in the variables \a vs.
template<class T> Bool is_constant_in(const Expression<T>& e, const Set<Variable<T>>& vs);
//! \brief Returns \a true if the expression \a e is syntactically affine in the variables \a vs.
Bool is_affine_in(const Expression<Real>& e, const Set<Variable<Real>>& vs);
//! \brief Returns \a true if the vector expression \a e is syntactically affine in the variables \a vs.
Bool is_affine_in(const Vector<Expression<Real>>& e, const Set<Variable<Real>>& vs);
//! \brief Returns \a true if the vector expression \a e is syntactically additive (possibly with multipliers) in the variables \a vs.
Bool is_additive_in(const Vector<Expression<Real>>& e, const Set<Variable<Real>>& vs);
Bool is_additive_in(const Expression<Real>& e, const Variable<Real>& v);
//! \brief Returns \a true if the expression \a e is syntactically polynomial in the variables \a vs.
Bool is_polynomial_in(const Vector<Expression<Real>>& e, const Set<Variable<Real>>& vs);
Bool is_polynomial_in(const Expression<Real>& e, const Set<Variable<Real>>& vs);

//! \brief Returns true if the expressions are mutual negations. <br>
//! Currently can only test for pairs of the form (a1<=a2; a1>=a2),  (a1<=a2; a2<=a1)
//! or (a1>=a2; a2>=a1).
Bool opposite(Expression<Kleenean> p, Expression<Kleenean> q);
//!@}

//!@{
//! \name Equality and ordering.
//! \related Expression

//! \brief Tests whether two expressions are identical.
template<class T> Bool identical(const Expression<T>& e1, const Expression<T>& e2);
//! \brief Check the ordering of two expressions \a e1 and \a e2, by identifying whether \a e1 precedes \a e2.
template<class T> Bool before(Expression<T> const& e1, Expression<T> const& e2);
//!@}

//!@{
//! \name Complexity checks and simplification.
//! \related Expression

//! \brief Count the number of nodes in the expression \a e.
template<class T> SizeType count_nodes(const Expression<T>& e);
//! \brief Count the number of distinct (i.e., having identical representation) nodes in the expression \a e.
template<class T> SizeType count_distinct_nodes(const Expression<T>& e);
//! \brief Count the number of distinct node pointers in the expression \a e.
template<class T> SizeType count_distinct_node_pointers(const Expression<T>& e);

//! \brief Simplify the expression \a e, such as by eliminating double negations \f$-(-e) \mapsto e\f$.
template<class T> Expression<T> simplify(const Expression<T>& e);
//! \brief Eliminate common subexpression in \a e by replacing identical nodes.
template<class T> Void eliminate_common_subexpressions(Expression<T>& e);
template<class T> Void eliminate_common_subexpressions(Vector<Expression<T>>& e);
//!@}




//!@{
//! \name Conversion to/from functions and formulae.
//! \related Expression

//! \brief Make a formula in terms of numbered coordinates from an expression in named variables.
Formula<EffectiveNumber> make_formula(const Expression<Real>& e, const Map<Identifier,SizeType>& v);
Formula<EffectiveNumber> make_formula(const Expression<Real>& e, const Space<Real>& spc);
Vector<Formula<EffectiveNumber>> make_formula(const Vector<Expression<Real>>& e, const Space<Real>& spc);
Formula<EffectiveNumber> make_formula(const Expression<Real>& e, const Variable<Real>& var);
Vector<Formula<EffectiveNumber>> make_formula(const Vector<Expression<Real>>& e, const Variable<Real>& spc);
Formula<EffectiveNumber> make_formula(const Expression<Real>& e, const List<Variable<Real>>& vars);
Formula<EffectiveNumber> make_formula(const Expression<Real>& out, const List<Assignment<Variable<Real>,Expression<Real>>>& aux, const Space<Real> spc);
Vector<Formula<EffectiveNumber>> make_formula(const Vector<Expression<Real>>& out, const List<Assignment<Variable<Real>,Expression<Real>>>& aux, const Space<Real> spc);

//! \brief Make a function on the real line given an expression in a single argument variable.
ScalarUnivariateFunction<EffectiveTag> make_function(const Variable<Real>& v, const Expression<Real>& e);
//! \brief Make a vector function on the real line given a vector of expressions in a single argument variable.
VectorUnivariateFunction<EffectiveTag> make_function(const Variable<Real>& v, const Vector<Expression<Real>>& e);
//! \brief Make a function on a Euclidean domain given an ordered list including all argument variables.
ScalarMultivariateFunction<EffectiveTag> make_function(const Space<Real>& s, const Expression<Real>& e);
//! \brief Make a function on a Euclidean domain given an ordered list including all argument variables.
VectorMultivariateFunction<EffectiveTag> make_function(const Space<Real>& s, const Vector<Expression<Real>>& e);

//! \brief Make a function on a Euclidean domain given an ordered list including all argument variables.
Expression<Real> make_expression(const ScalarMultivariateFunction<EffectiveTag>& f, const Space<Real>& s);

Expression<Real> make_expression(const Formula<Real>& f, const Space<Real>& s);
//!@}


//! \brief Prefix notation for writing an Expression
template<class T> class PrefixExpressionWriter : public WriterInterface<Expression<T>> {
    virtual OutputStream& _write(OutputStream& os, Expression<T> const& e) const final override;
};
//! \brief Infix notation for writing an Expression
template<class T> class InfixExpressionWriter : public WriterInterface<Expression<T>> {
    virtual OutputStream& _write(OutputStream& os, Expression<T> const& e) const final override;
};


} // namespace SymboliCore

#endif /* SYMBOLICORE_EXPRESSION_HPP */
