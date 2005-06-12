//
//  std_ptr_shared_ptr_test.cpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//----------------------------------------------------------------------------
//  The above copywrite was for the code before the following changes:
//----------------------------------------------------------------------------
// Change log(latest at top):
// - 2005-06-07: Larry Evans 
//   WHAT:
//     1) renamed std_ptrs.hpp to std_ptr.hpp
//     2) renamed ptr template from std_ptrs.hpp to std_ptr.
// - 2005-05-31: Larry Evans 
//   WHAT:
//     1) eliminated std::auto_ptr tests.
//     2) eliminated tests which reset deleter.
//     3) eliminated namespace n_spt_shared_from_this.
//     4) eliminated comparison tests between "distinct pointer types"
//     5) eliminated deleter_constructor tests.
//     6) eliminated calls to checked_deleter.
//     7) eliminated namespaces:
//          n_report_2
//          n_spt_preventing_delete
//          n_spt_array
//          n_spt_static
//          n_spt_another_sp
//          n_spt_intrusive
//     8) eliminated namespaces:
//          n_const_cast
//     9) eliminated tests using smart_ptr for void types in:
//          n_constructors::pointer_constructor(void)
//    10) rm'ed all ptr<void> from:
//          n_constructors::copy_constructor(void)
//          n_assignment::copy_assignment(void)
//          n_reset::plain_reset(void)
//   WHY:
//     1) current policy_ptr<...>::to has no CTOR's taking auto_ptr's.
//     2) current shared_storage_ has no interface for this.  Maybe
//        later it will be installed.
//     3) it requires emulation of boost::weak_ptr::lock which cannot
//        be done without weak_storage_ knowing about the smart_ptr<...> ::to
//        interface in order to return the corresponding emulation of
//        boost::shared_ptr.  It's unclear to me how to do this at
//        the moment.
//     4) policy_ptr<...>::to makes no provision for multiple inheritance
//        and comparison between pointers to different superclasses
//        of a give class.
//     5) same as 2).
//     6) current shared_storage_ has no CTOR taking a deleter.  In addition,
//        the smart_ptr<...>::to has no such CTOR.
//     7) same as 6.
//     8) Couldn't figure *quick* way to avoid g++ compiler error:
//          ../../../boost/policy_ptr/policy/std.hpp:105: error: invalid static_cast from type `const volatile void* const' to type `void*'
//        in libs/policy_ptr/test/std_ptrs_shared_ptr_test.cpp.
//     9) Avoid compiler error:
/*
std_ptrs_shared_ptr_test.cpp:227:   instantiated from here
../../../../../boost/checked_delete.hpp:32: error: invalid application of `sizeof' to a void type
../../../../../boost/checked_delete.hpp:33: error: invalid application of `sizeof' to a void type
../../../../../boost/checked_delete.hpp:34: warning: deleting `void*' is undefined

 */
//    10) Avoid compiler error:
/*
../../../../../boost/checked_delete.hpp: In function `void boost::checked_delete(T*) [with T = void]':

std_ptrs_shared_ptr_test.cpp:501:   instantiated from here
../../../../../boost/checked_delete.hpp:32: error: invalid application of `sizeof' to a void type
../../../../../boost/checked_delete.hpp:33: error: invalid application of `sizeof' to a void type
../../../../../boost/checked_delete.hpp:34: warning: deleting `void*' is undefined

 */
// - 2005-05-30: Larry Evans 
//   WHAT:
//     1) copied from:
//          libs/smart_ptr/test/shared_ptr_test.cpp
//     2) replaced boost/shared_ptr.hpp with boost/policy_ptr/std_ptrs.hpp.
//     3) replaced:
//          shared_ptr<X> with ptr<X>::shared
//          weak_ptr<X> with ptr<X>::weak
//   WHY:
//     1) provide similar test for policy_ptr emulators of shared_ptr.
//     2) to enable substitution with policy_ptr's emulations.
//     3) to use policy_ptr emulations of the substituted smart pointers.
//----------------------------------------------------------------------------
#include <boost/config.hpp>
#include <iostream>
//#define BOOST_SP_DEBUG_MODE
#ifdef BOOST_SP_DEBUG_MODE
template<class T>
void sp_constructor_hook(T& sp, char const* msg)
{
    std::cout<<"sp_constructor_hook:ptr="<<sp.get()<<":msg="<<msg<<"\n";
}
template<class T>
void sp_destructor_hook(T& sp, char const* msg)
{
    std::cout<<"sp_destructor_hook:ptr="<<sp.get()<<":msg="<<msg<<"\n";
}
#endif

