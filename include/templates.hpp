/***************************************************************************
 *            templates.hpp
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

/*! \file templates.hpp
 *  \brief
 */

#include "operators.hpp"
#include "sequence.hpp"

#ifndef SYMBOLICORE_EXPRESSION_TEMPLATES_HPP
#define SYMBOLICORE_EXPRESSION_TEMPLATES_HPP

namespace SymboliCore {

/************ Symbolic ************************************************/

template<class OP, class... AS> struct Symbolic;

template<class C> struct Symbolic<Cnst,C> {
    Cnst _op; C _val;
    explicit Symbolic(C c) : _op(), _val(c) { }
    Symbolic(Cnst o, C c) : _op(o), _val(c) { }
    C val() const { return _val; }
    template<class T> explicit operator T() const { return static_cast<T>(_val); }
    template<class... AS> auto operator() (AS... vals) const -> C { return _val; }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) { return os << expr._val; }
};

template<class I> struct Symbolic<Var,I> {
    Var _op; I _ind;
    explicit Symbolic(I i) : _op(), _ind(i) { }
    Symbolic(Var o, I i) : _op(o), _ind(i) { }
    template<class AS> auto operator() (AS vals) const -> decltype(vals[_ind]) { return vals[_ind]; }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) { return os << expr._ind; }
};

template<class O, class A> struct Symbolic<O,A> {
    O _op; A _arg;
    O op() const { return _op; } A arg() const { return _arg; }
    Symbolic(O o, A a) : _op(o), _arg(a) { }
//    template<class T> explicit operator T() const { return _op(static_cast<T>(_arg)); }
    template<class... AS> auto operator() (AS... vals) const -> decltype(_op(_arg(vals...))) {
        return _op(_arg(vals...)); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << expr._op.code() << "(" << expr._arg << ")"; }
};

template<class O, class A1, class A2> struct Symbolic<O,A1,A2> {
    O _op; A1 _arg1; A2 _arg2;
    O op() const { return _op; } A1 arg1() const { return _arg1; } A2 arg2() const { return _arg2; }
    Symbolic(O o, A1 a1, A2 a2) : _op(o), _arg1(a1), _arg2(a2) { }
//    template<class T> explicit operator T() const { return _op(static_cast<T>(_arg1),static_cast<T>(_arg2)); }
    template<class... AS> auto operator() (AS... vals) const -> decltype(_op(_arg1(vals...),_arg2(vals...))) {
        return _op(_arg1(vals...),_arg2(vals...)); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << expr._op.code() << "(" << expr._arg1 << "," << expr._arg2 << ")"; }
};

template<class A, class N> struct Symbolic<Pow,A,N> {
    typedef Pow O;
    O _op; A _arg; N _num;
    O op() const { return _op; } A arg() const { return _arg; } N num() const { return _num; }
    Symbolic(O o, A a, N n) : _op(o), _arg(a), _num(n) { }
//    template<class T> explicit operator T() const { return _op(static_cast<T>(_arg),_num); }
    template<class... AS> auto operator() (AS... vals) const -> decltype(_op(_arg(vals...),_num)) {
        return _op(_arg(vals...),_num); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << expr._op.code() << "(" << expr._arg << "," << expr._num << ")"; }
};

template<class A, class N> struct Symbolic<GradedElementaryOperator,A,N> {
    typedef GradedElementaryOperator O;
    O _op; A _arg; N _num;
    O op() const { return _op; } A arg() const { return _arg; } N num() const { return _num; }
    Symbolic(O o, A a, N n) : _op(o), _arg(a), _num(n) { }
    template<class T> explicit operator T() const { return _op(static_cast<T>(_arg),_num); }
    template<class... AS> auto operator() (AS... vals) const -> decltype(_op(_arg(vals...),_num)) {
        return _op(_arg(vals...),_num); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << expr._op.code() << "(" << expr._arg << "," << expr._num << ")"; }
};

template<class O, class A1, class A2, class A3> struct Symbolic<O,A1,A2,A3> {
    O _op; A1 _arg1; A2 _arg2; A3 _arg3;
    Symbolic(O o, A1 a1, A2 a2, A3 a3) : _op(o), _arg1(a1), _arg3(a3) { }
    template<class T> explicit operator T() const { return _op(static_cast<T>(_arg1),static_cast<T>(_arg2),static_cast<T>(_arg3)); }
    template<class... AS> auto operator() (AS... vals) const -> decltype(_op(_arg1(vals...),_arg2(vals...),_arg3(vals...))) {
        return _op(_arg1(vals...),_arg2(vals...)); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << expr._op.code() << "(" << expr._arg1 << "," << expr._arg2 << "," << expr._arg3 << ")"; }
};

template<class O, class... Args> Symbolic<O,Args...> make_expression_template(O o, Args... as) {
    return Symbolic<O,Args...>(o, as...); }

template<class... SS> class SymbolicVariant : public Variant<SS...> {
    template<class VIS> decltype(auto) accept(VIS&& vis) const { return std::visit(std::forward<VIS>(vis),static_cast<Variant<SS...>const&>(*this)); }
};

struct While { };
struct Iterate { };
struct IfThnEls { };

template<class C, class A> struct Symbolic<IfThnEls,C,A> {
    IfThnEls _op; C _cnd; A _atru; A _afls;
    Symbolic(IfThnEls op, C cnd, A atru, A afls) : _op(op), _cnd(cnd), _atru(atru), _afls(afls) { }
    template<class... VS> decltype(auto) operator () (VS... vals) const {
        if(_cnd(vals...)) { return  _atr(vals...); } else { return _afls(vals...); } }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << "while(" << expr._cnd << ") { " << expr._fn << "}";
    }
};
template<class C, class F> struct Symbolic<While,C,F> {
    While _op; C _cnd; F _fn;
    Symbolic(While op, C cnd, F fn) : _op(op), _cnd(cnd), _fn(fn) { }
    template<class S> S& operator () (S& s) const {
        while(_cnd(s)) { s=_fn(s); } return s; }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << "while(" << expr._cnd << ") { " << expr._fn << "}";
    }
};

template<class F> struct Symbolic<Iterate,F> {
    F _fn;
    Symbolic(Iterate op, F fn) : _fn(fn) { }
    template<class S> Sequence<S> operator () (S const& s) const {
        // TODO: Use cache so that sequence does not always have to be recomputed
        return Sequence<S>([&](Natural const& n){ S t=s; for(Natural i=0u; i!=n; ++i) { t=_fn(t); } return t; }); }
    friend OutputStream& operator<<(OutputStream& os, Symbolic expr) {
        return os << "iterate(f)" << expr._cnd << ") { " << expr._fn << "}";
    }
};


template<class O, class... Args> struct TemporaryExpression;

template<class O, class A> struct TemporaryExpression<O,A> {
    O _op; A const& _arg;
    TemporaryExpression(O o, A const& a) : _op(o), _arg(a) { }
};

template<class O, class A1, class A2> struct TemporaryExpression<O,A1,A2> {
    O _op; A1 const& _arg1; A2 const& _arg2;
    TemporaryExpression(O o, A1 const& a1, A2 const& a2) : _op(o), _arg1(a1), _arg2(a2) { }
};

template<class O, class A1, class A2, class A3> struct TemporaryExpression<O,A1,A2,A3> {
    O _op; A1 const& _arg1; A2 const& _arg2; A3 const& _arg3;
    TemporaryExpression(O o, A1 const& a1, A2 const& a2, A3 const& a3) : _op(o), _arg1(a1), _arg3(a3) { }
};


} // namespace SymboliCore

#endif
