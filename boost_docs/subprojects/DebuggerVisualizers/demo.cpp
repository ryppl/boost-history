#define BOOST_MULTI_INDEX_LIMIT_INDEXED_BY_SIZE    5
#define BOOST_MULTI_INDEX_LIMIT_TAG_SIZE           3
#define BOOST_MULTI_INDEX_LIMIT_COMPOSITE_KEY_SIZE 5
#include "mic_visualizer.hpp"

#include <string>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
using namespace boost::multi_index;
using std::wstring;
using namespace std;

#include <boost/pool/pool_alloc.hpp>

struct test {
  wstring x;
  wstring y;
  test(wchar_t const *str, wchar_t const *str2=L"def") : x(str), y(str2) {}
  bool operator<(test const& other) const {
    return x<other.x;
  }
  bool operator==(test const& other) const {
    return x==other.x;
  }
};

namespace boost {
  inline std::size_t hash_value(test const& t)
  {
    return hash_value(t.x);
  }
}

typedef multi_index_container<test, indexed_by<hashed_non_unique<identity<test> >, sequenced<>>/*, boost::fast_pool_allocator<test> */> cont;

VISUALIZE_MULTI_INDEX_CONTAINER(cont);

int wmain() {
  cont test_cont;
  test_cont.insert(test(L"aaa"));
  test_cont.insert(test(L"aaa", L"other"));
  test_cont.insert(test(L"E", L"other"));
  test_cont.insert(test(L"aac"));
  /*
  test_cont.push_back(test(L"aaa"));
  test_cont.push_back(test(L"aaa", L"other"));
  test_cont.push_back(test(L"E", L"other"));
  test_cont.push_back(test(L"aac"));
  */
  return 0; // see screenshoxt
}
