//
//  shared_ptr_basic_test.cpp
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
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

// Hack for VC
namespace boost
{
    using mpl::_;

int cnt = 0;

struct X
{
    X()
    {
        ++cnt;
    }

    ~X() // virtual destructor deliberately omitted
    {
        --cnt;
    }

    virtual int id() const
    {
        return 1;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

struct Y: public X
{
    Y()
    {
        ++cnt;
    }

    ~Y()
    {
        --cnt;
    }

    virtual int id() const
    {
        return 2;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

int * get_object()
{
    ++cnt;
    return &cnt;
}

void release_object(int * p)
{
    BOOST_TEST(p == &cnt);
    --cnt;
}

template<class T> void test_is_X(smart_ptr<T, shared_storage<_>, boost_ref<_> > const & p)
{
    BOOST_TEST(p->id() == 1);
    BOOST_TEST((*p).id() == 1);
}

template<class T> void test_is_X(smart_ptr<T, weak_storage<_>, boost_ref<_> > const & p)
{
    BOOST_TEST(p.get() != 0);
    BOOST_TEST(p.get()->id() == 1);
}

template<class T> void test_is_Y(smart_ptr<T, shared_storage<_>, boost_ref<_> > const & p)
{
    BOOST_TEST(p->id() == 2);
    BOOST_TEST((*p).id() == 2);
}

template<class T> void test_is_Y(smart_ptr<T, weak_storage<_>, boost_ref<_> > const & p)
{
    smart_ptr<T, shared_storage<_>, boost_ref<_> > q = p.lock();
    BOOST_TEST(q.get() != 0);
    BOOST_TEST(q->id() == 2);
}

template<class T> void test_eq(T const & a, T const & b)
{
    BOOST_TEST(a == b);
    BOOST_TEST(!(a != b));
    BOOST_TEST(!(a < b));
    BOOST_TEST(!(b < a));
}

template<class T> void test_ne(T const & a, T const & b)
{
    BOOST_TEST(!(a == b));
    BOOST_TEST(a != b);
    BOOST_TEST(a < b || b < a);
    BOOST_TEST(!(a < b && b < a));
}

template<class T, class U> void test_shared(smart_ptr<T, weak_storage<_>, boost_ref<_> > const & a, smart_ptr<U, weak_storage<_>, boost_ref<_> > const & b)
{
    BOOST_TEST(!(a < b));
    BOOST_TEST(!(b < a));
}

template<class T, class U> void test_nonshared(smart_ptr<T, weak_storage<_>, boost_ref<_> > const & a, smart_ptr<U, weak_storage<_>, boost_ref<_> > const & b)
{
    BOOST_TEST(a < b || b < a);
    BOOST_TEST(!(a < b && b < a));
}

template<class T, class U> void test_eq2(T const & a, U const & b)
{
    BOOST_TEST(a == b);
    BOOST_TEST(!(a != b));
}

template<class T, class U> void test_ne2(T const & a, U const & b)
{
    BOOST_TEST(!(a == b));
    BOOST_TEST(a != b);
}

template<class T> void test_is_zero(smart_ptr<T, shared_storage<_>, boost_ref<_> > const & p)
{
    BOOST_TEST(!p);
    BOOST_TEST(p.get() == 0);
}

template<class T> void test_is_nonzero(smart_ptr<T, shared_storage<_>, boost_ref<_> > const & p)
{
    // p? true: false is used to test p in a boolean context.
    // BOOST_TEST(p) is not guaranteed to test the conversion,
    // as the macro might test !!p instead.
    BOOST_TEST(p? true: false);
    BOOST_TEST(p.get() != 0);
}
}

int main()
{
    using namespace boost;

    {
        smart_ptr<X, shared_storage<_>, boost_ref<_> > p(new Y);
        smart_ptr<X, shared_storage<_>, boost_ref<_> > p2(new X);

        test_is_nonzero(p);
        test_is_nonzero(p2);
        test_is_Y(p);
        test_is_X(p2);
        test_ne(p, p2);

        {
            smart_ptr<X, shared_storage<_>, boost_ref<_> > q(p);
            test_eq(p, q);
        }

        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p3 = dynamic_pointer_cast<Y>(p);
        smart_ptr<Y, shared_storage<_>, boost_ref<_> > p4 = dynamic_pointer_cast<Y>(p2);

        test_is_nonzero(p3);
        test_is_zero(p4);

        BOOST_TEST(p.use_count() == 2);
        BOOST_TEST(p2.use_count() == 1);
        BOOST_TEST(p3.use_count() == 2);

        test_is_Y(p3);
        test_eq2(p, p3);
        test_ne2(p2, p4);

        smart_ptr<void, shared_storage<_>, boost_ref<_> > p5(p);

        test_is_nonzero(p5);
        test_eq2(p, p5);

        smart_ptr<X, weak_storage<_>, boost_ref<_> > wp1(p2);

        BOOST_TEST(!wp1.expired());
        BOOST_TEST(wp1.use_count() != 0);

        p.reset();
        p2.reset();
        p3.reset();
        p4.reset();

        test_is_zero(p);
        test_is_zero(p2);
        test_is_zero(p3);
        test_is_zero(p4);

        BOOST_TEST(p5.use_count() == 1);

        BOOST_TEST(wp1.expired());
        BOOST_TEST(wp1.use_count() == 0);

        try
        {
            smart_ptr<X, shared_storage<_>, boost_ref<_> > sp1(wp1);
            BOOST_ERROR("shared_ptr<X> sp1(wp1) failed to throw");
        }
        catch(bad_weak_ptr const &)
        {
        }

        test_is_zero(wp1.lock());

        smart_ptr<X, weak_storage<_>, boost_ref<_> > wp2 = static_pointer_cast<X>(p5);

        BOOST_TEST(wp2.use_count() == 1);
        test_is_Y(wp2);
        test_nonshared(wp1, wp2);

        // Scoped to not affect the subsequent use_count() tests.
        {
            smart_ptr<X, shared_storage<_>, boost_ref<_> > sp2(wp2);
            test_is_nonzero(wp2.lock());
        }

        smart_ptr<Y, weak_storage<_>, boost_ref<_> > wp3 = dynamic_pointer_cast<Y>(wp2.lock());

        BOOST_TEST(wp3.use_count() == 1);
        test_shared(wp2, wp3);

        smart_ptr<X, weak_storage<_>, boost_ref<_> > wp4(wp3);

        BOOST_TEST(wp4.use_count() == 1);
        test_shared(wp2, wp4);

        wp1 = p2;
        test_is_zero(wp1.lock());

        wp1 = p4;
        wp1 = wp3;
        wp1 = wp2;

        BOOST_TEST(wp1.use_count() == 1);
        test_shared(wp1, wp2);

        smart_ptr<X, weak_storage<_>, boost_ref<_> > wp5;

        bool b1 = wp1 < wp5;
        bool b2 = wp5 < wp1;

        p5.reset();

        BOOST_TEST(wp1.use_count() == 0);
        BOOST_TEST(wp2.use_count() == 0);
        BOOST_TEST(wp3.use_count() == 0);

        // Test operator< stability for std::set< weak_ptr<> >
        // Thanks to Joe Gottman for pointing this out

        BOOST_TEST(b1 == (wp1 < wp5));
        BOOST_TEST(b2 == (wp5 < wp1));

        {
            // note that both get_object and release_object deal with int*
            smart_ptr<void, shared_storage<_>, boost_ref<_> > p6(get_object(), release_object);
        }
    }

    BOOST_TEST(cnt == 0);

    return report_errors();
}
