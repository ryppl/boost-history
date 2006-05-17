
//  Copyright Daniel James 2005-2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/concept_check.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include <boost/limits.hpp>
#include "../helpers/check_return_type.hpp"

typedef long double comparison_type;

template <class T> void sink(T const&) {}

template <class X, class T>
void container_test(X& r, T& value)
{
    typedef typename X::iterator iterator;
    typedef typename X::const_iterator const_iterator;
    typedef typename X::difference_type difference_type;
    typedef typename X::size_type size_type;

    typedef typename boost::iterator_value<iterator>::type iterator_value_type;
    typedef typename boost::iterator_value<const_iterator>::type const_iterator_value_type;
    typedef typename boost::iterator_difference<iterator>::type iterator_difference_type;
    typedef typename boost::iterator_difference<const_iterator>::type const_iterator_difference_type;

    typedef typename X::value_type value_type;
    typedef typename X::reference reference;
    typedef typename X::const_reference const_reference;

    // value_type

    BOOST_MPL_ASSERT((boost::is_same<T, value_type>));
    boost::function_requires<boost::CopyConstructibleConcept<X> >();

    // reference_type / const_reference_type

    // TODO: 'lvalue of T'
    BOOST_MPL_ASSERT((boost::is_same<T&, reference>));
    // TODO: 'const lvalue of T'
    BOOST_MPL_ASSERT((boost::is_same<T const&, const_reference>));

    // iterator

    boost::function_requires<boost::InputIteratorConcept<iterator> >();
    BOOST_MPL_ASSERT((boost::is_same<T, iterator_value_type>));
    BOOST_MPL_ASSERT((boost::is_convertible<iterator, const_iterator>));

    // const_iterator

    // TODO: Test that it's a constant iterator?
    boost::function_requires<boost::InputIteratorConcept<const_iterator> >();
    BOOST_MPL_ASSERT((boost::is_same<T, const_iterator_value_type>));

    // difference_type

    BOOST_MPL_ASSERT((boost::mpl::bool_<
                std::numeric_limits<difference_type>::is_signed>));
    BOOST_MPL_ASSERT((boost::mpl::bool_<
                std::numeric_limits<difference_type>::is_integer>));
    BOOST_MPL_ASSERT((boost::is_same<difference_type,
                iterator_difference_type>));
    BOOST_MPL_ASSERT((boost::is_same<difference_type,
                const_iterator_difference_type>));

    // size_type

    BOOST_MPL_ASSERT_NOT((boost::mpl::bool_<
                std::numeric_limits<size_type>::is_signed>));
    BOOST_MPL_ASSERT((boost::mpl::bool_<
                std::numeric_limits<size_type>::is_integer>));

    // size_type can represent any non-negative value type of difference_type
    // I'm not sure about either of these tests...
    size_type max_diff((std::numeric_limits<difference_type>::max)());
    difference_type converted_diff(max_diff);
    BOOST_TEST((std::numeric_limits<difference_type>::max)()
            == converted_diff);

    BOOST_TEST(
        static_cast<comparison_type>(
            (std::numeric_limits<size_type>::max)()) >
        static_cast<comparison_type>(
            (std::numeric_limits<difference_type>::max)()));

    // I don't test the runtime post-conditions here.
    X u;
    BOOST_TEST(u.size() == 0);
    BOOST_TEST(X().size() == 0);

    X a,b;

    sink(X(a));
    X u2(a);
    X u3 = a;

    X* ptr = new X();
    X& a1 = *ptr;
    (&a1)->~X();

    X const a_const;
    test::check_return_type<iterator>::equals(a.begin());
    test::check_return_type<const_iterator>::equals(a_const.begin());
    test::check_return_type<const_iterator>::equals(a.cbegin());
    test::check_return_type<const_iterator>::equals(a_const.cbegin());
    test::check_return_type<iterator>::equals(a.end());
    test::check_return_type<const_iterator>::equals(a_const.end());
    test::check_return_type<const_iterator>::equals(a.cend());
    test::check_return_type<const_iterator>::equals(a_const.cend());

    // No tests for ==, != since they're not required for unordered containers.

    a.swap(b);
    test::check_return_type<X>::equals_ref(r = a);
    test::check_return_type<size_type>::equals(a.size());
    test::check_return_type<size_type>::equals(a.max_size());
    test::check_return_type<bool>::convertible(a.empty());
}
