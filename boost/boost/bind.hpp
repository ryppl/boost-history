#ifndef BOOST_BIND_HPP_INCLUDED
#define BOOST_BIND_HPP_INCLUDED

#if _MSC_VER >= 1020
#pragma once
#endif

//
//  bind.hpp - binds function objects to arguments
//
//  Copyright (c) 2001 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2001 David Abrahams
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org/libs/bind/bind.html for documentation.
//

#include <boost/config.hpp>
#include <boost/ref.hpp>
#include <boost/mem_fn.hpp>

namespace boost
{

namespace _bi // implementation details
{

// result_traits

template<class R, class F> struct result_traits
{
    typedef R type;
};

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)

struct unspecified {};

template<class F> struct result_traits<unspecified, F>
{
    typedef typename F::result_type type;
};

#endif

// bind_t forward declaration for listN

template<class R, class F, class L> class bind_t;

// value

template<class T> class value
{
public:

    value(T const & t): t_(t) {}

    T & get() { return t_; }
    T const & get() const { return t_; }

private:

    T t_;
    value & operator= (value const &);
};

// arg

template<int I> class arg {};

// type

template<class T> class type {};

// listN

#ifdef BOOST_NO_VOID_RETURNS

template <class R> struct evaluator0;
template <class R> struct evaluator1;
template <class R> struct evaluator2;
template <class R> struct evaluator3;
template <class R> struct evaluator4;
template <class R> struct evaluator5;
template <class R> struct evaluator6;
template <class R> struct evaluator7;
template <class R> struct evaluator8;
template <class R> struct evaluator9;

#endif

class list0
{
public:

    list0() {}

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A &) const
    {
        return f();
    }

    template<class V> void accept(V &) const
    {
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator0<R> type;
	};

#endif

private:

    list0 & operator= (list0 const &);
};

template<class A1> class list1
{
public:

    explicit list1(A1 a1): a1_(a1) {}

    A1 operator[] (arg<1>) const { return a1_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator1<R> type;
	};

#else

private:

#endif

    A1 a1_;

private:

    list1 & operator= (list1 const &);
};

template<class A1, class A2> class list2
{
public:

    list2(A1 a1, A2 a2): a1_(a1), a2_(a2) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator2<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;

private:

    list2 & operator= (list2 const &);
};

template<class A1, class A2, class A3> class list3
{
public:

    list3(A1 a1, A2 a2, A3 a3): a1_(a1), a2_(a2), a3_(a3) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator3<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;

private:

    list3 & operator= (list3 const &);
};

template<class A1, class A2, class A3, class A4> class list4
{
public:

    list4(A1 a1, A2 a2, A3 a3, A4 a4): a1_(a1), a2_(a2), a3_(a3), a4_(a4) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator4<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;

private:

    list4 & operator= (list4 const &);
};

