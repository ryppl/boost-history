//  (C) Copyright David Abrahams 2000. Permission to copy, use,
//  modify, sell and distribute this software is granted provided this
//  copyright notice appears in all copies. This software is provided
//  "as is" without express or implied warranty, and with no claim as
//  to its suitability for any purpose.

// jep
#include <boost/std.iterator.hpp>
// jep.end

#ifdef JEP_SGI_TREE_TEST
#include <stl_tree.h>
#define JEP_SET_TYPE boost::vector_set<int, Compare, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, Compare, std::allocator<int> > >
#define JEP_ANOTHER_SET_TYPE boost::vector_set<Elt, Comp, Alloc, _Rb_tree<Elt, Elt, _Identity<Elt>, Comp, Alloc> >
#define JEP_INT_SET_TYPE boost::vector_set<int, std::less<int>, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, std::less<int>, std::allocator<int> > >
#define JEP_METERED_SET_TYPE boost::vector_set<int, MeteredCompare, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, MeteredCompare, std::allocator<int> > >
#define JEP_MULTISET_TYPE boost::vector_multiset<int, Compare, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, Compare, std::allocator<int> > >
#define JEP_ANOTHER_MULTISET_TYPE boost::vector_multiset<Elt, Comp, Alloc, _Rb_tree<Elt, Elt, _Identity<Elt>, Comp, Alloc> >
#define JEP_INT_MULTISET_TYPE boost::vector_multiset<int, std::less<int>, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, std::less<int>, std::allocator<int> > >
#define JEP_METERED_MULTISET_TYPE boost::vector_multiset<int, MeteredCompare, std::allocator<int>, _Rb_tree<int, int, _Identity<int>, MeteredCompare, std::allocator<int> > >
#elif defined JEP_DEQUE_TEST
#include <deque>
#define JEP_SET_TYPE boost::vector_set<int, Compare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, Compare, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#define JEP_ANOTHER_SET_TYPE boost::vector_set<Elt, Comp, Alloc, boost::associative_vector<int, int, boost::identity<int>, Comp, Alloc, std::deque<int, Alloc > > >
#define JEP_INT_SET_TYPE boost::vector_set<int, std::less<int>, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#define JEP_METERED_SET_TYPE boost::vector_set<int, MeteredCompare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#define JEP_MULTISET_TYPE boost::vector_multiset<int, Compare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, Compare, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#define JEP_ANOTHER_MULTISET_TYPE boost::vector_multiset<Elt, Comp, Alloc, boost::associative_vector<int, int, boost::identity<int>, Comp, Alloc, std::deque<int, Alloc > > >
#define JEP_INT_MULTISET_TYPE boost::vector_multiset<int, std::less<int>, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#define JEP_METERED_MULTISET_TYPE boost::vector_multiset<int, MeteredCompare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int>, std::deque<int, std::allocator<int> > > >
#elif defined JEP_LIST_TEST
#include <list>
#define JEP_SET_TYPE boost::vector_set<int, Compare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, Compare, std::allocator<int>, std::list<int, std::allocator<int> > > >
#define JEP_ANOTHER_SET_TYPE boost::vector_set<Elt, Comp, Alloc, boost::associative_vector<int, int, boost::identity<int>, Comp, Alloc, std::list<int, Alloc > > >
#define JEP_INT_SET_TYPE boost::vector_set<int, std::less<int>, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int>, std::list<int, std::allocator<int> > > >
#define JEP_METERED_SET_TYPE boost::vector_set<int, MeteredCompare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int>, std::list<int, std::allocator<int> > > >
#define JEP_MULTISET_TYPE boost::vector_multiset<int, Compare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, Compare, std::allocator<int>, std::list<int, std::allocator<int> > > >
#define JEP_ANOTHER_MULTISET_TYPE boost::vector_multiset<Elt, Comp, Alloc, boost::associative_vector<int, int, boost::identity<int>, Comp, Alloc, std::list<int, Alloc > > >
#define JEP_INT_MULTISET_TYPE boost::vector_multiset<int, std::less<int>, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int>, std::list<int, std::allocator<int> > > >
#define JEP_METERED_MULTISET_TYPE boost::vector_multiset<int, MeteredCompare, std::allocator<int>, boost::associative_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int>, std::list<int, std::allocator<int> > > >
#elif defined JEP_INDIRECT_TEST
#include <boost/associative_indirect_vector.hpp>
#define JEP_SET_TYPE boost::vector_set<int, Compare, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, Compare, std::allocator<int> > >
#define JEP_ANOTHER_SET_TYPE boost::vector_set<Elt, Comp, Alloc, boost::associative_indirect_vector<int, int, boost::identity<int>, Comp, Alloc> >
#define JEP_INT_SET_TYPE boost::vector_set<int, std::less<int>, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int> > >
#define JEP_METERED_SET_TYPE boost::vector_set<int, MeteredCompare, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int> > >
#define JEP_MULTISET_TYPE boost::vector_multiset<int, Compare, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, Compare, std::allocator<int> > >
#define JEP_ANOTHER_MULTISET_TYPE boost::vector_multiset<Elt, Comp, Alloc, boost::associative_indirect_vector<int, int, boost::identity<int>, Comp, Alloc> >
#define JEP_INT_MULTISET_TYPE boost::vector_multiset<int, std::less<int>, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, std::less<int>, std::allocator<int> > >
#define JEP_METERED_MULTISET_TYPE boost::vector_multiset<int, MeteredCompare, std::allocator<int>, boost::associative_indirect_vector<int, int, boost::identity<int>, MeteredCompare, std::allocator<int> > >
#else
#define JEP_SET_TYPE boost::vector_set<int, Compare>
#define JEP_ANOTHER_SET_TYPE boost::vector_set<Elt, Comp, Alloc>
#define JEP_INT_SET_TYPE boost::vector_set<int>
#define JEP_METERED_SET_TYPE boost::vector_set<int, MeteredCompare>
#define JEP_MULTISET_TYPE boost::vector_multiset<int, Compare>
#define JEP_ANOTHER_MULTISET_TYPE boost::vector_multiset<Elt, Comp, Alloc>
#define JEP_INT_MULTISET_TYPE boost::vector_multiset<int>
#define JEP_METERED_MULTISET_TYPE boost::vector_multiset<int, MeteredCompare>
#endif

