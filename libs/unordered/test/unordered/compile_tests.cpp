
//  Copyright Daniel James 2006. Use, modification, and distribution are
//  subject to the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#include <boost/concept_check.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/iterator/iterator_traits.hpp>
#include "../helpers/check_return_type.hpp"

#include <iostream>
#include <boost/detail/lightweight_test.hpp>
#include "../objects/minimal.hpp"

template <class T> void sink(T const&) {}

template <class X, class Key>
void unordered_set_test(X& r, Key const& k)
{
    typedef typename X::value_type value_type;
    typedef typename X::key_type key_type;

    BOOST_MPL_ASSERT((boost::is_same<value_type, key_type>));
}

template <class X, class Key, class T>
void unordered_map_test(X& r, Key const& k, T const& t)
{
    typedef typename X::value_type value_type;
    typedef typename X::key_type key_type;
    BOOST_MPL_ASSERT((boost::is_same<value_type, std::pair<key_type const, T> >));
}

template <class X, class T>
void unordered_unique_test(X& r, T const& t)
{
    typedef typename X::iterator iterator;
    test::check_return_type<std::pair<iterator, bool> >::equals(r.insert(t));
}

template <class X, class T>
void unordered_equivalent_test(X& r, T const& t)
{
    typedef typename X::iterator iterator;
    test::check_return_type<iterator>::equals(r.insert(t));
}

