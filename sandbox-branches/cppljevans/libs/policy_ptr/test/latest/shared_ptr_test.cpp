#if defined(_MSC_VER) && !defined(__ICL) && !defined(__COMO__)
#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#pragma warning(disable: 4355)  // 'this' : used in base member initializer list
#endif

//
//  shared_ptr_test.cpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//

#include <boost/detail/lightweight_test.hpp>

#define BOOST_SMART_POINTER_ORDERING_OPERATORS
#include "../smart_ptr.hpp"
#include "../policy/boost.hpp"

#include <map>
#include <vector>

// Hack for VC
namespace boost
{

namespace n_element_type
{

void f(int &)
{
}

void test()
{
    typedef smart_ptr<int, shared_storage<_>, boost_ref<_> >::element_type T;
    T t;
    f(t);
}

} // namespace n_element_type

namespace n_constructors
{

class incomplete;

void default_constructor()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi;
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px;
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }
}

struct A
{
    int dummy;
};

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

// virtual inheritance stresses the implementation

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

void pointer_constructor()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    {
        smart_ptr<int const, shared_storage<_>, boost_ref<_> > pi(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(static_cast<int*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(static_cast<int*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<Y*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(static_cast<Y*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(static_cast<X*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(static_cast<X*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        int * p = new int(7);
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == p);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);
    }

    {
        int * p = new int(7);
        smart_ptr<int const, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == p);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);
    }

    {
        int * p = new int(7);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == p);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        int * p = new int(7);
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == p);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == p);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == p);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);
}

int m = 0;

void deleter(int * p)
{
    BOOST_TEST(p == 0);
}

void deleter2(int * p)
{
    BOOST_TEST(p == &m);
    ++*p;
}

struct deleter3
{
    void operator()(incomplete * p)
    {
        BOOST_TEST(p == 0);
    }
};

// Borland C++ 5.5.1 fails on static_cast<incomplete*>(0)

incomplete * p0 = 0;

void deleter_constructor()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(static_cast<int*>(0), deleter);
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(static_cast<int*>(0), &deleter);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(static_cast<int*>(0), deleter);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px(p0, deleter3());
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p0, deleter3());
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    {
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p0, deleter3());
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    BOOST_TEST(m == 0);

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(&m, deleter2);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == &m);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    BOOST_TEST(m == 1);

    {
        smart_ptr<int const, shared_storage<_>, boost_ref<_> > pi(&m, &deleter2);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == &m);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    BOOST_TEST(m == 2);

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(&m, deleter2);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == &m);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    BOOST_TEST(m == 3);

    {
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(&m, &deleter2);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == &m);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
    }

    BOOST_TEST(m == 4);
}