#if defined(BOOST_MSVC)

#pragma warning(disable: 4786)  // identifier truncated in debug info
#pragma warning(disable: 4710)  // function not inlined
#pragma warning(disable: 4711)  // function selected for automatic inline expansion
#pragma warning(disable: 4514)  // unreferenced inline removed
#pragma warning(disable: 4355)  // 'this' : used in base member initializer list

#if (BOOST_MSVC >= 1310)
#pragma warning(disable: 4675)  // resolved overload found with Koenig lookup
#endif

#endif


#include <boost/detail/lightweight_test.hpp>

#include <boost/policy_ptr/std_ptr.hpp>

#include <map>
#include <vector>

//

namespace n_element_type
{

void f(int &)
{
}

void test()
{
    typedef boost::std_ptr<int>::shared::element_type T;
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
        boost::std_ptr<int>::shared pi;
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        boost::std_ptr<void>::shared pv;
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
    }

    {
        boost::std_ptr<incomplete>::shared px;
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

template<class T> void pc0_test(T * p)
{
    BOOST_TEST(p == 0);
    typename boost::std_ptr<T>::shared pt(p);
    BOOST_TEST(pt? false: true);
    BOOST_TEST(!pt);
    BOOST_TEST(pt.get() == 0);
    BOOST_TEST(pt.use_count() == 1);
    BOOST_TEST(pt.unique());
}

void pointer_constructor()
{
#if 1
    pc0_test(static_cast<int*>(0));

#if !defined(BOOST_MSVC) || (BOOST_MSVC > 1300)

    pc0_test(static_cast<int const*>(0));
    pc0_test(static_cast<int volatile*>(0));
    pc0_test(static_cast<int const volatile*>(0));

#endif

    {
        boost::std_ptr<int const>::shared pi(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    {
        boost::std_ptr<int volatile>::shared pi(static_cast<int*>(0));
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
    }

    pc0_test(static_cast<X*>(0));
    pc0_test(static_cast<X const*>(0));
    pc0_test(static_cast<X volatile*>(0));
    pc0_test(static_cast<X const volatile*>(0));

    {
        boost::std_ptr<X const>::shared px(static_cast<X*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        boost::std_ptr<X>::shared px(static_cast<Y*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        boost::std_ptr<X const>::shared px(static_cast<Y*>(0));
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
    }

    {
        int * p = new int(7);
        boost::std_ptr<int>::shared pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == p);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);
    }

    {
        int * p = new int(7);
        boost::std_ptr<int const>::shared pi(p);
        BOOST_TEST(pi? true: false);
        BOOST_TEST(!!pi);
        BOOST_TEST(pi.get() == p);
        BOOST_TEST(pi.use_count() == 1);
        BOOST_TEST(pi.unique());
        BOOST_TEST(*pi == 7);
    }

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        boost::std_ptr<X>::shared px(p);
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
        boost::std_ptr<X const>::shared px(p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());
        BOOST_TEST(X::instances == 1);
    }

#endif
    BOOST_TEST(X::instances == 0);
    BOOST_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        boost::std_ptr<X>::shared px(p);
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
#if 1

    {
        Y * p = new Y;
        boost::std_ptr<X const>::shared px(p);
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
#endif
}

void copy_constructor()
{
    {
        boost::std_ptr<int>::shared pi;

        boost::std_ptr<int>::shared pi2(pi);
        BOOST_TEST(pi2 == pi);
        BOOST_TEST(pi2? false: true);
        BOOST_TEST(!pi2);
        BOOST_TEST(pi2.get() == 0);
        BOOST_TEST(pi2.use_count() == pi.use_count());

    }

    {
        boost::std_ptr<void>::shared pv;

        boost::std_ptr<void>::shared pv2(pv);
        BOOST_TEST(pv2 == pv);
        BOOST_TEST(pv2? false: true);
        BOOST_TEST(!pv2);
        BOOST_TEST(pv2.get() == 0);
        BOOST_TEST(pv2.use_count() == pv.use_count());
    }

    {
        boost::std_ptr<incomplete>::shared px;

        boost::std_ptr<incomplete>::shared px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? false: true);
        BOOST_TEST(!px2);
        BOOST_TEST(px2.get() == 0);
        BOOST_TEST(px2.use_count() == px.use_count());

        boost::std_ptr<void>::shared px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? false: true);
        BOOST_TEST(!px3);
        BOOST_TEST(px3.get() == 0);
        BOOST_TEST(px3.use_count() == px.use_count());
    }

    {
        boost::std_ptr<int>::shared pi(static_cast<int*>(0));

        boost::std_ptr<int>::shared pi2(pi);
        BOOST_TEST(pi2 == pi);
        BOOST_TEST(pi2? false: true);
        BOOST_TEST(!pi2);
        BOOST_TEST(pi2.get() == 0);
        BOOST_TEST(pi2.use_count() == 2);
        BOOST_TEST(!pi2.unique());
        BOOST_TEST(pi2.use_count() == pi.use_count());
        BOOST_TEST(!(pi < pi2 || pi2 < pi)); // shared ownership test

        boost::std_ptr<void>::shared pi3(pi);
        BOOST_TEST(pi3 == pi);
        BOOST_TEST(pi3? false: true);
        BOOST_TEST(!pi3);
        BOOST_TEST(pi3.get() == 0);
        BOOST_TEST(pi3.use_count() == 3);
        BOOST_TEST(!pi3.unique());
        BOOST_TEST(pi3.use_count() == pi.use_count());
        BOOST_TEST(!(pi < pi3 || pi3 < pi)); // shared ownership test

        boost::std_ptr<void>::shared pi4(pi2);
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
        boost::std_ptr<X>::shared px(static_cast<X*>(0));

        boost::std_ptr<X>::shared px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? false: true);
        BOOST_TEST(!px2);
        BOOST_TEST(px2.get() == 0);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px2.use_count() == px.use_count());
        BOOST_TEST(!(px < px2 || px2 < px)); // shared ownership test

        boost::std_ptr<void>::shared px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? false: true);
        BOOST_TEST(!px3);
        BOOST_TEST(px3.get() == 0);
        BOOST_TEST(px3.use_count() == 3);
        BOOST_TEST(!px3.unique());
        BOOST_TEST(px3.use_count() == px.use_count());
        BOOST_TEST(!(px < px3 || px3 < px)); // shared ownership test