template <class X, class Key, class T, class Hash, class Pred>
void unordered_test(X& ref, Key& k, T& t, Hash& hf, Pred& eq)
{
    typedef typename X::key_type key_type;
    typedef typename X::hasher hasher;
    typedef typename X::key_equal key_equal;
    typedef typename X::size_type size_type;

    typedef typename X::iterator iterator;
    typedef typename X::const_iterator const_iterator;
    typedef typename X::local_iterator local_iterator;
    typedef typename X::const_local_iterator const_local_iterator;

    typedef typename boost::BOOST_ITERATOR_CATEGORY<iterator>::type iterator_category;
    typedef typename boost::iterator_difference<iterator>::type iterator_difference;
    typedef typename boost::iterator_pointer<iterator>::type iterator_pointer;
    typedef typename boost::iterator_reference<iterator>::type iterator_reference;

    typedef typename boost::BOOST_ITERATOR_CATEGORY<local_iterator>::type local_iterator_category;
    typedef typename boost::iterator_difference<local_iterator>::type local_iterator_difference;
    typedef typename boost::iterator_pointer<local_iterator>::type local_iterator_pointer;
    typedef typename boost::iterator_reference<local_iterator>::type local_iterator_reference;

    typedef typename boost::BOOST_ITERATOR_CATEGORY<const_iterator>::type const_iterator_category;
    typedef typename boost::iterator_difference<const_iterator>::type const_iterator_difference;
    typedef typename boost::iterator_pointer<const_iterator>::type const_iterator_pointer;
    typedef typename boost::iterator_reference<const_iterator>::type const_iterator_reference;

    typedef typename boost::BOOST_ITERATOR_CATEGORY<const_local_iterator>::type const_local_iterator_category;
    typedef typename boost::iterator_difference<const_local_iterator>::type const_local_iterator_difference;
    typedef typename boost::iterator_pointer<const_local_iterator>::type const_local_iterator_pointer;
    typedef typename boost::iterator_reference<const_local_iterator>::type const_local_iterator_reference;

    BOOST_MPL_ASSERT((boost::is_same<Key, key_type>));
    boost::function_requires<boost::CopyConstructibleConcept<key_type> >();
    boost::function_requires<boost::AssignableConcept<key_type> >();

    BOOST_MPL_ASSERT((boost::is_same<Hash, hasher>));
    test::check_return_type<std::size_t>::equals(hf(k));

    BOOST_MPL_ASSERT((boost::is_same<Pred, key_equal>));
    test::check_return_type<bool>::convertible(eq(k, k));
    // TODO: Pred is an equivalence relation. Doesn't really matter for these
    // tests.

    boost::function_requires<boost::InputIteratorConcept<local_iterator> >();
    BOOST_MPL_ASSERT((boost::is_same<local_iterator_category, iterator_category>));
    BOOST_MPL_ASSERT((boost::is_same<local_iterator_difference, iterator_difference>));
    BOOST_MPL_ASSERT((boost::is_same<local_iterator_pointer, iterator_pointer>));
    BOOST_MPL_ASSERT((boost::is_same<local_iterator_reference, iterator_reference>));

    boost::function_requires<boost::InputIteratorConcept<const_local_iterator> >();
    BOOST_MPL_ASSERT((boost::is_same<const_local_iterator_category, const_iterator_category>));
    BOOST_MPL_ASSERT((boost::is_same<const_local_iterator_difference, const_iterator_difference>));
    BOOST_MPL_ASSERT((boost::is_same<const_local_iterator_pointer, const_iterator_pointer>));
    BOOST_MPL_ASSERT((boost::is_same<const_local_iterator_reference, const_iterator_reference>));

    X(10, hf, eq);
    X a(10, hf, eq);
    X(10, hf);
    X a2(10, hf);
    X(10);
    X a3(10);
    X();
    X a4;

    typename X::value_type* i = 0;
    typename X::value_type* j = 0;

    X(i, j, 10, hf, eq);
    X a5(i, j, 10, hf, eq);
    X(i, j, 10, hf);
    X a6(i, j, 10, hf);
    X(i, j, 10);
    X a7(i, j, 10);
    X(i, j);
    X a8(i, j);

    X const b;
    sink(X(b));
    X a9(b);
    a = b;

    test::check_return_type<hasher>::equals(b.hash_function());
    test::check_return_type<key_equal>::equals(b.key_eq());

    iterator q = a.begin();
    const_iterator r = a.begin();
    test::check_return_type<iterator>::equals(a.insert(q, t));
    test::check_return_type<const_iterator>::equals(a.insert(r, t));

    // TODO: void return?
    a.insert(i, j);
    test::check_return_type<size_type>::equals(a.erase(k));

    BOOST_TEST(a.empty());
    if(a.empty()) {
        a.insert(t);
        q = a.begin();
        test::check_return_type<iterator>::equals(a.erase(q));
    }

    BOOST_TEST(a.empty());
    if(a.empty()) {
        a.insert(t);
        r = a.begin();
        test::check_return_type<const_iterator>::equals(a.erase(r));
    }

    iterator q1 = a.begin(), q2 = a.end();
    test::check_return_type<iterator>::equals(a.erase(q1, q2));

    const_iterator r1 = a.begin(), r2 = a.end();
    test::check_return_type<const_iterator>::equals(a.erase(r1, r2));

    // TODO: void return?
    a.clear();

    test::check_return_type<iterator>::equals(a.find(k));
    test::check_return_type<const_iterator>::equals(b.find(k));
    test::check_return_type<size_type>::equals(b.count(k));
    test::check_return_type<std::pair<iterator, iterator> >::equals(
            a.equal_range(k));
    test::check_return_type<std::pair<const_iterator, const_iterator> >::equals(
            b.equal_range(k));
    test::check_return_type<size_type>::equals(b.bucket_count());
    test::check_return_type<size_type>::equals(b.max_bucket_count());
    test::check_return_type<size_type>::equals(b.bucket(k));
    test::check_return_type<size_type>::equals(b.bucket_size(0));

    test::check_return_type<local_iterator>::equals(a.begin(0));
    test::check_return_type<const_local_iterator>::equals(b.begin(0));
    test::check_return_type<local_iterator>::equals(a.end(0));
    test::check_return_type<const_local_iterator>::equals(b.end(0));

    test::check_return_type<float>::equals(b.load_factor());
    test::check_return_type<float>::equals(b.max_load_factor());
    a.max_load_factor((float) 2.0);
    a.rehash(100);
}