void copy_constructor()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi;

        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi2(pi);
        BOOST_TEST(pi2 == pi);
        BOOST_TEST(pi2? false: true);
        BOOST_TEST(!pi2);
        BOOST_TEST(pi2.get() == 0);
        BOOST_TEST(pi2.use_count() == pi.use_count());

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pi3(pi);
        BOOST_TEST(pi3 == pi);
        BOOST_TEST(pi3? false: true);
        BOOST_TEST(!pi3);
        BOOST_TEST(pi3.get() == 0);
        BOOST_TEST(pi3.use_count() == pi.use_count());

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pi4(pi3);
        BOOST_TEST(pi4 == pi3);
        BOOST_TEST(pi4? false: true);
        BOOST_TEST(!pi4);
        BOOST_TEST(pi4.get() == 0);
        BOOST_TEST(pi4.use_count() == pi3.use_count());
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv2(pv);
        BOOST_TEST(pv2 == pv);
        BOOST_TEST(pv2? false: true);
        BOOST_TEST(!pv2);
        BOOST_TEST(pv2.get() == 0);
        BOOST_TEST(pv2.use_count() == pv.use_count());
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px;

        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? false: true);
        BOOST_TEST(!px2);
        BOOST_TEST(px2.get() == 0);
        BOOST_TEST(px2.use_count() == px.use_count());

        smart_ptr<void, shared_storage<_>, boost_ref<_> > px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? false: true);
        BOOST_TEST(!px3);
        BOOST_TEST(px3.get() == 0);
        BOOST_TEST(px3.use_count() == px.use_count());
    }

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(static_cast<int*>(0));

        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi2(pi);
        BOOST_TEST(pi2 == pi);
        BOOST_TEST(pi2? false: true);
        BOOST_TEST(!pi2);
        BOOST_TEST(pi2.get() == 0);
        BOOST_TEST(pi2.use_count() == 2);
        BOOST_TEST(!pi2.unique());
        BOOST_TEST(pi2.use_count() == pi.use_count());
        BOOST_TEST(!(pi < pi2 || pi2 < pi)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pi3(pi);
        BOOST_TEST(pi3 == pi);
        BOOST_TEST(pi3? false: true);
        BOOST_TEST(!pi3);
        BOOST_TEST(pi3.get() == 0);
        BOOST_TEST(pi3.use_count() == 3);
        BOOST_TEST(!pi3.unique());
        BOOST_TEST(pi3.use_count() == pi.use_count());
        BOOST_TEST(!(pi < pi3 || pi3 < pi)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pi4(pi2);
        BOOST_TEST(pi4 == pi2);
        BOOST_TEST(pi4? false: true);
        BOOST_TEST(!pi4);
        BOOST_TEST(pi4.get() == 0);
        BOOST_TEST(pi4.use_count() == 4);
        BOOST_TEST(!pi4.unique());
        BOOST_TEST(pi4.use_count() == pi2.use_count());
        BOOST_TEST(!(pi2 < pi4 || pi4 < pi2)); // shared ownership test

        BOOST_TEST(pi3.use_count() == pi4.use_count());
        BOOST_TEST(!(pi3 < pi4 || pi4 < pi3)); // shared ownership test
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0));

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? false: true);
        BOOST_TEST(!px2);
        BOOST_TEST(px2.get() == 0);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px2.use_count() == px.use_count());
        BOOST_TEST(!(px < px2 || px2 < px)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? false: true);
        BOOST_TEST(!px3);
        BOOST_TEST(px3.get() == 0);
        BOOST_TEST(px3.use_count() == 3);
        BOOST_TEST(!px3.unique());
        BOOST_TEST(px3.use_count() == px.use_count());
        BOOST_TEST(!(px < px3 || px3 < px)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > px4(px2);
        BOOST_TEST(px4 == px2);
        BOOST_TEST(px4? false: true);
        BOOST_TEST(!px4);
        BOOST_TEST(px4.get() == 0);
        BOOST_TEST(px4.use_count() == 4);
        BOOST_TEST(!px4.unique());
        BOOST_TEST(px4.use_count() == px2.use_count());
        BOOST_TEST(!(px2 < px4 || px4 < px2)); // shared ownership test

        BOOST_TEST(px3.use_count() == px4.use_count());
        BOOST_TEST(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    {
        int * p = new int(7);
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(p);

        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi2(pi);
        BOOST_TEST(pi2 == pi);
        BOOST_TEST(pi2? true: false);
        BOOST_TEST(!!pi2);
        BOOST_TEST(pi2.get() == p);
        BOOST_TEST(pi2.use_count() == 2);
        BOOST_TEST(!pi2.unique());
        BOOST_TEST(*pi2 == 7);
        BOOST_TEST(pi2.use_count() == pi.use_count());
        BOOST_TEST(!(pi < pi2 || pi2 < pi)); // shared ownership test
    }

    {
        int * p = new int(7);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv.get() == p);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv2(pv);
        BOOST_TEST(pv2 == pv);
        BOOST_TEST(pv2? true: false);
        BOOST_TEST(!!pv2);
        BOOST_TEST(pv2.get() == p);
        BOOST_TEST(pv2.use_count() == 2);
        BOOST_TEST(!pv2.unique());
        BOOST_TEST(pv2.use_count() == pv.use_count());
        BOOST_TEST(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px.get() == p);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? true: false);
        BOOST_TEST(!!px2);
        BOOST_TEST(px2.get() == p);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());

        BOOST_TEST(X::instances == 1);

        BOOST_TEST(px2.use_count() == px.use_count());
        BOOST_TEST(!(px < px2 || px2 < px)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? true: false);
        BOOST_TEST(!!px3);
        BOOST_TEST(px3.get() == p);
        BOOST_TEST(px3.use_count() == 3);
        BOOST_TEST(!px3.unique());
        BOOST_TEST(px3.use_count() == px.use_count());
        BOOST_TEST(!(px < px3 || px3 < px)); // shared ownership test

        smart_ptr<void, shared_storage<_>, boost_ref<_> > px4(px2);
        BOOST_TEST(px4 == px2);
        BOOST_TEST(px4? true: false);
        BOOST_TEST(!!px4);
        BOOST_TEST(px4.get() == p);
        BOOST_TEST(px4.use_count() == 4);
        BOOST_TEST(!px4.unique());
        BOOST_TEST(px4.use_count() == px2.use_count());
        BOOST_TEST(!(px2 < px4 || px4 < px2)); // shared ownership test

        BOOST_TEST(px3.use_count() == px4.use_count());
        BOOST_TEST(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        smart_ptr<Y, shared_storage<_>, boost_ref<_> > py(p);
        BOOST_TEST(py.get() == p);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(py);
        BOOST_TEST(px == py);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
        BOOST_TEST(px.use_count() == py.use_count());
        BOOST_TEST(!(px < py || py < px)); // shared ownership test

        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(px);
        BOOST_TEST(pv == px);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == px.get());
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(!pv.unique());
        BOOST_TEST(pv.use_count() == px.use_count());
        BOOST_TEST(!(px < pv || pv < px)); // shared ownership test

        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv2(py);
        BOOST_TEST(pv2 == py);
        BOOST_TEST(pv2? true: false);
        BOOST_TEST(!!pv2);
        BOOST_TEST(pv2.get() == py.get());
        BOOST_TEST(pv2.use_count() == 4);
        BOOST_TEST(!pv2.unique());
        BOOST_TEST(pv2.use_count() == py.use_count());
        BOOST_TEST(!(py < pv2 || pv2 < py)); // shared ownership test

        BOOST_TEST(pv.use_count() == pv2.use_count());
        BOOST_TEST(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);
}