template<class A1, class A2, class A3, class A4, class A5> class list5
{
public:

    list5(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5): a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }
    A5 operator[] (arg<5>) const { return a5_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_], a[a5_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
        v(a5_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator5<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;

private:

    list5 & operator= (list5 const &);
};

template<class A1, class A2, class A3, class A4, class A5, class A6> class list6
{
public:

    list6(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6): a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5), a6_(a6) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }
    A5 operator[] (arg<5>) const { return a5_; }
    A6 operator[] (arg<6>) const { return a6_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_], a[a5_], a[a6_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
        v(a5_);
        v(a6_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator6<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;
    A6 a6_;

private:

    list6 & operator= (list6 const &);
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7> class list7
{
public:

    list7(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7): a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5), a6_(a6), a7_(a7) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }
    A5 operator[] (arg<5>) const { return a5_; }
    A6 operator[] (arg<6>) const { return a6_; }
    A7 operator[] (arg<7>) const { return a7_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_], a[a5_], a[a6_], a[a7_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
        v(a5_);
        v(a6_);
        v(a7_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator7<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;
    A6 a6_;
    A7 a7_;

private:

    list7 & operator= (list7 const &);
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> class list8
{
public:

    list8(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8): a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5), a6_(a6), a7_(a7), a8_(a8) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }
    A5 operator[] (arg<5>) const { return a5_; }
    A6 operator[] (arg<6>) const { return a6_; }
    A7 operator[] (arg<7>) const { return a7_; }
    A8 operator[] (arg<8>) const { return a8_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_], a[a5_], a[a6_], a[a7_], a[a8_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
        v(a5_);
        v(a6_);
        v(a7_);
        v(a8_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator8<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;
    A6 a6_;
    A7 a7_;
    A8 a8_;

private:

    list8 & operator= (list8 const &);
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9> class list9
{
public:

    list9(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9): a1_(a1), a2_(a2), a3_(a3), a4_(a4), a5_(a5), a6_(a6), a7_(a7), a8_(a8), a9_(a9) {}

    A1 operator[] (arg<1>) const { return a1_; }
    A2 operator[] (arg<2>) const { return a2_; }
    A3 operator[] (arg<3>) const { return a3_; }
    A4 operator[] (arg<4>) const { return a4_; }
    A5 operator[] (arg<5>) const { return a5_; }
    A6 operator[] (arg<6>) const { return a6_; }
    A7 operator[] (arg<7>) const { return a7_; }
    A8 operator[] (arg<8>) const { return a8_; }
    A9 operator[] (arg<9>) const { return a9_; }

    template<class T> T & operator[] (value<T> & v) const { return v.get(); }

    template<class T> T const & operator[] (value<T> const & v) const { return v.get(); }

    template<class T> T & operator[] (reference_wrapper<T> const & v) const { return v.get(); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> & b) const { return b.eval(*this); }

    template<class R, class F, class L> typename result_traits<R, F>::type operator[] (bind_t<R, F, L> const & b) const { return b.eval(*this); }

    template<class R, class F, class A> R operator()(type<R>, F f, A & a) const
    {
        return f(a[a1_], a[a2_], a[a3_], a[a4_], a[a5_], a[a6_], a[a7_], a[a8_], a[a9_]);
    }

    template<class V> void accept(V & v) const
    {
        v(a1_);
        v(a2_);
        v(a3_);
        v(a4_);
        v(a5_);
        v(a6_);
        v(a7_);
        v(a8_);
        v(a9_);
    }

#ifdef BOOST_NO_VOID_RETURNS

	template<class R> struct evaluator
	{
        typedef evaluator9<R> type;
	};

#else

private:

#endif

    A1 a1_;
    A2 a2_;
    A3 a3_;
    A4 a4_;
    A5 a5_;
    A6 a6_;
    A7 a7_;
    A8 a8_;
    A9 a9_;

private:

    list9 & operator= (list9 const &);
};

#ifdef BOOST_NO_VOID_RETURNS

template <class R> struct evaluator0
{
    template<class L, class F, class A>
    static R eval(L const&, F f, A &)
    {
        return f();
    }
};

template <> struct evaluator0<void>
{
    template<class L, class F, class A>
    static void eval(L const&, F f, A &)
    {
        f();
    }
};

template <class R> struct evaluator1
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_]);
    }
};

template <> struct evaluator1<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_]);
    }
};

template <class R> struct evaluator2
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_]);
    }
};

template <> struct evaluator2<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_]);
    }
};

template <class R> struct evaluator3
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_]);
    }
};

template <> struct evaluator3<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_]);
    }
};

template <class R> struct evaluator4
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_]);
    }
};

template <> struct evaluator4<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_]);
    }
};

template <class R> struct evaluator5
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_]);
    }
};

template <> struct evaluator5<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_]);
    }
};

template <class R> struct evaluator6
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_]);
    }
};

template <> struct evaluator6<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_]);
    }
};

template <class R> struct evaluator7
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_]);
    }
};

template <> struct evaluator7<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_]);
    }
};

template <class R> struct evaluator8
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_], a[l.a8_]);
    }
};

template <> struct evaluator8<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_], a[l.a8_]);
    }
};

template <class R> struct evaluator9
{
    template<class L, class F, class A>
    static R eval(L const& l, F f, A & a)
    {
        return f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_], a[l.a8_], a[l.a9_]);
    }
};

