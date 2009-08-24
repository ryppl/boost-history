/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#include <boost/fusion/support/deduce_sequence.hpp>
#include <boost/fusion/mpl.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/config.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/static_assert.hpp>
#include <boost/ref.hpp>

using boost::reference_wrapper;
using boost::fusion::traits::deduce;
using boost::fusion::traits::deduce_sequence;

namespace fusion = boost::fusion;

template <class Args>
struct test_seq_ctor
{
    typename deduce_sequence<Args>::type fsq_args;

    test_seq_ctor(Args const& args)
      : fsq_args(sequence_assign(args))
    {}
};

#define TEST_SAME_TYPE(a,b) BOOST_STATIC_ASSERT(( boost::is_same< a, b >::value ))
#define TEST_SAME_ELEMENTS(a,b) BOOST_STATIC_ASSERT(( boost::mpl::equal< a, b >::type::value ))

typedef fusion::vector<int, int const, int &, int const&> args1;
#ifdef BOOST_NO_RVALUE_REFERENCES
typedef fusion::vector<int, int, int &, int> storable1;
#else
typedef fusion::vector<int, int, int &, int const&> storable1;
#endif
template struct test_seq_ctor<args1>;

typedef fusion::vector< reference_wrapper<int>&, reference_wrapper<int const>&,
    reference_wrapper<int> const&, reference_wrapper<int const> const& > args2;
typedef fusion::vector<int &, int const&, int &, int const&> storable2;
template struct test_seq_ctor<args2>;


typedef fusion::vector<int *, int const *, int const * const, int const * &, int const * const&> args3;
#ifdef BOOST_NO_RVALUE_REFERENCES
typedef fusion::vector<int *, int const *, int const *, int const * &, int const * > storable3;
#else
typedef fusion::vector<int *, int const *, int const *, int const * &, int const * const&> storable3;
#endif
template struct test_seq_ctor<args3>;

typedef fusion::vector<int(&)[2], int const(&)[2]> args4;
typedef args4 storable4;
template struct test_seq_ctor<args4>;

int main()
{
    //TODO !!!
    /*
    TEST_SAME_TYPE(deduce<int &>::type, int &);
    TEST_SAME_TYPE(deduce<int volatile &>::type, int volatile &);

    TEST_SAME_TYPE(deduce<int>::type, int);
#ifdef BOOST_NO_RVALUE_REFERENCES
    TEST_SAME_TYPE(deduce<int const&>::type, int);
    TEST_SAME_TYPE(deduce<int const volatile&>::type, int);
#else
    TEST_SAME_TYPE(deduce<int const&>::type, int const&);
    TEST_SAME_TYPE(deduce<int const volatile&>::type, int const volatile&);
#endif

    TEST_SAME_TYPE(deduce< reference_wrapper<int>& >::type, int &);
    TEST_SAME_TYPE(deduce< reference_wrapper<int const>& >::type, int const&);
    TEST_SAME_TYPE(deduce< reference_wrapper<int> const& >::type, int &);
    TEST_SAME_TYPE(deduce< reference_wrapper<int const> const& >::type, int const&);

    TEST_SAME_TYPE(deduce< int(&)[2] >::type, int(&)[2]);
    TEST_SAME_TYPE(deduce< int const (&)[2] >::type, int const (&)[2]);
    TEST_SAME_TYPE(deduce< int volatile (&)[2] >::type, int volatile (&)[2]);
    TEST_SAME_TYPE(deduce< int const volatile (&)[2] >::type, int const volatile (&)[2]);

    TEST_SAME_TYPE(deduce< void(*&)() >::type, void(*)());

    TEST_SAME_ELEMENTS(deduce_sequence<args1>::type,storable1);
    TEST_SAME_ELEMENTS(deduce_sequence<args2>::type,storable2);
    TEST_SAME_ELEMENTS(deduce_sequence<args3>::type,storable3);
    TEST_SAME_ELEMENTS(deduce_sequence<args4>::type,storable4);
    */

    return boost::report_errors();
}