void weak_ptr_constructor()
{
    {
        smart_ptr<Y, weak_storage<_>, boost_ref<_> > wp;
        BOOST_TEST(wp.use_count() == 0);

        try
        {
            smart_ptr<Y, shared_storage<_>, boost_ref<_> > p2(wp);
            BOOST_ERROR("shared_ptr<Y> p2(wp) failed to throw");
        }
        catch(bad_weak_ptr)
        {
        }

        try
        {
            smart_ptr<X, shared_storage<_>, boost_ref<_> > p3(wp);
            BOOST_ERROR("shared_ptr<X> p3(wp) failed to throw");
        }
        catch(bad_weak_ptr)
        {
        }
    }

    {
        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p;
        smart_ptr<Y, weak_storage<_>, boost_ref<_> > wp(p);

        if(wp.use_count() != 0) // 0 allowed but not required
        {
            smart_ptr<Y, shared_storage<_>, boost_ref<_> > p2(wp);
            BOOST_TEST(p2.use_count() == wp.use_count());
            BOOST_TEST(p2.get() == 0);

            smart_ptr<X, shared_storage<_>, boost_ref<_> > p3(wp);
            BOOST_TEST(p3.use_count() == wp.use_count());
            BOOST_TEST(p3.get() == 0);
        }
    }

    {
        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p(new Y);
        smart_ptr<Y, weak_storage<_>, boost_ref<_> > wp(p);

        {
            smart_ptr<Y, shared_storage<_>, boost_ref<_> > p2(wp);
            BOOST_TEST(p2? true: false);
            BOOST_TEST(!!p2);
            BOOST_TEST(p2.get() == p.get());
            BOOST_TEST(p2.use_count() == 2);
            BOOST_TEST(!p2.unique());
            BOOST_TEST(p2.use_count() == wp.use_count());

            BOOST_TEST(p.use_count() == p2.use_count());
            BOOST_TEST(!(p < p2 || p2 < p)); // shared ownership test

            smart_ptr<X, shared_storage<_>, boost_ref<_> > p3(wp);
            BOOST_TEST(p3? true: false);
            BOOST_TEST(!!p3);
            BOOST_TEST(p3.get() == p.get());
            BOOST_TEST(p3.use_count() == 3);
            BOOST_TEST(!p3.unique());
            BOOST_TEST(p3.use_count() == wp.use_count());

            BOOST_TEST(p.use_count() == p3.use_count());
        }

        p.reset();
        BOOST_TEST(wp.use_count() == 0);

        try
        {
            smart_ptr<Y, shared_storage<_>, boost_ref<_> > p2(wp);
            BOOST_ERROR("shared_ptr<Y> p2(wp) failed to throw");
        }
        catch(bad_weak_ptr)
        {
        }

        try
        {
            smart_ptr<X, shared_storage<_>, boost_ref<_> > p3(wp);
            BOOST_ERROR("shared_ptr<X> p3(wp) failed to throw");
        }
        catch(bad_weak_ptr)
        {
        }
    }
}

void auto_ptr_constructor()
{
    {
        std::auto_ptr<int> p;
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        smart_ptr<int const, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<int> p;
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<X> p;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<X> p;
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<Y> p;
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(p.get() == 0);
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == q);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        smart_ptr<int const, shared_storage<_>, boost_ref<_> > pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == q);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == q);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    {
        std::auto_ptr<int> p(new int(7));
        int * q = p.get();
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == q);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == q);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == q);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == q);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);

    {
        std::auto_ptr<X> p(new X);
        X * q = p.get();
        smart_ptr<void const, shared_storage<_>, boost_ref<_> > pv(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == q);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        std::auto_ptr<Y> p(new Y);
        Y * q = p.get();
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == q);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        std::auto_ptr<Y> p(new Y);
        Y * q = p.get();
        smart_ptr<X const, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == q);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p.get() == 0);
#endif
    }

    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);
}

void test()
{
    default_constructor();
    pointer_constructor();
    deleter_constructor();
    copy_constructor();
    weak_ptr_constructor();
    auto_ptr_constructor();
}

} // namespace n_constructors

namespace n_assignment
{

class incomplete;

struct A
{
    int dummy;
};

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

void copy_assignment()
{
    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > p4(new int);
        BOOST_TEST(p4.use_count() == 1);

        p1 = p4;

        BOOST_TEST(p1 == p4);
        BOOST_TEST(!(p1 < p4 || p4 < p1));
        BOOST_TEST(p1.use_count() == 2);
        BOOST_TEST(p4.use_count() == 2);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p4.use_count() == 1);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        BOOST_TEST(X::instances == 0);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > p4(new X);

        BOOST_TEST(X::instances == 1);

        p1 = p4;

        BOOST_TEST(X::instances == 1);

        BOOST_TEST(p1 == p4);
        BOOST_TEST(!(p1 < p4 || p4 < p1));

        BOOST_TEST(p1.use_count() == 2);

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(X::instances == 1);

        p4 = p3;

        BOOST_TEST(p4 == p3);
        BOOST_TEST(X::instances == 0);
    }
}

void conversion_assignment()
{
    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > p1;

        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        smart_ptr<int, shared_storage<_>, boost_ref<_> > p4(new int);
        BOOST_TEST(p4.use_count() == 1);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > p5(p4);
        BOOST_TEST(p4.use_count() == 2);

        p1 = p4;

        BOOST_TEST(p1 == p4);
        BOOST_TEST(!(p1 < p5 || p5 < p1));
        BOOST_TEST(p1.use_count() == 3);
        BOOST_TEST(p4.use_count() == 3);

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p4.use_count() == 2);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > p1;

        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);

        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p4(new Y);

        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);
        BOOST_TEST(p4.use_count() == 1);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > p5(p4);
        BOOST_TEST(p4.use_count() == 2);

        p1 = p4;

        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

        BOOST_TEST(p1 == p4);
        BOOST_TEST(!(p1 < p5 || p5 < p1));

        BOOST_TEST(p1.use_count() == 3);
        BOOST_TEST(p4.use_count() == 3);

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);
        BOOST_TEST(p4.use_count() == 2);

        p4 = p2;
        p5 = p2;

        BOOST_TEST(p4 == p2);
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);
    }
}

