/***************************************************************************
 *            sequence.hpp
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

/*! \file sequence.hpp
 *  \brief
 */

#ifndef SYMBOLICORE_SEQUENCE_HPP
#define SYMBOLICORE_SEQUENCE_HPP

#include <functional>

namespace SymboliCore {

using Nat = unsigned int;

//! \brief A function \f$\mathbb{N} \to X\f$.
template<class X> class Sequence {
    std::function<X(Nat)> _fn;
  public:
    Sequence(std::function<X(Nat)> const& fn) : _fn(fn) { }
    X operator[](Nat const& n) const { return _fn(n); }
};

//! \brief A convergent sequence in \f$X\f$, with no further information about the convergence rate
template<class X> class ConvergentSequence : public Sequence<X> {
  public:
    ConvergentSequence(std::function<X(Nat)> const& fn) : Sequence<X>(fn) { }
    ConvergentSequence(Sequence<X> const& seq) : Sequence<X>(seq) { }
};

//! An alternating sequence in a partially-ordered space (X,,≤), satisfying \f$x_n \in [x_{n-1},x_{n-2}]\f$.
template<class X> class AlternatingSequence : public ConvergentSequence<X> {
  public:
    explicit AlternatingSequence(Sequence<X> const& seq) : ConvergentSequence<X>(seq) { }
};

//! \brief An increasing sequence in a partially-ordered space (X,≤).
template<class X> class IncreasingSequence : public Sequence<X> {
  public:
    IncreasingSequence(std::function<X(Nat)> fn) : Sequence<X>(fn) { }
    IncreasingSequence(Sequence<X> const& seq) : Sequence<X>(seq) { }
};
//! \brief A decreasing sequence in a partially-ordered space (X,≤).
template<class X> class DecreasingSequence : public Sequence<X> {
  public:
    DecreasingSequence(std::function<X(Nat)> fn) : Sequence<X>(fn) { }
    DecreasingSequence(Sequence<X> const& seq) : Sequence<X>(seq) { }
};

//! \brief A fast-converging Cauchy sequence in a metric space (X,d), satisfying \f$ d(x_m,x_n) \leq 2^{-min(m,n)}\f$.
template<class X> class FastCauchySequence : public Sequence<X> {
  public:
    FastCauchySequence(std::function<X(Nat)> const& fn) : Sequence<X>(fn) { }
    FastCauchySequence(Sequence<X> const& seq) : Sequence<X>(seq) { }
};


using OutputStream = std::ostream;
template<class T, class W> class WritableTemporary;

class SequenceWriter {
    Nat _num;
  public:
    SequenceWriter(Nat const& n) : _num(n) { }
    template<class T> inline WritableTemporary<Sequence<T>,SequenceWriter> operator() (Sequence<T> const& seq) const {
        return WritableTemporary<Sequence<T>,SequenceWriter>(*this,seq); }
    template<class T> OutputStream& _write(OutputStream& os, Sequence<T> const& seq) const {
        os << "["; for (Nat i=0u; i!=_num; ++i) { if (i!=0) { os << ","; } os << seq[i]; } os << "]"; return os; }
};

} // namespace SymboliCore

#endif
