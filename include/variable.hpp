/***************************************************************************
 *            variable.hpp
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

/*! \file variable.hpp
 *  \brief Internal variables
 */

#ifndef SYMBOLICORE_VARIABLE_HPP
#define SYMBOLICORE_VARIABLE_HPP

#include <cstdarg>
#include <iosfwd>
#include <iostream>

#include "macros.hpp"
#include "string.hpp"
#include "logical.decl.hpp"
#include "expression.decl.hpp"
#include "identifier.hpp"
#include "operations.hpp"
#include "container.hpp"

namespace SymboliCore {

class UntypedVariable;
class ExtendedUntypedVariable;


enum class VariableType : char { BOOLEAN, ENUMERATED, STRING, INTEGER, REAL };

template<class T> inline VariableType variable_type() { SYMBOLICORE_FAIL_MSG("Unknown variable type"); }
template<> inline constexpr VariableType variable_type<Boolean>() { return VariableType::BOOLEAN; }
template<> inline constexpr VariableType variable_type<String>() { return VariableType::STRING; }
template<> inline constexpr VariableType variable_type<Integer>() { return VariableType::INTEGER; }
template<> inline constexpr VariableType variable_type<Real>() { return VariableType::REAL; }

inline String class_name(const VariableType& tp) {
    switch(tp) {
        case VariableType::BOOLEAN: return "Boolean";
        case VariableType::ENUMERATED: return "Enumerated";
        case VariableType::STRING: return "String";
        case VariableType::INTEGER: return "Integer";
        case VariableType::REAL: return "Real";
        default:
            SYMBOLICORE_FAIL_MSG("Unhandled VariableType for output stream");
    }
}

//! \ingroup SymbolicModule
//! \brief A named variable of unknown type.
//! \see Variable
class UntypedVariable {
  public:
    //! \brief The name of the variable.
    const Identifier& name() const { return this->_name; }
    const VariableType& type() const { return this->_type; }
    Bool operator==(const UntypedVariable& other) const {
        return (this->name()==other.name()) && (this->type()==other.type()); }
    Bool operator!=(const UntypedVariable& other) const { return !(*this==other); }
    Bool operator<(const UntypedVariable& other) const {
        return this->name()<other.name() || (this->name()==other.name() && this->type() < other.type()); }
    friend OutputStream& operator<<(OutputStream& os, const UntypedVariable& var) {
        return os << var.name(); }
  protected:
    explicit UntypedVariable(const Identifier& nm, VariableType tp)
        : _name(nm), _type(tp) { }
  private:
    Identifier _name;
    VariableType _type;
};


//! \ingroup SymbolicModule
//! \brief A named variable of type \a T.
//! We support variables of type Boolean, Kleenean, String, Integer and Real.
//! \see Constant, Expression, Assignment, Space
//! \see Variables, TimeVariable,
//! \see LetVariable, PrimedVariable, DottedVariable
template<class T> class Variable
    : public UntypedVariable
    , public DeclareExpressionOperations<T>
{
  public:
    typedef T Type;
    typedef Variable<T> BaseType;
    //! \brief Construct a variable with name \a name.
    explicit Variable(const Identifier& name) : UntypedVariable(name,variable_type<T>()) { }
    Variable<T> const& base() const { return *this; }
    inline Assignment<Variable<T>,T> operator=(const T& c) const;
    Expression<T> create_zero() const { return Expression<T>::constant(0); }
  public:
    //! \brief Allows \a v to be used as the left-hand-side of an assignment \c let(v)=e defines the assignment \f$v:=e\f$.
    friend LetVariable<T> let(const Variable<T>& v);
    //! \brief Allows \a v to be used as the left-hand-side of an update rule in a discrete-time or hybrid system.
    //! \c prime(v)=e defines the update rule \f$v':=e\f$, where \f$v'\f$ is the value of \f$v\f$ after the jump.
    friend PrimedVariable<T> prime(const Variable<T>&);
    //! \brief Synonym for \ref prime(). \c next(v)=e defines the update rule \f$v':=e\f$, where \f$v'\f$ is the value of \f$v\f$ after the jump.
    friend PrimedVariable<T> next(const Variable<T>&);
    //! \brief Allows \a v to be used as the left-hand-side of a differential equation.
    //! \c dot(v)=e specifies the differential equation \f$\frac{d}{dt}v=e\f$.
    friend DottedVariable<Real> dot(const Variable<Real>&);
};

//! \ingroup SymbolicModule
//! \brief A special variable representing time.
//! \sa Variable
class TimeVariable : public Variable<Real> {
  public:
    TimeVariable() : Variable<Real>("t") { }
};

//! \ingroup SymbolicModule
//! \brief A list of variables of type \a T, with a common base name and an index.
//! \sa Variable
template<class T> class Variables : public List<Variable<T>> {
  public:
    //! \brief Construct \a n variables with base name \a name,
    //! so that the \a i<sup>th</sup> variable is <tt>x</tt><i>i</i>.
    Variables(Identifier name, SizeType n) : List<Variable<T>>() {
        this->reserve(n); for(SizeType i=0; i!=n; ++i) { this->append(Variable<T>(name+to_str(i))); } }
    inline List<Assignment<Variable<T>,T>> operator=(const List<T>& c) const;
    //! \brief The Construct \a n variables with name \a name.
    Variable<T> const& operator[] (SizeType i) const { return this->List<Variable<T>>::operator[](i); }
    List<Identifier> names() const {
        List<Variable<T>> const& lst=*this; return apply([](Variable<T>const& var){return var.name();},lst); }
};



enum class VariableCategory : char { SIMPLE, DOTTED, PRIMED };

inline OutputStream& operator<<(OutputStream& os, VariableCategory const& cat) {
    switch(cat) {
        case VariableCategory::SIMPLE: os << "SIMPLE"; break;
        case VariableCategory::PRIMED: os << "DOTTED"; break;
        case VariableCategory::DOTTED: os << "PRIMED"; break;
        default:
            SYMBOLICORE_FAIL_MSG("Unhandled VariableCategory for output streaming");
    }
    return os;
}

// A named variable of type \a T, possibly decorated by a "let", "dot" or "prime"
// representing a time derivative or updated value.
class ExtendedUntypedVariable
    : public UntypedVariable
{
    VariableCategory _category;
  public:
    VariableCategory category() const { return this->_category; }
  protected:
    explicit ExtendedUntypedVariable(const Identifier& name, VariableType type, VariableCategory category)
        : UntypedVariable(name, type), _category(category) { }
};

inline OutputStream& operator<<(OutputStream& os, ExtendedUntypedVariable const& var) {
    switch(var.category()) {
        case VariableCategory::SIMPLE: os << var.name(); break;
        case VariableCategory::PRIMED: os << "prime("<<var.name()<<")"; break;
        case VariableCategory::DOTTED: os << "dot("<<var.name()<<")"; break;
        default:
            SYMBOLICORE_FAIL_MSG("Unhandled VariableCategory "<<var.category());
    }
    return os;
}



template<class T> class ExtendedVariable
    : public ExtendedUntypedVariable
{
  public:
    typedef Variable<T> BaseType;
    Variable<T> base() const { return Variable<T>(this->name()); }
  protected:
    explicit ExtendedVariable(const Variable<T>& var, VariableCategory category) : ExtendedUntypedVariable(var.name(),variable_type<T>(),category) { }
};

//! \ingroup SymbolicModule
//! \brief A named variable of type \a T to be used on the left-hand-side of an assignment denoting an algebraic equation.
//! \see Variable, Assignment
template<class T> class LetVariable
    : public ExtendedVariable<T>
{
  public:
    //! \brief Decorate a simple variable to denote the left-hand-side of an algebraic equation.
    friend LetVariable<T> let(const Variable<T>& var) { return LetVariable<T>(var); }
    //! \brief Construct an assignment statement representing the algebraic equation \a var := \a expr.
    inline Assignment<Variable<T>,Expression<T>> operator=(const Expression<T>& expr) const;
  private:
    explicit LetVariable(const Variable<T>& var) : ExtendedVariable<T>(var,VariableCategory::SIMPLE) { }
};
template<class T> inline LetVariable<T> set(const Variable<T>& var) { return let(var); }


//! \ingroup SymbolicModule
//! \brief A named variable of type \a T decorated by a prime representing a value after a discrete jump.
//! \details \par \b Example
//! \snippet tutorials/symbolic_usage.cpp PrimedVariable_usage
//! \see Variable, Assignment
template<class T> class PrimedVariable
    : public ExtendedVariable<T>
{
  public:
    //! \brief Decorate a simple variable with a prime.
    friend PrimedVariable<T> prime(const Variable<T>& var) { return PrimedVariable<T>(var); }
    //! \brief Synonym for \ref prime().
    friend PrimedVariable<T> next(const Variable<T>& var) { return PrimedVariable<T>(var); }
    //! \brief Construct an assignment statement representing the difference equation \a var' := \a expr.
    inline Assignment<PrimedVariable<T>,Expression<T>> operator=(const Expression<T>& e) const;
  private:
    explicit PrimedVariable(const Variable<T>& var) : ExtendedVariable<T>(var,VariableCategory::PRIMED) { }
};
template<class T> inline PrimedVariable<T> next(const Variable<T>& var) { return prime(var); }


//! \ingroup SymbolicModule
//! \brief A named variable of type \a T decorated by a dot representing differentiation with respect to time.
//! \details \par \b Example
//! \snippet tutorials/symbolic_usage.cpp DottedVariable_usage
//! \see Variable, Assignment
template<class T> class DottedVariable
    : public ExtendedVariable<T>
{
  public:
    //! \brief Decorate a simple variable with a dot.
    friend DottedVariable<Real> dot(const Variable<Real>& var);
    //! \brief Construct an assignment statement representing the differential equation \a dot(var) := \a expr.
    inline Assignment<DottedVariable<T>,Expression<T>> operator=(const Expression<T>& e) const;
  private:
    explicit DottedVariable(const Variable<T>& var) : ExtendedVariable<T>(var,VariableCategory::DOTTED) { }
};
DottedVariable<Real> inline dot(const Variable<Real>& var) { return DottedVariable<Real>(var); }



template<class T> struct LetVariables {
    const List<Variable<T>> _lhs;
    LetVariables(const List<Variable<T>>& lhs) : _lhs(lhs) { }
    List<Assignment<Variable<T>,Expression<T>>> operator=(const List<Expression<T>>&);
};
template<class T> inline LetVariables<T> let(const List<Variable<T>>& lhs) { return LetVariables<T>(lhs); }
template<class T> inline LetVariables<T> set(const List<Variable<T>>& lhs) { return LetVariables<T>(lhs); }
inline LetVariables<Real> let(const InitializerList<Variable<Real>>& lhs) { return let(List<Variable<Real>>(lhs)); }
inline LetVariables<Real> set(const InitializerList<Variable<Real>>& lhs) { return set(List<Variable<Real>>(lhs)); }

template<class T> struct PrimedVariables {
    const List<Variable<T>> _lhs;
    PrimedVariables(const List<Variable<T>>& lhs) : _lhs(lhs) { }
    List<Assignment<PrimedVariable<T>,Expression<T>>> operator=(const List<Expression<T>>&);
    friend OutputStream& operator<<(OutputStream& os, PrimedVariables<T> const& dv) { return os <<"prime("<<dv._lhs<<")"; }
};
template<class T> inline PrimedVariables<T> prime(const List<Variable<T>>& lhs) { return PrimedVariables<T>(lhs); }
template<class T> inline PrimedVariables<T> next(const List<Variable<T>>& lhs) { return PrimedVariables<T>(lhs); }
inline PrimedVariables<Real> prime(const InitializerList<Variable<Real>>& lhs) { return prime(List<Variable<Real>>(lhs)); }
inline PrimedVariables<Real> next(const InitializerList<Variable<Real>>& lhs) { return next(List<Variable<Real>>(lhs)); }

template<class T> struct DottedVariables {
    const List<Variable<T>> _lhs;
    DottedVariables(const List<Variable<T>>& lhs) : _lhs(lhs) { }
    List<Assignment<DottedVariable<T>,Expression<T>>> operator=(const List<Expression<T>>&);
    friend OutputStream& operator<<(OutputStream& os, DottedVariables<T> const& dv) { return os <<"dot("<<dv._lhs<<")"; }
};
inline DottedVariables<Real> dot(const List<Variable<Real>>& lhs) { return DottedVariables<Real>(lhs); }
inline DottedVariables<Real> dot(const InitializerList<Variable<Real>>& lhs) { return DottedVariables<Real>(List<Variable<Real>>(lhs)); }

} // namespace SymboliCore

#endif /* SYMBOLICORE_VARIABLE_HPP */