void auto_ptr_assignment()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > p1;

        std::auto_ptr<int> p2;

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);

        int * p = new int;
        std::auto_ptr<int> p3(p);

        p1 = p3;
        BOOST_TEST(p1.get() == p);
        BOOST_TEST(p1.use_count() == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p3.get() == 0);
#endif

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > p1;

        std::auto_ptr<int> p2;

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);

        int * p = new int;
        std::auto_ptr<int> p3(p);

        p1 = p3;
        BOOST_TEST(p1.get() == p);
        BOOST_TEST(p1.use_count() == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p3.get() == 0);
#endif

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);
    }


    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > p1;

        std::auto_ptr<Y> p2;

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);

        Y * p = new Y;
        std::auto_ptr<Y> p3(p);

        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

        p1 = p3;
        BOOST_TEST(p1.get() == p);
        BOOST_TEST(p1.use_count() == 1);
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1300)
        BOOST_TEST(p3.get() == 0);
#endif

        p1 = p2;
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
        BOOST_TEST(p1.use_count() == 1);
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);
    }
}

void test()
{
    copy_assignment();
    conversion_assignment();
    auto_ptr_assignment();
}

} // namespace n_assignment

namespace n_reset
{

class incomplete;

incomplete * p0 = 0;

void deleter(incomplete *)
{
}

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

void plain_reset()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi;
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(static_cast<int*>(0));
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(new int);
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px;
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px(p0, deleter);
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }

    {
        BOOST_TEST(X::instances == 0);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        BOOST_TEST(X::instances == 1);
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(X::instances == 0);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;
        pv.reset();
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
    }

    {
        BOOST_TEST(X::instances == 0);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(new X);
        BOOST_TEST(X::instances == 1);
        pv.reset();
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(X::instances == 0);
    }
}

struct A
{
    int dummy;
};

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

void pointer_reset()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi;

        pi.reset(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());

        int * p = new int;
        pi.reset(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == p);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());

        pi.reset(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;

        px.reset(static_cast<X*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 0);

        X * p = new X;
        px.reset(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);

        px.reset(static_cast<X*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);

        Y * q = new Y;
        px.reset(q);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == q);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

        px.reset(static_cast<Y*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;

        pv.reset(static_cast<X*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 0);

        X * p = new X;
        pv.reset(p);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == p);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);

        pv.reset(static_cast<X*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);

        Y * q = new Y;
        pv.reset(q);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == q);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);

        pv.reset(static_cast<Y*>(0));
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());
        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);
    }
}

void * deleted = 0;

void deleter2(void * p)
{
    deleted = p;
}

void deleter_reset()
{
    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi;

        pi.reset(static_cast<int*>(0), deleter2);
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());

        deleted = &pi;

        int m = 0;
        pi.reset(&m, deleter2);
        BOOST_TEST(deleted == 0);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == &m);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());

        pi.reset(static_cast<int*>(0), deleter2);
        BOOST_TEST(deleted == &m);
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());

        pi.reset();
        BOOST_TEST(deleted == 0);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;

        px.reset(static_cast<X*>(0), deleter2);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        deleted = &px;

        X x;
        px.reset(&x, deleter2);
        BOOST_TEST(deleted == 0);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == &x);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        px.reset(static_cast<X*>(0), deleter2);
        BOOST_TEST(deleted == &x);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        Y y;
        px.reset(&y, deleter2);
        BOOST_TEST(deleted == 0);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == &y);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        px.reset(static_cast<Y*>(0), deleter2);
        BOOST_TEST(deleted == &y);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        px.reset();
        BOOST_TEST(deleted == 0);
    }

    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;

        pv.reset(static_cast<X*>(0), deleter2);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

        deleted = &pv;

        X x;
        pv.reset(&x, deleter2);
        BOOST_TEST(deleted == 0);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == &x);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

        pv.reset(static_cast<X*>(0), deleter2);
        BOOST_TEST(deleted == &x);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

        Y y;
        pv.reset(&y, deleter2);
        BOOST_TEST(deleted == 0);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == &y);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

        pv.reset(static_cast<Y*>(0), deleter2);
        BOOST_TEST(deleted == &y);
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
        BOOST_TEST(pv.use_count() == 1);
        BOOST_TEST(pv.unique());

        pv.reset();
        BOOST_TEST(deleted == 0);
    }

    {
        smart_ptr<incomplete, shared_storage<_>, boost_ref<_> > px;

        px.reset(p0, deleter2);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        deleted = &px;
        px.reset(p0, deleter2);
        BOOST_TEST(deleted == 0);
    }
}

void test()
{
    plain_reset();
    pointer_reset();
    deleter_reset();
}

} // namespace n_reset

namespace n_access
{

struct X
{
};

void test()
{
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0));
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0), checked_deleter<X>());
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(&*px == px.get());
        BOOST_TEST(px.operator ->() == px.get());

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p, checked_deleter<X>());
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(&*px == px.get());
        BOOST_TEST(px.operator ->() == px.get());

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }
}

} // namespace n_access

namespace n_use_count
{

struct X
{
};

void test()
{
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(static_cast<X*>(0));
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X, checked_deleter<X>());
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
    }
}

} // namespace n_use_count

