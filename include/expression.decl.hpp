/***************************************************************************
 *            expression.decl.hpp
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

/*! \file expression.decl.hpp
 *  \brief Declarations of expression classes and type aliases.
 */

#ifndef SYMBOLICORE_EXPRESSION_DECL_HPP
#define SYMBOLICORE_EXPRESSION_DECL_HPP

namespace SymboliCore {

using Utility::List;

class Integer;
class Natural;

template<class T> class Vector;

class Identifier;

template<class T> class Constant;
template<class T> class Variable;
template<class T> class Variables;
template<class T> class Space;
template<class T> class Expression;

template<class T> class LetVariable;
template<class T> class DottedVariable;
template<class T> class PrimedVariable;
template<class V,class E> class Assignment;

//!@{
//! \ingroup SymbolicModule
//! \relates Constant
//! \name Type synonyms
using StringConstant = Constant<String>; //!< <p/>
using IntegerConstant = Constant<Integer>; //!< <p/>
using RealConstant = Constant<Real>; //!< <p/>
//!@}

//!@{
//! \ingroup SymbolicModule
//! \relates Variable
//! \name Type synonyms
using BooleanVariable = Variable<Boolean>; //!< <p/>
using KleeneanVariable = Variable<Kleenean>; //!< <p/>
using StringVariable = Variable<String>; //!< <p/>
using IntegerVariable = Variable<Integer>; //!< <p/>
using RealVariable = Variable<Real>; //!< <p/>
//!@}

//!@{
//! \relates Variables
//! \name Type synonyms
using RealVariables = Variables<Real>; //!< <p/>
//!@}

//!@{
//! \relates LetVariable
//! \name Type synonyms
using LetStringVariable = LetVariable<String>; //!< <p/>
using LetIntegerVariable = LetVariable<Integer>; //!< <p/>
using LetRealVariable = LetVariable<Real>; //!< <p/>
//!@}

//!@{
//! \relates PrimedVariable
//! \name Type synonyms
using PrimedStringVariable = PrimedVariable<String>; //!< <p/>
using PrimedIntegerVariable = PrimedVariable<Integer>; //!< <p/>
using PrimedRealVariable = PrimedVariable<Real>; //!< <p/>
//!@}

//! \relates DottedVariable
//! \name Type synonyms
using DottedRealVariable = DottedVariable<Real>; //!< <p/>
//!@}

//!@{
//! \relates Expression
//! \name Type synonyms
using BooleanExpression = Expression<Boolean>; //!< <p/>
using KleeneanExpression = Expression<Kleenean>; //!< <p/>
using StringExpression = Expression<String>; //!< <p/>
using IntegerExpression = Expression<Integer>; //!< <p/>
using RealExpression = Expression<Real>; //!< <p/>
using RealExpressions = List<Expression<Real>>; //!< <p/>
using RealExpressionVector = Vector<Expression<Real>>; //!< <p/>

using DiscretePredicate = Expression<Boolean>; //!< \brief A decidable predicate over discrete variables.
using ContinuousPredicate = Expression<Kleenean>; //!< \brief A quasidecidable predicate over continuous variables.
//!@}

//!@{
//! \relates Assignment
//! \name Type synonyms
using StringAssignment = Assignment<StringVariable,StringExpression>; //!< <p/>
using PrimedStringAssignment = Assignment<PrimedStringVariable,StringExpression>; //!< <p/>
using IntegerAssignment = Assignment<IntegerVariable,IntegerExpression>; //!< <p/>
using PrimedIntegerAssignment = Assignment<PrimedIntegerVariable,IntegerExpression>; //!< <p/>
using RealAssignment = Assignment<RealVariable,RealExpression>; //!< <p/>
using PrimedRealAssignment = Assignment<PrimedRealVariable,RealExpression>; //!< <p/>
using DottedRealAssignment = Assignment<DottedRealVariable,RealExpression>; //!< <p/>

using PrimedStringAssignments = List<PrimedStringAssignment>; //!< <p/>
using RealAssignments = List<RealAssignment>; //!< <p/>
using PrimedRealAssignments = List<PrimedRealAssignment>; //!< <p/>
using DottedRealAssignments = List<DottedRealAssignment>; //!< <p/>

using RealConstantAssignment = Assignment<RealVariable,Real>; //!< <p/>
//!@}

typedef Space<Real> RealSpace;

template<class T, class X=T> class Valuation;
//!@{
//! \relates Valuation
//! \name Type synonyms
using StringValuation = Valuation<String>; //!< <p/>
using IntegerValuation = Valuation<Integer>; //!< <p/>
//!@}

class DiscreteValuation;
template<class X> class ContinuousValuation;




template<class UB> class Interval;
using RealInterval = Interval<Real>;

template<class UB> class VariableInterval;
template<class UB> class VariableLowerInterval;
template<class UB> class VariableUpperInterval;
template<class IVL> class VariablesBox;

//!@{
//! \relates RealVariableInterval
//! \name Type synonyms
using RealVariableInterval = VariableInterval<Real>; //!< <p/>
using RealVariableLowerInterval = VariableLowerInterval<Real>; //!< <p/>
using RealVariableUpperInterval = VariableUpperInterval<Real>; //!< <p/>
using RealVariableIntervals = List<RealVariableInterval>; //!< <p/>
//!@}

//!@{
//! \relates VariablesBox
//! \name Type synonyms
using RealVariablesBox = VariablesBox<RealInterval>; //!< <p/>
//!@}

} // namespace SymboliCore

#endif /* SYMBOLICORE_EXPRESSION_DECL_HPP */