template <> struct evaluator9<void>
{
    template<class L, class F, class A>
    static void eval(L const& l, F f, A & a)
    {
        f(a[l.a1_], a[l.a2_], a[l.a3_], a[l.a4_], a[l.a5_], a[l.a6_], a[l.a7_], a[l.a8_], a[l.a9_]);
    }
};

#endif

// bind_t

#ifndef BOOST_NO_VOID_RETURNS

template<class R, class F, class L> class bind_t
{
public:

    bind_t(F f, L const & l): f_(f), l_(l) {}

#define BOOST_BIND_EVALUATE return l_(type<result_type>(), f_, a)
#include <boost/bind/bind_template.hpp>
#undef BOOST_BIND_EVALUATE

private:

    bind_t & operator= (bind_t const &);
};

#else

template <class R> struct bind_t_generator
{

template<class F, class L> class implementation
{
public:

    implementation(F f, L const & l): f_(f), l_(l) {}

#define BOOST_BIND_EVALUATE return L::BOOST_NESTED_TEMPLATE evaluator<result_type>::type::eval(l_, f_, a);
#include <boost/bind/bind_template.hpp>
#undef BOOST_BIND_EVALUATE

private:

    implementation & operator= (implementation const &);
};

};

template<> struct bind_t_generator<void>
{

template<class F, class L> class implementation
{
public:

    implementation(F f, L const & l): f_(f), l_(l) {}

#define BOOST_BIND_EVALUATE L::BOOST_NESTED_TEMPLATE evaluator<result_type>::type::eval(l_, f_, a);
#include <boost/bind/bind_template.hpp>
#undef BOOST_BIND_EVALUATE

private:

    implementation & operator= (implementation const &);
};

};

#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
#pragma warning(push)
#pragma warning(disable: 4097) // typedef name 'base' used as a synonym for class
#endif

template<class R, class F, class L> class bind_t: public bind_t_generator<R>::BOOST_NESTED_TEMPLATE implementation<F, L>
{
private:

    typedef typename bind_t_generator<R>::template implementation<F, L> base;

public:

    bind_t(F f, L const & l): base(f, l) {}

private:

    bind_t & operator= (bind_t const &);
};

#if defined(BOOST_MSVC) && (BOOST_MSVC < 1300)
#pragma warning(pop)
#endif

#endif

// add_value

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) || (__SUNPRO_CC >= 0x530)

template<class T> struct add_value
{
    typedef value<T> type;
};

template<class T> struct add_value< value<T> >
{
    typedef value<T> type;
};

template<class T> struct add_value< reference_wrapper<T> >
{
    typedef reference_wrapper<T> type;
};

template<int I> struct add_value< arg<I> >
{
    typedef arg<I> type;
};

template<class R, class F, class L> struct add_value< bind_t<R, F, L> >
{
    typedef bind_t<R, F, L> type;
};

#else

template<int I> struct _avt_0;

template<> struct _avt_0<1>
{
    template<class T> struct inner
    {
        typedef T type;
    };
};

template<> struct _avt_0<2>
{
    template<class T> struct inner
    {
        typedef value<T> type;
    };
};

typedef char (&_avt_r1) [1];
typedef char (&_avt_r2) [2];

template<class T> _avt_r1 _avt_f(value<T>);
template<class T> _avt_r1 _avt_f(reference_wrapper<T>);
template<int I> _avt_r1 _avt_f(arg<I>);
template<class R, class F, class L> _avt_r1 _avt_f(bind_t<R, F, L>);

_avt_r2 _avt_f(...);

template<class T> struct add_value
{
    static T t();
    typedef typename _avt_0<sizeof(_avt_f(t()))>::template inner<T>::type type;
};

#endif

// list_av_N

template<class A1> struct list_av_1
{
    typedef typename add_value<A1>::type B1;
    typedef list1<B1> type;
};

template<class A1, class A2> struct list_av_2
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef list2<B1, B2> type;
};

template<class A1, class A2, class A3> struct list_av_3
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef list3<B1, B2, B3> type;
};

template<class A1, class A2, class A3, class A4> struct list_av_4
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef list4<B1, B2, B3, B4> type;
};