namespace n_swap
{

struct X
{
};

void test()
{
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2;

        px.swap(px2);

        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px2.get() == 0);

        using std::swap;
        swap(px, px2);

        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px2.get() == 0);
    }

    {
        X * p = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(p);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px3(px2);

        px.swap(px2);

        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(px2.get() == 0);
        BOOST_TEST(px3.get() == p);
        BOOST_TEST(px3.use_count() == 2);

        using std::swap;
        swap(px, px2);

        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px2.get() == p);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(px3.get() == p);
        BOOST_TEST(px3.use_count() == 2);
    }

    {
        X * p1 = new X;
        X * p2 = new X;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p1);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(p2);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px3(px2);

        px.swap(px2);

        BOOST_TEST(px.get() == p2);
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(px2.get() == p1);
        BOOST_TEST(px2.use_count() == 1);
        BOOST_TEST(px3.get() == p2);
        BOOST_TEST(px3.use_count() == 2);

        using std::swap;
        swap(px, px2);

        BOOST_TEST(px.get() == p1);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px2.get() == p2);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(px3.get() == p2);
        BOOST_TEST(px3.use_count() == 2);
    }
}

} // namespace n_swap

namespace n_comparison
{

struct X
{
    int dummy;
};

struct Y
{
    int dummy2;
};

struct Z: public X, public virtual Y
{
};

void test()
{
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        BOOST_TEST(px == px);
        BOOST_TEST(!(px != px));
        BOOST_TEST(!(px < px));

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2;

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);

        BOOST_TEST(px2 == px2);
        BOOST_TEST(!(px2 != px2));
        BOOST_TEST(!(px2 < px2));

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px;
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(new X);

        BOOST_TEST(px2 == px2);
        BOOST_TEST(!(px2 != px2));
        BOOST_TEST(!(px2 < px2));

        BOOST_TEST(px.get() != px2.get());
        BOOST_TEST(px != px2);
        BOOST_TEST(!(px == px2));
        BOOST_TEST(px < px2 || px2 < px);
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(new X);

        BOOST_TEST(px.get() != px2.get());
        BOOST_TEST(px != px2);
        BOOST_TEST(!(px == px2));
        BOOST_TEST(px < px2 || px2 < px);
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);

        BOOST_TEST(px2 == px2);
        BOOST_TEST(!(px2 != px2));
        BOOST_TEST(!(px2 < px2));

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 || px2 < px));
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        smart_ptr<Y, shared_storage<_>, boost_ref<_> > py(new Y);
        smart_ptr<Z, shared_storage<_>, boost_ref<_> > pz(new Z);

        BOOST_TEST(px.get() != pz.get());
        BOOST_TEST(px != pz);
        BOOST_TEST(!(px == pz));

        BOOST_TEST(py.get() != pz.get());
        BOOST_TEST(py != pz);
        BOOST_TEST(!(py == pz));

        BOOST_TEST(px < py || py < px);
        BOOST_TEST(px < pz || pz < px);
        BOOST_TEST(py < pz || pz < py);

        BOOST_TEST(!(px < py && py < px));
        BOOST_TEST(!(px < pz && pz < px));
        BOOST_TEST(!(py < pz && pz < py));

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvx(px);

        BOOST_TEST(pvx == pvx);
        BOOST_TEST(!(pvx != pvx));
        BOOST_TEST(!(pvx < pvx));

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvy(py);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvz(pz);

        BOOST_TEST(pvx < pvy || pvy < pvx);
        BOOST_TEST(pvx < pvz || pvz < pvx);
        BOOST_TEST(pvy < pvz || pvz < pvy);

        BOOST_TEST(!(pvx < pvy && pvy < pvx));
        BOOST_TEST(!(pvx < pvz && pvz < pvx));
        BOOST_TEST(!(pvy < pvz && pvz < pvy));
    }

    {
        smart_ptr<Z, shared_storage<_>, boost_ref<_> > pz(new Z);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(pz);

        BOOST_TEST(px == px);
        BOOST_TEST(!(px != px));
        BOOST_TEST(!(px < px));

        smart_ptr<Y, shared_storage<_>, boost_ref<_> > py(pz);

        BOOST_TEST(px.get() == pz.get());
        BOOST_TEST(px == pz);
        BOOST_TEST(!(px != pz));

        BOOST_TEST(py.get() == pz.get());
        BOOST_TEST(py == pz);
        BOOST_TEST(!(py != pz));

        BOOST_TEST(!(px < py || py < px));
        BOOST_TEST(!(px < pz || pz < px));
        BOOST_TEST(!(py < pz || pz < py));

        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvx(px);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvy(py);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pvz(pz);

        // pvx and pvy aren't equal...
        BOOST_TEST(pvx.get() != pvy.get());
        BOOST_TEST(pvx != pvy);
        BOOST_TEST(!(pvx == pvy));

        // ... but they share ownership ...
        BOOST_TEST(!(pvx < pvy || pvy < pvx));

        // ... with pvz
        BOOST_TEST(!(pvx < pvz || pvz < pvx));
        BOOST_TEST(!(pvy < pvz || pvz < pvy));
    }
}

} // namespace n_comparison