void test1()
{
    boost::hash<int> hash;
    std::equal_to<int> equal_to;
    int value = 0;
    std::pair<int const, int> map_value(0, 0);

    std::cout<<"Test unordered_set.\n";

    boost::unordered_set<int> set;
    
    unordered_unique_test(set, value);
    unordered_set_test(set, value);
    unordered_test(set, value, value, hash, equal_to);

    std::cout<<"Test unordered_multiset.\n";

    boost::unordered_multiset<int> multiset;
    
    unordered_equivalent_test(multiset, value);
    unordered_set_test(multiset, value);
    unordered_test(multiset, value, value, hash, equal_to);

    std::cout<<"Test unordered_map.\n";

    boost::unordered_map<int, int> map;

    unordered_unique_test(map, map_value);
    unordered_map_test(map, value, value);
    unordered_test(map, value, map_value, hash, equal_to);

    std::cout<<"Test unordered_multimap.\n";

    boost::unordered_multimap<int, int> multimap;

    unordered_equivalent_test(multimap, map_value);
    unordered_map_test(multimap, value, value);
    unordered_test(multimap, value, map_value, hash, equal_to);
}

void test2()
{
    test::minimal::assignable assignable
        = test::minimal::assignable::create();
    test::minimal::copy_constructible copy_constructible
        = test::minimal::copy_constructible::create();
    test::minimal::hash<test::minimal::assignable> hash
        = test::minimal::hash<test::minimal::assignable>::create();
    test::minimal::equal_to<test::minimal::assignable> equal_to
        = test::minimal::equal_to<test::minimal::assignable>::create();

    typedef std::pair<test::minimal::assignable const,
            test::minimal::copy_constructible> map_value_type;
    map_value_type map_value(assignable, copy_constructible);

    std::cout<<"Test unordered_set.\n";

    boost::unordered_set<
        test::minimal::assignable,
        test::minimal::hash<test::minimal::assignable>,
        test::minimal::equal_to<test::minimal::assignable>,
        test::minimal::allocator<test::minimal::assignable> > set;

    unordered_unique_test(set, assignable);
    unordered_set_test(set, assignable);
    unordered_test(set, assignable, assignable, hash, equal_to);

    std::cout<<"Test unordered_multiset.\n";

    boost::unordered_multiset<
        test::minimal::assignable,
        test::minimal::hash<test::minimal::assignable>,
        test::minimal::equal_to<test::minimal::assignable>,
        test::minimal::allocator<test::minimal::assignable> > multiset;

    unordered_equivalent_test(multiset, assignable);
    unordered_set_test(multiset, assignable);
    unordered_test(multiset, assignable, assignable, hash, equal_to);

    std::cout<<"Test unordered_map.\n";

    boost::unordered_map<
        test::minimal::assignable,
        test::minimal::copy_constructible,
        test::minimal::hash<test::minimal::assignable>,
        test::minimal::equal_to<test::minimal::assignable>,
        test::minimal::allocator<map_value_type> > map;

    unordered_unique_test(map, map_value);
    unordered_map_test(map, assignable, copy_constructible);
    unordered_test(map, assignable, map_value, hash, equal_to);

    std::cout<<"Test unordered_multimap.\n";

    boost::unordered_multimap<
        test::minimal::assignable,
        test::minimal::copy_constructible,
        test::minimal::hash<test::minimal::assignable>,
        test::minimal::equal_to<test::minimal::assignable>,
        test::minimal::allocator<map_value_type> > multimap;

    unordered_equivalent_test(multimap, map_value);
    unordered_map_test(multimap, assignable, copy_constructible);
    unordered_test(multimap, assignable, map_value, hash, equal_to);
}

int main() {
    test1();
    test2();

    return boost::report_errors();
}