template<class A1, class A2, class A3, class A4, class A5> struct list_av_5
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef typename add_value<A5>::type B5;
    typedef list5<B1, B2, B3, B4, B5> type;
};

template<class A1, class A2, class A3, class A4, class A5, class A6> struct list_av_6
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef typename add_value<A5>::type B5;
    typedef typename add_value<A6>::type B6;
    typedef list6<B1, B2, B3, B4, B5, B6> type;
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7> struct list_av_7
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef typename add_value<A5>::type B5;
    typedef typename add_value<A6>::type B6;
    typedef typename add_value<A7>::type B7;
    typedef list7<B1, B2, B3, B4, B5, B6, B7> type;
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8> struct list_av_8
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef typename add_value<A5>::type B5;
    typedef typename add_value<A6>::type B6;
    typedef typename add_value<A7>::type B7;
    typedef typename add_value<A8>::type B8;
    typedef list8<B1, B2, B3, B4, B5, B6, B7, B8> type;
};

template<class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9> struct list_av_9
{
    typedef typename add_value<A1>::type B1;
    typedef typename add_value<A2>::type B2;
    typedef typename add_value<A3>::type B3;
    typedef typename add_value<A4>::type B4;
    typedef typename add_value<A5>::type B5;
    typedef typename add_value<A6>::type B6;
    typedef typename add_value<A7>::type B7;
    typedef typename add_value<A8>::type B8;
    typedef typename add_value<A9>::type B9;
    typedef list9<B1, B2, B3, B4, B5, B6, B7, B8, B9> type;
};

} // namespace _bi

// bind

#ifndef BOOST_BIND
#define BOOST_BIND bind
#endif

// generic function objects

template<class R, class F>
    _bi::bind_t<R, F, _bi::list0>
    BOOST_BIND(F f)
{
    typedef _bi::list0 list_type;
    return _bi::bind_t<R, F, list_type> (f, list_type());
}

template<class R, class F, class A1>
    _bi::bind_t<R, F, typename _bi::list_av_1<A1>::type>
    BOOST_BIND(F f, A1 a1)
{
    typedef typename _bi::list_av_1<A1>::type list_type;
    return _bi::bind_t<R, F, list_type> (f, list_type(a1));
}

template<class R, class F, class A1, class A2>
    _bi::bind_t<R, F, typename _bi::list_av_2<A1, A2>::type>
    BOOST_BIND(F f, A1 a1, A2 a2)
{
    typedef typename _bi::list_av_2<A1, A2>::type list_type;
    return _bi::bind_t<R, F, list_type> (f, list_type(a1, a2));
}

template<class R, class F, class A1, class A2, class A3>
    _bi::bind_t<R, F, typename _bi::list_av_3<A1, A2, A3>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3)
{
    typedef typename _bi::list_av_3<A1, A2, A3>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3));
}

template<class R, class F, class A1, class A2, class A3, class A4>
    _bi::bind_t<R, F, typename _bi::list_av_4<A1, A2, A3, A4>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4)
{
    typedef typename _bi::list_av_4<A1, A2, A3, A4>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4));
}

template<class R, class F, class A1, class A2, class A3, class A4, class A5>
    _bi::bind_t<R, F, typename _bi::list_av_5<A1, A2, A3, A4, A5>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    typedef typename _bi::list_av_5<A1, A2, A3, A4, A5>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4, a5));
}

template<class R, class F, class A1, class A2, class A3, class A4, class A5, class A6>
    _bi::bind_t<R, F, typename _bi::list_av_6<A1, A2, A3, A4, A5, A6>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    typedef typename _bi::list_av_6<A1, A2, A3, A4, A5, A6>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6));
}

template<class R, class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    _bi::bind_t<R, F, typename _bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    typedef typename _bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7));
}

template<class R, class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    _bi::bind_t<R, F, typename _bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    typedef typename _bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7, a8));
}

template<class R, class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    _bi::bind_t<R, F, typename _bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    typedef typename _bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type list_type;
    return _bi::bind_t<R, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)

// adaptable function objects

template<class F>
    _bi::bind_t<_bi::unspecified, F, _bi::list0>
    BOOST_BIND(F f)
{
    typedef _bi::list0 list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type> (f, list_type());
}