namespace n_static_cast
{

struct X
{
};

struct Y: public X
{
};

void test()
{
    {
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv;

        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi = static_pointer_cast<int>(pv);
        BOOST_TEST(pi.get() == 0);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px = static_pointer_cast<X>(pv);
        BOOST_TEST(px.get() == 0);
    }

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi(new int);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(pi);

        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi2 = static_pointer_cast<int>(pv);
        BOOST_TEST(pi.get() == pi2.get());
        BOOST_TEST(!(pi < pi2 || pi2 < pi));
        BOOST_TEST(pi.use_count() == 3);
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(pi2.use_count() == 3);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X);
        smart_ptr<void, shared_storage<_>, boost_ref<_> > pv(px);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2 = static_pointer_cast<X>(pv);
        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(!(px < px2 || px2 < px));
        BOOST_TEST(px.use_count() == 3);
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(px2.use_count() == 3);
    }

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new Y);

        smart_ptr<Y, shared_storage<_>, boost_ref<_> > py = static_pointer_cast<Y>(px);
        BOOST_TEST(px.get() == py.get());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(py.use_count() == 2);

        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(py);
        BOOST_TEST(!(px < px2 || px2 < px));
    }
}

} // namespace n_static_cast

namespace n_dynamic_cast
{

struct V
{
    virtual ~V() {}
};

struct W: public V
{
};

void test()
{
    {
        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv;
        smart_ptr<W, shared_storage<_>, boost_ref<_> > pw = dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);
    }

    {
        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv(static_cast<V*>(0));

        smart_ptr<W, shared_storage<_>, boost_ref<_> > pw = dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv(static_cast<W*>(0));

        smart_ptr<W, shared_storage<_>, boost_ref<_> > pw = dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv(new V);

        smart_ptr<W, shared_storage<_>, boost_ref<_> > pw = dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv(new W);

        smart_ptr<W, shared_storage<_>, boost_ref<_> > pw = dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == pv.get());
        BOOST_TEST(pv.use_count() == 2);
        BOOST_TEST(pw.use_count() == 2);

        smart_ptr<V, shared_storage<_>, boost_ref<_> > pv2(pw);
        BOOST_TEST(!(pv < pv2 || pv2 < pv));
    }
}

} // namespace n_dynamic_cast

namespace n_map
{

struct X
{
};

void test()
{
    std::vector< smart_ptr<int, shared_storage<_>, boost_ref<_> > > vi;

    {
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi1(new int);
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi2(new int);
        smart_ptr<int, shared_storage<_>, boost_ref<_> > pi3(new int);

        vi.push_back(pi1);
        vi.push_back(pi1);
        vi.push_back(pi1);
        vi.push_back(pi2);
        vi.push_back(pi1);
        vi.push_back(pi2);
        vi.push_back(pi1);
        vi.push_back(pi3);
        vi.push_back(pi3);
        vi.push_back(pi2);
        vi.push_back(pi1);
    }

    std::vector< smart_ptr<X, shared_storage<_>, boost_ref<_> > > vx;

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px1(new X);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(new X);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px3(new X);

        vx.push_back(px2);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px1);
        vx.push_back(px1);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px3);
        vx.push_back(px2);
    }

    std::map< smart_ptr<void, shared_storage<_>, boost_ref<_> >, long > m;

    {
        for(std::vector< smart_ptr<int, shared_storage<_>, boost_ref<_> > >::iterator i = vi.begin(); i != vi.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::vector< smart_ptr<X, shared_storage<_>, boost_ref<_> > >::iterator i = vx.begin(); i != vx.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::map< smart_ptr<void, shared_storage<_>, boost_ref<_> >, long >::iterator i = m.begin(); i != m.end(); ++i)
        {
            BOOST_TEST(i->first.use_count() == i->second + 1);
        }
    }
}

} // namespace n_map

namespace n_transitive
{

struct X
{
    X(): next() {}
    smart_ptr<X, shared_storage<_>, boost_ref<_> > next;
};

void test()
{
    smart_ptr<X, shared_storage<_>, boost_ref<_> > p(new X);
    p->next = smart_ptr<X, shared_storage<_>, boost_ref<_> >(new X);
    BOOST_TEST(!p->next->next);
    p = p->next;
    BOOST_TEST(!p->next);
}

} // namespace n_transitive

namespace n_report_1
{

class foo
{ 
public: 

    foo(): m_self(this)
    {
    } 

    void suicide()
    {
        m_self.reset();
    }

private:

    smart_ptr<foo, shared_storage<_>, boost_ref<_> > m_self;
}; 

void test()
{
    foo * foo_ptr = new foo;
    foo_ptr->suicide();
} 

} // namespace n_report_1

// Test case by Per Kristensen
namespace n_report_2
{

class foo
{
public:

    void setWeak(smart_ptr<foo, shared_storage<_>, boost_ref<_> > s)
    {
        w = s;
    }

private:

    smart_ptr<foo, weak_storage<_>, boost_ref<_> > w;
};
 
class deleter
{
public:

    deleter(): lock(0)
    {
    }

    ~deleter()
    {
        BOOST_TEST(lock == 0);
    }

    void operator() (foo * p)
    {
        ++lock;
        delete p;
        --lock;
    }
 
private:

    int lock;
};
 
void test()
{
    smart_ptr<foo, shared_storage<_>, boost_ref<_> > s(new foo, deleter());
    s->setWeak(s);
    s.reset();
}

} // namespace n_report_2

namespace n_spt_incomplete
{

class file;

smart_ptr<file, shared_storage<_>, boost_ref<_> > fopen(char const * name, char const * mode);
void fread(smart_ptr<file, shared_storage<_>, boost_ref<_> > f, void * data, long size);

int file_instances = 0;

void test()
{
    BOOST_TEST(file_instances == 0);

    {
        smart_ptr<file, shared_storage<_>, boost_ref<_> > pf = fopen("name", "mode");
        BOOST_TEST(file_instances == 1);
        fread(pf, 0, 17041);
    }

    BOOST_TEST(file_instances == 0);
}

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

class file
{
private:

    class impl;
    smart_ptr<impl, shared_storage<_>, boost_ref<_> > pimpl_;

public:

    file(char const * name, char const * mode);

    // compiler generated members are fine and useful

    void read(void * data, long size);

    long total_size() const;
};

int file_instances = 0;

void test()
{
    BOOST_TEST(file_instances == 0);

    {
        file f("name", "mode");
        BOOST_TEST(file_instances == 1);
        f.read(0, 152);

        file f2(f);
        BOOST_TEST(file_instances == 1);
        f2.read(0, 894);

        BOOST_TEST(f.total_size() == 152+894);

        {
            file f3("name2", "mode2");
            BOOST_TEST(file_instances == 2);
        }

        BOOST_TEST(file_instances == 1);
    }

    BOOST_TEST(file_instances == 0);
}

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

class X
{
public:

    virtual void f(int) = 0;
    virtual int g() = 0;

protected:

    ~X() {}
};

smart_ptr<X, shared_storage<_>, boost_ref<_> > createX();

int X_instances = 0;

void test()
{
    BOOST_TEST(X_instances == 0);

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px = createX();

        BOOST_TEST(X_instances == 1);

        px->f(18);
        px->f(152);

        BOOST_TEST(px->g() == 170);
    }

    BOOST_TEST(X_instances == 0);
}

} // namespace n_spt_abstract

namespace n_spt_preventing_delete
{

int X_instances = 0;

class X
{
private:

    X()
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }

    class deleter;
    friend class deleter;

    class deleter
    {
    public:

        void operator()(X * p) { delete p; }
    };

public:

    static smart_ptr<X, shared_storage<_>, boost_ref<_> > create()
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X, X::deleter());
        return px;
    }
};

void test()
{
    BOOST_TEST(X_instances == 0);

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px = X::create();
        BOOST_TEST(X_instances == 1);
    }

    BOOST_TEST(X_instances == 0);
}

} // namespace n_spt_preventing_delete

namespace n_spt_array
{

int X_instances = 0;

struct X
{
    X()
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }
};

void test()
{
    BOOST_TEST(X_instances == 0);

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X[4], checked_array_deleter<X>());
        BOOST_TEST(X_instances == 4);
    }

    BOOST_TEST(X_instances == 0);
}

} // namespace n_spt_array

namespace n_spt_static
{

class X
{
public:

    X()
    {
    }

private:

    void operator delete(void *)
    {
        // Comeau 4.3.0.1 wants a definition
        BOOST_ERROR("n_spt_static::X::operator delete() called.");
    }
};

struct null_deleter
{
    void operator()(void const *) const
    {
    }
};

static X x;

void test()
{
    smart_ptr<X, shared_storage<_>, boost_ref<_> > px(&x, null_deleter());
}

} // namespace n_spt_static

namespace n_spt_intrusive
{

int X_instances = 0;

struct X
{
    long count;

    X(): count(0)
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }
};

void intrusive_ptr_add_ref(X * p)
{
    ++p->count;
}

void intrusive_ptr_release(X * p)
{
    if(--p->count == 0) delete p;
}

template<class T> struct intrusive_deleter
{
    void operator()(T * p)
    {
        if(p != 0) intrusive_ptr_release(p);
    }
};

smart_ptr<X, shared_storage<_>, boost_ref<_> > make_shared_from_intrusive(X * p)
{
    if(p != 0) intrusive_ptr_add_ref(p);
    smart_ptr<X, shared_storage<_>, boost_ref<_> > px(p, intrusive_deleter<X>());
    return px;
}

void test()
{
    BOOST_TEST(X_instances == 0);

    {
        X * p = new X;
        BOOST_TEST(X_instances == 1);
        BOOST_TEST(p->count == 0);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px = make_shared_from_intrusive(p);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(p->count == 1);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px2(px);
        BOOST_TEST(px2.get() == p);
        BOOST_TEST(p->count == 1);
    }

    BOOST_TEST(X_instances == 0);
}

} // namespace n_spt_intrusive

namespace n_spt_another_sp
{

template<class T> class another_ptr: private smart_ptr<T, shared_storage<_>, boost_ref<_> >
{
private:

    typedef smart_ptr<T, shared_storage<_>, boost_ref<_> > base_type;

public:

    explicit another_ptr(T * p = 0): base_type(p)
    {
    }

    void reset()
    {
        base_type::reset();
    }

    T * get() const
    {
        return base_type::get();
    }
};

class event_handler
{
public:

    virtual ~event_handler() {}
    virtual void begin() = 0;
    virtual void handle(int event) = 0;
    virtual void end() = 0;
};

int begin_called = 0;
int handle_called = 0;
int end_called = 0;

class event_handler_impl: public event_handler
{
public:

    virtual void begin()
    {
        ++begin_called;
    }

    virtual void handle(int event)
    {
        handle_called = event;
    }