#include <boost/vector_set.hpp>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>
// jep
// #include <limits>
#include <limits.h>
// jep.end
#include <boost/utility.hpp>

struct Compare : std::binary_function<int, int, bool>
{
    enum Direction { fwd, rev };
    Compare(Direction dir = fwd) : m_dir(dir) {}
    bool operator()(int x, int y) const { return (m_dir == fwd) ? x < y : y < x; }
    Direction m_dir;
};

struct MeteredCompare : std::binary_function<int, int, bool>
{
    static int count;
    bool operator()(int lhs, int rhs) const { ++ count; return lhs < rhs; }
};
int MeteredCompare::count;

template <class Container>
Container sorted(Container c)
{
    Container result(c);
    std::sort(result.begin(), result.end());
    return result;
}

template <class Container>
Container uniqued(Container c)
{
    Container result(c);
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

template <class X, class Y>
struct tied
{
    tied(X& x, Y& y) : m_x(x), m_y(y) {}
    
    template <class T1, class T2>
    void operator=(const std::pair<T1,T2>& p)
        { m_x = p.first; m_y = p.second; }
    mutable X& m_x;
    mutable Y& m_y;
};

template <class X, class Y>
tied<X, Y> tie(X& x, Y& y) { return tied<X,Y>(x,y); }

void assert_sorted(const JEP_SET_TYPE& s)
{
    if (s.empty())
        return;

    Compare cmp = s.key_comp();
    
    JEP_SET_TYPE::const_iterator p = s.begin();
    for (++ p; p != s.end(); ++p)
        assert(cmp(*p-1, *p));
}

void assert_sorted(const JEP_MULTISET_TYPE& s)
{
    if (s.empty())
        return;

    Compare cmp = s.key_comp();
    
    JEP_MULTISET_TYPE::const_iterator p = s.begin();
    for (++ p; p != s.end(); ++p)
        assert(!cmp(*p, *p-1));
}

template <class Elt, class Comp, class Alloc>
void assert_inserted(JEP_ANOTHER_MULTISET_TYPE& c, Elt x)
{
    const std::size_t size = c.size();
    const JEP_ANOTHER_MULTISET_TYPE::iterator p = c.insert(x);
    assert_sorted(c);
    assert(c.size() == size + 1);
    assert(*p == x);
}

template <class Elt, class Comp, class Alloc>
void assert_inserted(JEP_ANOTHER_SET_TYPE& c, Elt x)
{
    const std::size_t size = c.size();
    const std::pair<JEP_ANOTHER_SET_TYPE::iterator, bool> p = c.insert(x);
    assert(c.size() == size + 1);
    assert(p.second);
    assert(*p.first == x);
}

template <class Elt, class Comp, class Alloc>
void assert_not_inserted(JEP_ANOTHER_SET_TYPE& c, Elt x)
{
    const std::size_t size = c.size();
    const std::pair<JEP_ANOTHER_SET_TYPE::iterator, bool> p = c.insert(x);
    assert(c.size() == size);
    assert(!p.second);
    assert(*p.first == x);
}

template <class Container>
void assert_inserted(Container& c, typename Container::const_iterator pos, typename Container::value_type x)
{
    const std::size_t size = c.size();
    typename Container::iterator p = c.insert(pos, x);
    assert_sorted(c);
    assert(c.size() == size + 1);
    assert(*p == x);
}

template <class Container>
void assert_not_inserted(Container& c, typename Container::const_iterator pos, const typename Container::value_type& x)
{
    const std::size_t size = c.size();
    typename Container::iterator p = c.insert(pos, x);
    assert(c.size() == size);
    assert(*p == x);
}

template <class Container>
bool sorted_contains(const Container& c, typename Container::value_type x)
{
    return std::binary_search(c.begin(), c.end(), x);
}

void test_set(const std::vector<int>& input)
{
    JEP_INT_SET_TYPE s0;
    s0.size();
    assert(s0.empty());

    typedef JEP_SET_TYPE Set;
    Set s1(input.begin(), input.end());

    // Test the version that takes a compare parameter
    Set s2(input.begin(), input.end(), Compare(Compare::rev) );
    assert(s1.size() == s2.size());
    assert(std::equal(s1.begin(), s1.end(), s2.rbegin()));

    const std::vector<int> sorted_input = sorted(input);
    const std::vector<int> unique_input = uniqued(sorted_input);

    std::size_t unique_size = unique_input.size();
    std::size_t s1size = s1.size();
    assert(unique_size == s1size);
    assert(std::equal(s1.begin(), s1.end(), unique_input.begin()));
    assert(std::equal(s2.begin(), s2.end(), unique_input.rbegin()));

    // I'm not going to test the allocator parameter; I don't know how to (conveniently).

    // copy constructor
    Set s1copy(s1);
    assert(std::equal(s1copy.begin(), s1copy.end(), unique_input.begin()));
    assert(s1copy == s1);

    // assignment
    s1copy.clear();
    assert(s1copy.size() == 0);
    assert(!(s1copy == s1));
    
    s1copy = s1;
    assert(s1copy == s1);

    // accessors
    assert(s1.key_comp()(3,4));
    assert(s2.key_comp()(4,3));
    assert(s1.value_comp()(3,4));
    assert(s2.value_comp()(4,3));
    std::allocator<int> alloc = s1.get_allocator();
    
    // non-const iterators already tested.
    const Set s2copy(s2);
    assert(unique_input.size() == s2copy.size());
    assert(std::equal(unique_input.rbegin(), unique_input.rend(), s2copy.begin()));
    assert(std::equal(s2copy.begin(), s2copy.end(), unique_input.rbegin()));
    assert(std::equal(unique_input.begin(), unique_input.end(), s2copy.rbegin()));
    assert(std::equal(s2copy.rbegin(), s2copy.rend(), unique_input.begin()));

    std::size_t max_size = s1.max_size();
    assert(max_size != 0);

    // okay, if rand() happened to generate only identical #s, this assert will
    // fail. But if we have such a degenerate sequence, there's not much point
    // in running this test anyway.
    assert(s1copy != s2);

    // swaps
    s1.swap(s2);
    assert(s1copy == s2);
    s1.swap(s2);
    assert(s1copy == s1);

    // inserting a duplicate should be a no-op.
    assert_not_inserted(s1, s1.front());
    assert(s1copy == s1);

    // rand() never returns a negative number, so this should actually work
    assert_inserted(s1, -1);
    assert(s1.front() == -1);

    // Test the other extreme
    int max_elt = s1.back();
    assert_not_inserted(s1, max_elt);

    assert_inserted(s1, max_elt + 1);

    // Find a likely value in the middle to insert.
    int mid_elt = max_elt / 2;
    bool already_in = sorted_contains(s1, mid_elt);
    if (already_in)
        assert_not_inserted(s1, mid_elt);
    else
        assert_inserted(s1, mid_elt);

    assert_sorted(s1);
    
    // inserting with a hint
    assert_not_inserted(s1, s1.begin(), -1);
    assert_inserted(s1, s1.begin(), -3);

    // insert with a hint which is just *after* the place where the element
    // belongs.  This tests the standard behavior, which is supposed to avoid a
    // binary search, just as when the hint points at the location of the new
    // element (our extension, but useful).

    Set::iterator testPoint;

//    assert_inserted(s1, s1.begin() + 2, -2);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    assert_inserted(s1, testPoint, -2);
    
    assert_sorted(s1);

    assert_not_inserted(s1, s1.end(), s1.back());
    
    max_elt = s1.back();
    assert_inserted(s1, s1.end(), max_elt + 2);
    assert_inserted(s1, s1.end(), max_elt + 1);

    // try some incorrect hints
    int min_value = s1.front();
    assert_not_inserted(s1, s1.end(), min_value);
    assert_inserted(s1, s1.end(), min_value - 1);

    int max_value = s1.front();    
    assert_not_inserted(s1, s1.begin(), max_value);
    assert_inserted(s1, s1.begin(), max_value - 1);

//    assert_not_inserted(s1, s1.begin() + 1, mid_elt);
    testPoint = s1.begin();
    std::advance(testPoint, 1);
    assert_not_inserted(s1, testPoint, mid_elt);
//    assert_not_inserted(s1, s1.begin() + 2, mid_elt);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    assert_not_inserted(s1, testPoint, mid_elt);


    already_in = sorted_contains(s1, mid_elt + 1);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    if (already_in)
//        assert_not_inserted(s1, s1.begin() + 2, mid_elt + 1);
        assert_not_inserted(s1, testPoint, mid_elt + 1);
    else
//        assert_inserted(s1, s1.begin() + 2, mid_elt + 1);
        assert_inserted(s1, testPoint, mid_elt + 1);

    assert_sorted(s1);

    // insert a range

    // first a wholly redundant one
    std::size_t size = s1.size();
    s1.insert(input.begin(), input.end()); // duplicates, so this is a no-op
    assert(size == s1.size());

    // now a partially redundant one
    std::vector<int> more_input;
    more_input.reserve(100);
    std::size_t new_item_cnt = 0;
    for (int i = 0; i < 100; ++i)
    {
        if (std::rand() % 3 == 0)
            more_input.push_back(s1[std::rand() % s1.size()]);
        else
            more_input.push_back(std::rand());
        
        if (!sorted_contains(s1, more_input.back())
            && std::find(more_input.begin(), more_input.end() - 1, more_input.back())
               == more_input.end() - 1)
            ++new_item_cnt;
    }
    s1.insert(more_input.begin(), more_input.end());
    assert(s1.size() == size + new_item_cnt);

    assert_sorted(s1);

    // erase an iterator
    Set::iterator p = s1.insert(999).first;
    assert(sorted_contains(s1, 999));
    s1.erase(p);
    assert(!sorted_contains(s1, 999));

    // erase a key
    s1.insert(999);
    assert(sorted_contains(s1, 999));
    std::size_t erased = s1.erase(999);
    assert(erased == 1);
    assert(!sorted_contains(s1, 999));
    
    erased = s1.erase(999);
    assert(erased == 0);

    // erase a range
    Set::iterator first = std::lower_bound(s1.begin(), s1.end(), 500);
    Set::iterator last = std::upper_bound(s1.begin(), s1.end(), 1000);
    s1.erase(first, last);
    assert_sorted(s1);
    assert(std::lower_bound(s1.begin(), s1.end(), 500)
           == std::upper_bound(s1.begin(), s1.end(), 1000));

    // just in case we killed too much of it.
    while (s1.size() < 5)
        s1.insert(std::rand());
    
    s2.clear();
    assert(s2.size() == 0);

    // find
    Set::iterator pos = s1.find(s1.front());
    assert(pos == s1.begin());
    pos = s1.find(s1.back());
    assert(pos == boost::prior(s1.end()));

    const int range = static_cast<int>(s1.back() - s1.front());
    mid_elt = s1.front() + range / 2;
    pos = s1.find(mid_elt);
    if (pos != s1.end())
        assert(*pos == mid_elt);
    else
        assert(!sorted_contains(s1, mid_elt));

    // count, lower_bound, upper_bound, equal_range
    for (int i2 = 0; i2 < 20; ++i2)
    {
        int x = std::rand() % range + s1.front();
        std::size_t cnt = s1.count(x);
        assert(cnt == 0 || cnt == 1);
        assert((cnt == 1) == sorted_contains(s1, x));

        pos = s1.lower_bound(x);
        assert(pos == s1.end() || *pos >= x);
        assert(pos == s1.begin() || *--pos < x);

        pos = s1.upper_bound(x);
        assert(pos == s1.end() || *pos > x);
        assert(pos == s1.begin() || *--pos <= x);

        std::pair<Set::iterator, Set::iterator> range = s1.equal_range(x);
        assert(cnt == static_cast<std::size_t>(
                std::distance(range.first, range.second)));
    }

    // comparisons
    assert(s1 != s2);
    assert(!(s1 == s2));
    s2 = s1;
    assert(s1 == s2);
    assert(!(s1 != s2));
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert(s1 >= s2);
    assert(s1 <= s2);
    s2.erase(s2[0]);
    s2.insert(s1[0] - 1);
    assert(!(s1 < s2));
    assert(s1 > s2);
    assert(!(s1 <= s2));
    assert(s1 >= s2);

}

void test_multiset(const std::vector<int>& input)
{
    // default constructor
    JEP_INT_MULTISET_TYPE s0;
    s0.size();
    assert(s0.empty());

    typedef JEP_MULTISET_TYPE Set;
    Set s1(input.begin(), input.end());

    // Test the version that takes a compare parameter
    Set s2(input.begin(), input.end(), Compare(Compare::rev) );
    assert(s1.size() == s2.size());
    assert(std::equal(s1.begin(), s1.end(), s2.rbegin()));

    const std::vector<int> sorted_input = sorted(input);

    std::size_t sorted_size = sorted_input.size();
    std::size_t s1size = s1.size();
    assert(sorted_size == s1size);
    assert(std::equal(s1.begin(), s1.end(), sorted_input.begin()));
    assert(std::equal(s2.begin(), s2.end(), sorted_input.rbegin()));

    // I'm not going to test the allocator parameter; I don't know how to (conveniently).

    // copy constructor
    Set s1copy(s1);
    assert(std::equal(s1copy.begin(), s1copy.end(), sorted_input.begin()));
    assert(s1copy == s1);

    // assignment
    s1copy.clear();
    assert(s1copy.size() == 0);
    assert(!(s1copy == s1));
    
    s1copy = s1;
    assert(s1copy == s1);

    // accessors
    assert(s1.key_comp()(3,4));
    assert(s2.key_comp()(4,3));
    assert(s1.value_comp()(3,4));
    assert(s2.value_comp()(4,3));
    std::allocator<int> alloc = s1.get_allocator();
    
    // non-const iterators already tested.
    const Set s2copy(s2);
    assert(sorted_input.size() == s2copy.size());
    assert(std::equal(sorted_input.rbegin(), sorted_input.rend(), s2copy.begin()));
    assert(std::equal(s2copy.begin(), s2copy.end(), sorted_input.rbegin()));
    assert(std::equal(sorted_input.begin(), sorted_input.end(), s2copy.rbegin()));
    assert(std::equal(s2copy.rbegin(), s2copy.rend(), sorted_input.begin()));

    std::size_t max_size = s1.max_size();
    assert(max_size != 0);

    // okay, if rand() happened to generate only identical #s, this assert will
    // fail. But if we have such a degenerate sequence, there's not much point
    // in running this test anyway.
    assert(s1copy != s2);

    // swaps
    s1.swap(s2);
    assert(s1copy == s2);
    s1.swap(s2);
    assert(s1copy == s1);

    // inserting a duplicate should work.
    assert_inserted(s1, s1.front());
    assert(s1copy != s1);
    
    // rand() never returns a negative number, so this should actually work
    assert_inserted(s1, -1);
    assert(s1.front() == -1);

    // Test the other extreme
    int max_elt = s1.back();
    assert_inserted(s1, max_elt);

    assert_inserted(s1, max_elt + 1);

    // Find a likely value in the middle to insert.
    int mid_elt = max_elt / 2;
    assert_inserted(s1, mid_elt);

    assert_sorted(s1);
    
    // inserting with a hint
    assert_inserted(s1, s1.begin(), -1);
    assert_sorted(s1);
    assert_inserted(s1, s1.begin(), -3);
    assert_sorted(s1);

    // insert with a hint which is just *after* the place where the element
    // belongs.  This tests the standard behavior, which is supposed to avoid a
    // binary search, just as when the hint points at the location of the new
    // element (our extension, but useful).
    Set::iterator testPoint;

//    assert_inserted(s1, s1.begin() + 2, -2);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    assert_inserted(s1, testPoint, -2);
    
    assert_sorted(s1);

    assert_inserted(s1, s1.end(), s1.back());
    
    max_elt = s1.back();
    assert_inserted(s1, s1.end(), max_elt + 2);
    assert_inserted(s1, s1.end(), max_elt + 1);

    // try some incorrect hints
    int min_value = s1.front();
    assert_inserted(s1, s1.end(), min_value);
    assert_inserted(s1, s1.end(), min_value - 1);

    int max_value = s1.front();    
    assert_inserted(s1, s1.begin(), max_value);
    assert_inserted(s1, s1.begin(), max_value - 1);

//    assert_inserted(s1, s1.begin() + 1, mid_elt);
    testPoint = s1.begin();
    std::advance(testPoint, 1);
    assert_inserted(s1, testPoint, mid_elt);
//    assert_inserted(s1, s1.begin() + 2, mid_elt);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    assert_inserted(s1, testPoint, mid_elt);


//    assert_inserted(s1, s1.begin() + 2, mid_elt + 1);
    testPoint = s1.begin();
    std::advance(testPoint, 2);
    assert_inserted(s1, testPoint, mid_elt + 1);
    assert_sorted(s1);

    // insert a range
    // a partially redundant one
    std::vector<int> more_input;
    more_input.reserve(100);
    std::size_t size = s1.size();
    std::size_t new_item_cnt = 0;
    for (int i = 0; i < 100; ++i)
    {
        if (std::rand() % 3 == 0)
            more_input.push_back(s1[std::rand() % s1.size()]);
        else
            more_input.push_back(std::rand());
        
        ++new_item_cnt;
    }
    s1.insert(more_input.begin(), more_input.end());
    assert(s1.size() == size + new_item_cnt);

    assert_sorted(s1);

    // erase an iterator
    std::size_t contained = s1.count(999);
    Set::iterator p = s1.insert(999);
    assert(s1.count(999) == contained + 1);
    s1.erase(p);
    assert(s1.count(999) == contained);

    // erase a key
    s1.insert(999);
    assert(s1.count(999) == contained + 1);
    std::size_t erased = s1.erase(999);
    assert(erased == contained + 1);
    assert(!sorted_contains(s1, 999));
    
    erased = s1.erase(999);
    assert(erased == 0);

    // erase a range
    Set::iterator first = std::lower_bound(s1.begin(), s1.end(), 500);
    Set::iterator last = std::upper_bound(s1.begin(), s1.end(), 1000);
    s1.erase(first, last);
    assert_sorted(s1);
    assert(std::lower_bound(s1.begin(), s1.end(), 500)
           == std::upper_bound(s1.begin(), s1.end(), 1000));

    // just in case we killed too much of it.
    while (s1.size() < 5)
        s1.insert(std::rand());

    // clear
    s2.clear();
    assert(s2.size() == 0);

    // find
    {
        std::pair<Set::iterator, Set::iterator> range;
        Set::iterator pos = s1.find(s1.front());
        range = std::equal_range(s1.begin(), s1.end(), s1.front());
//        assert(pos >= range.first && pos < range.second);
        assert(std::distance(range.first, pos) >= 0
                && std::distance(pos, range.second) > 0);
 
        pos = s1.find(s1.back());
        range = std::equal_range(s1.begin(), s1.end(), s1.back());
//        assert(pos >= range.first && pos < range.second);
        assert(std::distance(range.first, pos) >= 0
                && std::distance(pos, range.second) > 0);
    }
    const int range = static_cast<int>(s1.back() - s1.front());
    mid_elt = s1.front() + range / 2;
    Set::iterator pos = s1.find(mid_elt);
    if (pos != s1.end())
        assert(*pos == mid_elt);
    else
        assert(!sorted_contains(s1, mid_elt));

    // count, lower_bound, upper_bound, equal_range
    for (int i2 = 0; i2 < 20; ++i2)
    {
        int x = std::rand() % range + s1.front();
        std::size_t cnt = s1.count(x);
        assert(cnt == static_cast<std::size_t>(
//            std::upper_bound(s1.begin(), s1.end(), x)
//            - std::lower_bound(s1.begin(), s1.end(), x)));
            std::distance(std::lower_bound(s1.begin(), s1.end(), x),
            std::upper_bound(s1.begin(), s1.end(), x))));

        pos = s1.lower_bound(x);
        assert(pos == s1.end() || *pos >= x);
        assert(pos == s1.begin() || *--pos < x);

        pos = s1.upper_bound(x);
        assert(pos == s1.end() || *pos > x);
        assert(pos == s1.begin() || *--pos <= x);

        std::pair<Set::iterator, Set::iterator> range = s1.equal_range(x);
//        assert(cnt == static_cast<std::size_t>(range.second - range.first));
        assert(cnt == static_cast<std::size_t>(
                std::distance(range.first, range.second)));
    }

    // comparisons
    assert(s1 != s2);
    assert(!(s1 == s2));
    s2 = s1;
    assert(s1 == s2);
    assert(!(s1 != s2));
    assert(!(s1 < s2));
    assert(!(s1 > s2));
    assert(s1 >= s2);
    assert(s1 <= s2);
    s2.erase(s2.begin());
    s2.insert(s1[0] - 1);
    assert(!(s1 < s2));
    assert(s1 > s2);
    assert(!(s1 <= s2));
    assert(s1 >= s2);
}

void test_complexity () {
    // Complexity of insert(pos, value)
    cout << "Testing complexity of insert(pos, value)\nFor set" << endl;
    JEP_METERED_SET_TYPE s;
    JEP_METERED_SET_TYPE::iterator si;
    int x;
    MeteredCompare::count = 0;
    for (x = 1; x < 100; x += 2)
        s.insert(s.end(), x);
    cout << "Odd 1-99, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 100);
    assert(s.size() == 50);
    MeteredCompare::count = 0;
    for (x = 0, si = s.begin(); x < 100; x += 2, ++ si, ++ si)
        si = s.insert(si, x);
    cout << "Even 0-98, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 150);
    assert(s.size() == 100);
    MeteredCompare::count = 0;
    for (x = 0, si = s.begin(); x < 100; x += 2, ++ si, ++ si)
        si = s.insert(si, x);
    cout << "Even 0-98, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 150);
    assert(s.size() == 100);
    cout << "For multiset" << endl;
    JEP_METERED_MULTISET_TYPE ms;
    JEP_METERED_MULTISET_TYPE::iterator msi;
    MeteredCompare::count = 0;
    for (x = 1; x < 100; x += 2)
        ms.insert(ms.end(), x);
    cout << "Odd 1-99, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 100);
    assert(ms.size() == 50);
    MeteredCompare::count = 0;
    for (x = 0, msi = ms.begin(); x < 100; x += 2, ++ msi, ++ msi)
        msi = ms.insert(msi, x);
    cout << "Even 0-98, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 150);
    assert(ms.size() == 100);
    MeteredCompare::count = 0;
    for (x = 0, msi = ms.begin(); x < 100; x += 2, ++ msi, ++ msi, ++ msi)
        msi = ms.insert(msi, x);
    cout << "Even 0-98, compares = " << MeteredCompare::count << endl;
//    assert(MeteredCompare::count < 150);
    assert(ms.size() == 150);
}

int main(int /*argc*/, char** /*argv*/)
{
    std::cout << "starting test..." << std::endl;

    for (int iteration = 0; iteration < 50; ++iteration)
    {
        // Make a random sequence of integers
        std::vector<int> input;

// jep
//        const int max_value = std::numeric_limits<int>::max() - 20;
        const int max_value = INT_MAX - 20;
// jep.end
        const int element_count = std::rand() % 400 + 20;
        input.reserve(element_count + 50);
        for (int x = 0; x < element_count; ++x)
            input.push_back(std::rand() % max_value);
    
        // guarantee that there are some duplicates
        const int dup_count = std::rand() % 50;
        for (int y = 0; y < dup_count; ++y)
            input.push_back(input[std::rand() % input.size()]);

        test_set(input);
        test_multiset(input);
    }
    test_complexity();
    
    std::cout << "...test complete" << std::endl;
    // Broken MSVC doesn't implicitly return 0.
    return 0;
}