        boost::std_ptr<void>::shared px4(px2);
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
        boost::std_ptr<int>::shared pi(p);

        boost::std_ptr<int>::shared pi2(pi);
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

    BOOST_TEST(X::instances == 0);

    {
        X * p = new X;
        boost::std_ptr<X>::shared px(p);
        BOOST_TEST(px.get() == p);

        boost::std_ptr<X>::shared px2(px);
        BOOST_TEST(px2 == px);
        BOOST_TEST(px2? true: false);
        BOOST_TEST(!!px2);
        BOOST_TEST(px2.get() == p);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());

        BOOST_TEST(X::instances == 1);

        BOOST_TEST(px2.use_count() == px.use_count());
        BOOST_TEST(!(px < px2 || px2 < px)); // shared ownership test

        boost::std_ptr<void>::shared px3(px);
        BOOST_TEST(px3 == px);
        BOOST_TEST(px3? true: false);
        BOOST_TEST(!!px3);
        BOOST_TEST(px3.get() == p);
        BOOST_TEST(px3.use_count() == 3);
        BOOST_TEST(!px3.unique());
        BOOST_TEST(px3.use_count() == px.use_count());
        BOOST_TEST(!(px < px3 || px3 < px)); // shared ownership test

        boost::std_ptr<void>::shared px4(px2);
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
        boost::std_ptr<Y>::shared py(p);
        BOOST_TEST(py.get() == p);

        boost::std_ptr<X>::shared px(py);
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