template<class F, class A1>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_1<A1>::type>
    BOOST_BIND(F f, A1 a1)
{
    typedef typename _bi::list_av_1<A1>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type> (f, list_type(a1));
}

template<class F, class A1, class A2>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_2<A1, A2>::type>
    BOOST_BIND(F f, A1 a1, A2 a2)
{
    typedef typename _bi::list_av_2<A1, A2>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type> (f, list_type(a1, a2));
}

template<class F, class A1, class A2, class A3>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_3<A1, A2, A3>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3)
{
    typedef typename _bi::list_av_3<A1, A2, A3>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3));
}

template<class F, class A1, class A2, class A3, class A4>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_4<A1, A2, A3, A4>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4)
{
    typedef typename _bi::list_av_4<A1, A2, A3, A4>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4));
}

template<class F, class A1, class A2, class A3, class A4, class A5>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_5<A1, A2, A3, A4, A5>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5)
{
    typedef typename _bi::list_av_5<A1, A2, A3, A4, A5>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4, a5));
}

template<class F, class A1, class A2, class A3, class A4, class A5, class A6>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_6<A1, A2, A3, A4, A5, A6>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6)
{
    typedef typename _bi::list_av_6<A1, A2, A3, A4, A5, A6>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6));
}

template<class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7)
{
    typedef typename _bi::list_av_7<A1, A2, A3, A4, A5, A6, A7>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7));
}

template<class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8)
{
    typedef typename _bi::list_av_8<A1, A2, A3, A4, A5, A6, A7, A8>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7, a8));
}

template<class F, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8, class A9>
    _bi::bind_t<_bi::unspecified, F, typename _bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type>
    BOOST_BIND(F f, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6, A7 a7, A8 a8, A9 a9)
{
    typedef typename _bi::list_av_9<A1, A2, A3, A4, A5, A6, A7, A8, A9>::type list_type;
    return _bi::bind_t<_bi::unspecified, F, list_type>(f, list_type(a1, a2, a3, a4, a5, a6, a7, a8, a9));
}

#endif // !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) && !defined(BOOST_NO_FUNCTION_TEMPLATE_ORDERING)

// function pointers

#define BOOST_BIND_CC
#define BOOST_BIND_ST

#include <boost/bind/bind_cc.hpp>

#undef BOOST_BIND_CC
#undef BOOST_BIND_ST

#ifdef BOOST_BIND_ENABLE_STDCALL

#define BOOST_BIND_CC __stdcall
#define BOOST_BIND_ST

#include <boost/bind/bind_cc.hpp>

#undef BOOST_BIND_CC
#undef BOOST_BIND_ST

#endif

#ifdef BOOST_BIND_ENABLE_PASCAL

#define BOOST_BIND_ST pascal
#define BOOST_BIND_CC

#include <boost/bind/bind_cc.hpp>

#undef BOOST_BIND_ST
#undef BOOST_BIND_CC

#endif

// member function pointers

#define BOOST_BIND_MF_NAME(X) X
#define BOOST_BIND_MF_CC

#include <boost/bind/bind_mf_cc.hpp>

#undef BOOST_BIND_MF_NAME
#undef BOOST_BIND_MF_CC

#ifdef BOOST_MEM_FN_ENABLE_STDCALL

#define BOOST_BIND_MF_NAME(X) X##_stdcall
#define BOOST_BIND_MF_CC __stdcall

#include <boost/bind/bind_mf_cc.hpp>

#undef BOOST_BIND_MF_NAME
#undef BOOST_BIND_MF_CC

#endif

} // namespace boost

namespace
{
    boost::_bi::arg<1> _1;
    boost::_bi::arg<2> _2;
    boost::_bi::arg<3> _3;
    boost::_bi::arg<4> _4;
    boost::_bi::arg<5> _5;
    boost::_bi::arg<6> _6;
    boost::_bi::arg<7> _7;
    boost::_bi::arg<8> _8;
    boost::_bi::arg<9> _9;
}

#endif // #ifndef BOOST_BIND_HPP_INCLUDED