    virtual void end()
    {
        ++end_called;
    }
};

another_ptr<event_handler> get_event_handler()
{
    another_ptr<event_handler> p(new event_handler_impl);
    return p;
}

smart_ptr<event_handler, shared_storage<_>, boost_ref<_> > current_handler;

void install_event_handler(smart_ptr<event_handler, shared_storage<_>, boost_ref<_> > p)
{
    p->begin();
    current_handler = p;
}

void handle_event(int event)
{
    current_handler->handle(event);
}

void remove_event_handler()
{
    current_handler->end();
    current_handler.reset();
}

template<class P> class smart_pointer_deleter
{
private:

    P p_;

public:

    smart_pointer_deleter(P const & p): p_(p)
    {
    }

    void operator()(void const *)
    {
        p_.reset();
    }
};

void test()
{
    another_ptr<event_handler> p = get_event_handler();

    smart_ptr<event_handler, shared_storage<_>, boost_ref<_> > q(p.get(), smart_pointer_deleter< another_ptr<event_handler> >(p));

    p.reset();

    BOOST_TEST(begin_called == 0);

    install_event_handler(q);

    BOOST_TEST(begin_called == 1);

    BOOST_TEST(handle_called == 0);

    handle_event(17041);

    BOOST_TEST(handle_called == 17041);

    BOOST_TEST(end_called == 0);

    remove_event_handler();

    BOOST_TEST(end_called == 1);
}

} // namespace n_spt_another_sp

namespace n_spt_shared_from_this
{

class X
{
public:

    virtual void f() = 0;

protected:

    ~X() {}
};

class Y
{
public:

    virtual smart_ptr<X, shared_storage<_>, boost_ref<_> > getX() = 0;

protected:

    ~Y() {}
};

class impl: public X, public Y
{
private:

    smart_ptr<impl, weak_storage<_>, boost_ref<_> > weak_this;

    impl(impl const &);
    impl & operator=(impl const &);

    impl() {}

public:

    static smart_ptr<impl, shared_storage<_>, boost_ref<_> > create()
    {
        smart_ptr<impl, shared_storage<_>, boost_ref<_> > pi(new impl);
        pi->weak_this = pi;
        return pi;
    }

    virtual void f() {}

    virtual smart_ptr<X, shared_storage<_>, boost_ref<_> > getX()
    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > px = weak_this.lock();
        return px;
    }
};

void test()
{
    smart_ptr<Y, shared_storage<_>, boost_ref<_> > py = impl::create();
    BOOST_TEST(py.get() != 0);
    BOOST_TEST(py.use_count() == 1);

    smart_ptr<X, shared_storage<_>, boost_ref<_> > px = py->getX();
    BOOST_TEST(px.get() != 0);
    BOOST_TEST(py.use_count() == 2);

    smart_ptr<Y, shared_storage<_>, boost_ref<_> > py2 = dynamic_pointer_cast<Y>(px);
    BOOST_TEST(py.get() == py2.get());
    BOOST_TEST(!(py < py2 || py2 < py));
    BOOST_TEST(py.use_count() == 3);
}

} // namespace n_spt_shared_from_this

namespace n_spt_wrap
{

void test()
{
}

} // namespace n_spt_wrap

namespace n_spt_incomplete
{

class file
{
public:

    file(): fread_called(false)
    {
        ++file_instances;
    }

    ~file()
    {
        BOOST_TEST(fread_called);
        --file_instances;
    }

    bool fread_called;
};

smart_ptr<file, shared_storage<_>, boost_ref<_> > fopen(char const *, char const *)
{
    smart_ptr<file, shared_storage<_>, boost_ref<_> > pf(new file);
    return pf;
}

void fread(smart_ptr<file, shared_storage<_>, boost_ref<_> > pf, void *, long)
{
    pf->fread_called = true;
}

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

class file::impl
{
private:

    impl(impl const &);
    impl & operator=(impl const &);

    long total_size_;

public:

    impl(char const *, char const *): total_size_(0)
    {
        ++file_instances;
    }

    ~impl()
    {
        --file_instances;
    }

    void read(void *, long size)
    {
        total_size_ += size;
    }

    long total_size() const
    {
        return total_size_;
    }
};

file::file(char const * name, char const * mode): pimpl_(new impl(name, mode))
{
}

void file::read(void * data, long size)
{
    pimpl_->read(data, size);
}

long file::total_size() const
{
    return pimpl_->total_size();
}

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

class X_impl: public X
{
private:

    X_impl(X_impl const &);
    X_impl & operator=(X_impl const &);

    int n_;

public:

    X_impl(): n_(0)
    {
        ++X_instances;
    }

    ~X_impl()
    {
        --X_instances;
    }

    virtual void f(int n)
    {
        n_ += n;
    }

    virtual int g()
    {
        return n_;
    }
};

smart_ptr<X, shared_storage<_>, boost_ref<_> > createX()
{
    smart_ptr<X, shared_storage<_>, boost_ref<_> > px(new X_impl);
    return px;
}

} // namespace n_spt_abstract

} // namespace boost

int main()
{
    using namespace boost;
    n_element_type::test();
    n_constructors::test();
    n_assignment::test();
    n_reset::test();
    n_access::test();
    n_use_count::test();
    n_swap::test();
    n_comparison::test();
    n_static_cast::test();
    n_dynamic_cast::test();

    n_map::test();

    n_transitive::test();
    n_report_1::test();
    n_report_2::test();

    n_spt_incomplete::test();
    n_spt_pimpl::test();
    n_spt_abstract::test();
    n_spt_preventing_delete::test();
    n_spt_array::test();
    n_spt_static::test();
    n_spt_intrusive::test();
    n_spt_another_sp::test();
    n_spt_shared_from_this::test();
    n_spt_wrap::test();

    return report_errors();
}