        boost::std_ptr<void const>::shared pv(px);
        BOOST_TEST(pv == px);
        BOOST_TEST(pv? true: false);
        BOOST_TEST(!!pv);
        BOOST_TEST(pv.get() == px.get());
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(!pv.unique());
        BOOST_TEST(pv.use_count() == px.use_count());
        BOOST_TEST(!(px < pv || pv < px)); // shared ownership test

        boost::std_ptr<void const>::shared pv2(py);
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
        boost::std_ptr<Y>::weak wp;
        BOOST_TEST(wp.use_count() == 0);

        try
        {
            boost::std_ptr<Y>::shared p2(wp);
            BOOST_ERROR("ptr<Y>::shared p2(wp) failed to throw");
        }
        catch(boost::bad_weak_ptr)
        {
        }

        try
        {
            boost::std_ptr<X>::shared p3(wp);
            BOOST_ERROR("ptr<X>::shared p3(wp) failed to throw");
        }
        catch(boost::bad_weak_ptr)
        {
        }
    }

    {
        boost::std_ptr<Y>::shared p;
        boost::std_ptr<Y>::weak wp(p);

        if(wp.use_count() != 0) // 0 allowed but not required
        {
            boost::std_ptr<Y>::shared p2(wp);
            BOOST_TEST(p2.use_count() == wp.use_count());
            BOOST_TEST(p2.get() == 0);

            boost::std_ptr<X>::shared p3(wp);
            BOOST_TEST(p3.use_count() == wp.use_count());
            BOOST_TEST(p3.get() == 0);
        }
    }

    {
        boost::std_ptr<Y>::shared p(new Y);
        boost::std_ptr<Y>::weak wp(p);

        {
            boost::std_ptr<Y>::shared p2(wp);
            BOOST_TEST(p2? true: false);
            BOOST_TEST(!!p2);
            BOOST_TEST(p2.get() == p.get());
            BOOST_TEST(p2.use_count() == 2);
            BOOST_TEST(!p2.unique());
            BOOST_TEST(p2.use_count() == wp.use_count());

            BOOST_TEST(p.use_count() == p2.use_count());
            BOOST_TEST(!(p < p2 || p2 < p)); // shared ownership test

            boost::std_ptr<X>::shared p3(wp);
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
            boost::std_ptr<Y>::shared p2(wp);
            BOOST_ERROR("ptr<Y>::shared p2(wp) failed to throw");
        }
        catch(boost::bad_weak_ptr)
        {
        }

        try
        {
            boost::std_ptr<X>::shared p3(wp);
            BOOST_ERROR("ptr<X>::shared p3(wp) failed to throw");
        }
        catch(boost::bad_weak_ptr)
        {
        }
    }
}

void test()
{
#if 1
    default_constructor();
#endif
    pointer_constructor();
#if 1
    copy_constructor();
    weak_ptr_constructor();
#endif
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
        boost::std_ptr<incomplete>::shared p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<incomplete>::shared p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<incomplete>::shared p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);
    }

    {
        boost::std_ptr<void>::shared p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<void>::shared p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<void>::shared p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        p1 = p3;

        BOOST_TEST(p1 == p3);
    }

    {
        boost::std_ptr<X>::shared p1;

        p1 = p1;

        BOOST_TEST(p1 == p1);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<X>::shared p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<X>::shared p3(p1);

        p1 = p3;

        BOOST_TEST(p1 == p3);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        BOOST_TEST(X::instances == 0);

        boost::std_ptr<X>::shared p4(new X);

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
        boost::std_ptr<void>::shared p1;

        boost::std_ptr<incomplete>::shared p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        boost::std_ptr<int>::shared p4(new int);
        BOOST_TEST(p4.use_count() == 1);

        boost::std_ptr<void>::shared p5(p4);
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
        boost::std_ptr<X>::shared p1;

        boost::std_ptr<Y>::shared p2;

        p1 = p2;

        BOOST_TEST(p1 == p2);
        BOOST_TEST(p1? false: true);
        BOOST_TEST(!p1);
        BOOST_TEST(p1.get() == 0);

        BOOST_TEST(X::instances == 0);
        BOOST_TEST(Y::instances == 0);

        boost::std_ptr<Y>::shared p4(new Y);

        BOOST_TEST(X::instances == 1);
        BOOST_TEST(Y::instances == 1);
        BOOST_TEST(p4.use_count() == 1);

        boost::std_ptr<X>::shared p5(p4);
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

void test()
{
    copy_assignment();
    conversion_assignment();
}

} // namespace n_assignment

