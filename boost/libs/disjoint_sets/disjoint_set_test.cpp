//  (C) Copyright Jeremy Siek 2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all
//  copies. This software is provided "as is" without express or
//  implied warranty, and with no claim as to its suitability for any
//  purpose.

#include <boost/pending/disjoint_sets.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/cstdlib.hpp>

template <typename DisjointSet>
struct test_disjoint_set {
  static void do_test()
  {
    // The following tests are pretty lame, just a basic sanity check.
    // Industrial strength tests still need to be written.
    
    std::size_t elts[] = { 1, 2, 3, 4 }; 
    const int N = 4; //sizeof(elts)/sizeof(std::size_t);
    
    DisjointSet ds(N);
    ds.make_set(elts[0]);
    ds.make_set(elts[1]);
    ds.make_set(elts[2]);
    ds.make_set(elts[3]);

    BOOST_CHECK(ds.find_set(1) != ds.find_set(2));
    BOOST_CHECK(ds.find_set(1) != ds.find_set(3));
    BOOST_CHECK(ds.find_set(1) != ds.find_set(4));
    BOOST_CHECK(ds.find_set(2) != ds.find_set(3));
    BOOST_CHECK(ds.find_set(2) != ds.find_set(4));
    BOOST_CHECK(ds.find_set(3) != ds.find_set(4));

    ds.union_set(1, 2);
    ds.union_set(3, 4);

    BOOST_CHECK(ds.find_set(1) != ds.find_set(4));
    
    int a = ds.find_set(1);
    BOOST_CHECK(a == ds.find_set(2));
    int b = ds.find_set(3);
    BOOST_CHECK(b == ds.find_set(4));
    ds.link(a, b);
    BOOST_CHECK(ds.find_set(a) == ds.find_set(b));
    BOOST_CHECK(1 == ds.count_sets(elts, elts + N));

    ds.normalize_sets(elts, elts + N);
    ds.compress_sets(elts, elts + N);
    BOOST_CHECK(1 == ds.count_sets(elts, elts + N));
  }
};

int
test_main(int, char*[])
{
  using namespace boost;
  {
    typedef 
      disjoint_sets_with_storage<identity_property_map, identity_property_map,
      find_with_path_halving> ds_type;
    test_disjoint_set<ds_type>::do_test();
  }
  {
    typedef 
      disjoint_sets_with_storage<identity_property_map, identity_property_map,
      find_with_full_path_compression> ds_type;
    test_disjoint_set<ds_type>::do_test();
  }
  return boost::exit_success;
}