namespace n_reset
{

class incomplete;

incomplete * p0 = 0;

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
        boost::std_ptr<int>::shared pi;
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        boost::std_ptr<int>::shared pi(static_cast<int*>(0));
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        boost::std_ptr<int>::shared pi(new int);
        pi.reset();
        BOOST_TEST(pi? false: true);
        BOOST_TEST(!pi);
        BOOST_TEST(pi.get() == 0);
    }

    {
        boost::std_ptr<incomplete>::shared px;
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }

    {
        boost::std_ptr<X>::shared px;
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
    }

    {
        BOOST_TEST(X::instances == 0);
        boost::std_ptr<X>::shared px(new X);
        BOOST_TEST(X::instances == 1);
        px.reset();
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(X::instances == 0);
    }

    {
        boost::std_ptr<void>::shared pv;
        pv.reset();
        BOOST_TEST(pv? false: true);
        BOOST_TEST(!pv);
        BOOST_TEST(pv.get() == 0);
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
        boost::std_ptr<int>::shared pi;

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
        boost::std_ptr<X>::shared px;

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
        boost::std_ptr<void>::shared pv;

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

void test()
{
    plain_reset();
    pointer_reset();
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
        boost::std_ptr<X>::shared px;
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using boost::get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        boost::std_ptr<X>::shared px(static_cast<X*>(0));
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using boost::get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        boost::std_ptr<X>::shared px(static_cast<X*>(0));
        BOOST_TEST(px.get() == 0);
        BOOST_TEST(px? false: true);
        BOOST_TEST(!px);

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using boost::get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        boost::std_ptr<X>::shared px(p);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(&*px == px.get());
        BOOST_TEST(px.operator ->() == px.get());

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using boost::get_pointer;
#endif

        BOOST_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        boost::std_ptr<X>::shared px(p);
        BOOST_TEST(px.get() == p);
        BOOST_TEST(px? true: false);
        BOOST_TEST(!!px);
        BOOST_TEST(&*px == px.get());
        BOOST_TEST(px.operator ->() == px.get());

#if defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using boost::get_pointer;
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
        boost::std_ptr<X>::shared px(static_cast<X*>(0));
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        boost::std_ptr<X>::shared px2(px);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
    }

    {
        boost::std_ptr<X>::shared px(new X);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        boost::std_ptr<X>::shared px2(px);
        BOOST_TEST(px2.use_count() == 2);
        BOOST_TEST(!px2.unique());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(!px.unique());
    }

    {
        boost::std_ptr<X>::shared px(new X);
        BOOST_TEST(px.use_count() == 1);
        BOOST_TEST(px.unique());

        boost::std_ptr<X>::shared px2(px);
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
        boost::std_ptr<X>::shared px;
        boost::std_ptr<X>::shared px2;

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
        boost::std_ptr<X>::shared px;
        boost::std_ptr<X>::shared px2(p);
        boost::std_ptr<X>::shared px3(px2);

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
        boost::std_ptr<X>::shared px(p1);
        boost::std_ptr<X>::shared px2(p2);
        boost::std_ptr<X>::shared px3(px2);

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
        boost::std_ptr<X>::shared px;
        BOOST_TEST(px == px);
        BOOST_TEST(!(px != px));
        BOOST_TEST(!(px < px));

        boost::std_ptr<X>::shared px2;

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        boost::std_ptr<X>::shared px;
        boost::std_ptr<X>::shared px2(px);

        BOOST_TEST(px2 == px2);
        BOOST_TEST(!(px2 != px2));
        BOOST_TEST(!(px2 < px2));

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        boost::std_ptr<X>::shared px;
        boost::std_ptr<X>::shared px2(new X);

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
        boost::std_ptr<X>::shared px(new X);
        boost::std_ptr<X>::shared px2(new X);

        BOOST_TEST(px.get() != px2.get());
        BOOST_TEST(px != px2);
        BOOST_TEST(!(px == px2));
        BOOST_TEST(px < px2 || px2 < px);
        BOOST_TEST(!(px < px2 && px2 < px));
    }

    {
        boost::std_ptr<X>::shared px(new X);
        boost::std_ptr<X>::shared px2(px);

        BOOST_TEST(px2 == px2);
        BOOST_TEST(!(px2 != px2));
        BOOST_TEST(!(px2 < px2));

        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(px == px2);
        BOOST_TEST(!(px != px2));
        BOOST_TEST(!(px < px2 || px2 < px));
    }

    {
        boost::std_ptr<X>::shared px(new X);
        boost::std_ptr<Y>::shared py(new Y);
        boost::std_ptr<Z>::shared pz(new Z);

        BOOST_TEST(px.get() != pz.get());
        BOOST_TEST(px != pz);
        BOOST_TEST(!(px == pz));

        BOOST_TEST(py.get() != pz.get());
        BOOST_TEST(py != pz);
        BOOST_TEST(!(py == pz));

        BOOST_TEST(px < pz || pz < px);
        BOOST_TEST(py < pz || pz < py);

        BOOST_TEST(!(px < pz && pz < px));
        BOOST_TEST(!(py < pz && pz < py));

        boost::std_ptr<void>::shared pvx(px);

        BOOST_TEST(pvx == pvx);
        BOOST_TEST(!(pvx != pvx));
        BOOST_TEST(!(pvx < pvx));

        boost::std_ptr<void>::shared pvy(py);
        boost::std_ptr<void>::shared pvz(pz);

        BOOST_TEST(pvx < pvy || pvy < pvx);
        BOOST_TEST(pvx < pvz || pvz < pvx);
        BOOST_TEST(pvy < pvz || pvz < pvy);

        BOOST_TEST(!(pvx < pvy && pvy < pvx));
        BOOST_TEST(!(pvx < pvz && pvz < pvx));
        BOOST_TEST(!(pvy < pvz && pvz < pvy));
    }

    {
        boost::std_ptr<Z>::shared pz(new Z);
        boost::std_ptr<X>::shared px(pz);

        BOOST_TEST(px == px);
        BOOST_TEST(!(px != px));
        BOOST_TEST(!(px < px));

        boost::std_ptr<Y>::shared py(pz);

        BOOST_TEST(px.get() == pz.get());
        BOOST_TEST(px == pz);
        BOOST_TEST(!(px != pz));

        BOOST_TEST(py.get() == pz.get());
        BOOST_TEST(py == pz);
        BOOST_TEST(!(py != pz));

        BOOST_TEST(!(py < pz || pz < py));

        boost::std_ptr<void>::shared pvx(px);
        boost::std_ptr<void>::shared pvy(py);
        boost::std_ptr<void>::shared pvz(pz);

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
        typedef boost::std_ptr<void>::shared shared_void_type;
        shared_void_type pv;

        boost::std_ptr<int>::shared pi = boost::static_pointer_cast<int>(pv);
        BOOST_TEST(pi.get() == 0);

        boost::std_ptr<X>::shared px = boost::static_pointer_cast<X>(pv);
        BOOST_TEST(px.get() == 0);
    }

    {
        boost::std_ptr<int>::shared pi(new int);
        boost::std_ptr<void>::shared pv(pi);

        boost::std_ptr<int>::shared pi2 = boost::static_pointer_cast<int>(pv);
        BOOST_TEST(pi.get() == pi2.get());
        BOOST_TEST(!(pi < pi2 || pi2 < pi));
        BOOST_TEST(pi.use_count() == 3);
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(pi2.use_count() == 3);
    }

    {
        boost::std_ptr<X>::shared px(new X);
        boost::std_ptr<void>::shared pv(px);

        boost::std_ptr<X>::shared px2 = boost::static_pointer_cast<X>(pv);
        BOOST_TEST(px.get() == px2.get());
        BOOST_TEST(!(px < px2 || px2 < px));
        BOOST_TEST(px.use_count() == 3);
        BOOST_TEST(pv.use_count() == 3);
        BOOST_TEST(px2.use_count() == 3);
    }

    {
        boost::std_ptr<X>::shared px(new Y);

        boost::std_ptr<Y>::shared py = boost::static_pointer_cast<Y>(px);
        BOOST_TEST(px.get() == py.get());
        BOOST_TEST(px.use_count() == 2);
        BOOST_TEST(py.use_count() == 2);

        boost::std_ptr<X>::shared px2(py);
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
        boost::std_ptr<V>::shared pv;
        boost::std_ptr<W>::shared pw = boost::dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);
    }

    {
        boost::std_ptr<V>::shared pv(static_cast<V*>(0));

        boost::std_ptr<W>::shared pw = boost::dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        boost::std_ptr<V>::shared pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        boost::std_ptr<V>::shared pv(static_cast<W*>(0));

        boost::std_ptr<W>::shared pw = boost::dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        boost::std_ptr<V>::shared pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        boost::std_ptr<V>::shared pv(new V);

        boost::std_ptr<W>::shared pw = boost::dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == 0);

        boost::std_ptr<V>::shared pv2(pw);
        BOOST_TEST(pv < pv2 || pv2 < pv);
    }

    {
        boost::std_ptr<V>::shared pv(new W);

        boost::std_ptr<W>::shared pw = boost::dynamic_pointer_cast<W>(pv);
        BOOST_TEST(pw.get() == pv.get());
        BOOST_TEST(pv.use_count() == 2);
        BOOST_TEST(pw.use_count() == 2);

        boost::std_ptr<V>::shared pv2(pw);
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
    std::vector< boost::std_ptr<int>::shared > vi;

    {
        boost::std_ptr<int>::shared pi1(new int);
        boost::std_ptr<int>::shared pi2(new int);
        boost::std_ptr<int>::shared pi3(new int);

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

    std::vector< boost::std_ptr<X>::shared > vx;

    {
        boost::std_ptr<X>::shared px1(new X);
        boost::std_ptr<X>::shared px2(new X);
        boost::std_ptr<X>::shared px3(new X);

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

    std::map< boost::std_ptr<void>::shared, long > m;

    {
        for(std::vector< boost::std_ptr<int>::shared >::iterator i = vi.begin(); i != vi.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::vector< boost::std_ptr<X>::shared >::iterator i = vx.begin(); i != vx.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::map< boost::std_ptr<void>::shared, long >::iterator i = m.begin(); i != m.end(); ++i)
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
    boost::std_ptr<X>::shared next;
};

void test()
{
    boost::std_ptr<X>::shared p(new X);
    p->next = boost::std_ptr<X>::shared(new X);
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

    boost::std_ptr<foo>::shared m_self;
}; 

void test()
{
    foo * foo_ptr = new foo;
    foo_ptr->suicide();
} 

} // namespace n_report_1

namespace n_spt_incomplete
{

class file;

boost::std_ptr<file>::shared fopen(char const * name, char const * mode);
void fread(boost::std_ptr<file>::shared f, void * data, long size);

int file_instances = 0;

void test()
{
    BOOST_TEST(file_instances == 0);

    {
        boost::std_ptr<file>::shared pf = fopen("name", "mode");
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
    boost::std_ptr<impl>::shared pimpl_;

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

    virtual ~X() {}
protected:

//was here before 2005-05-31:    virtual ~X() {}
};

boost::std_ptr<X>::shared createX();

int X_instances = 0;

void test()
{
    BOOST_TEST(X_instances == 0);

    {
        boost::std_ptr<X>::shared px = createX();

        BOOST_TEST(X_instances == 1);

        px->f(18);
        px->f(152);

        BOOST_TEST(px->g() == 170);
    }

    BOOST_TEST(X_instances == 0);
}

} // namespace n_spt_abstract

namespace n_spt_wrap
{

void test()
{
}

} // namespace n_spt_wrap

int main()
{
#if 1
    n_element_type::test();
#endif    
    n_constructors::test();
#if 1
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

    n_spt_incomplete::test();
    n_spt_pimpl::test();
    n_spt_abstract::test();
    n_spt_wrap::test();
#endif
    return boost::report_errors();
}

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

boost::std_ptr<file>::shared fopen(char const *, char const *)
{
    boost::std_ptr<file>::shared pf(new file);
    return pf;
}

void fread(boost::std_ptr<file>::shared pf, void *, long)
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

boost::std_ptr<X>::shared createX()
{
    boost::std_ptr<X>::shared px(new X_impl);
    return px;
}

} // namespace n_spt_abstract
