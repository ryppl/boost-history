// (C) Copyright Jeremy Siek 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/iterator/iterator_concepts.hpp>
#include <boost/operators.hpp>
#include <boost/static_assert.hpp> // remove
#include <boost/detail/workaround.hpp>
#include "static_assert_same.hpp" // remove

struct new_random_access
  : std::random_access_iterator_tag
  , boost::random_access_traversal_tag
{};

struct new_iterator
  : public boost::iterator< new_random_access, int >
{
  int& operator*() const { return *m_x; }
  new_iterator& operator++() { return *this; }
  new_iterator operator++(int) { return *this; }
  new_iterator& operator--() { return *this; }
  new_iterator operator--(int) { return *this; }
  new_iterator& operator+=(std::ptrdiff_t) { return *this; }
  new_iterator operator+(std::ptrdiff_t) { return *this; }
  new_iterator& operator-=(std::ptrdiff_t) { return *this; }
  std::ptrdiff_t operator-(const new_iterator&) const { return 0; }
  new_iterator operator-(std::ptrdiff_t) const { return *this; }
  bool operator==(const new_iterator&) const { return false; }
  bool operator!=(const new_iterator&) const { return false; }
  bool operator<(const new_iterator&) const { return false; }
  int* m_x;
};
new_iterator operator+(std::ptrdiff_t, new_iterator x) { return x; }

struct old_iterator
  : public boost::iterator<std::random_access_iterator_tag, int>
{
  int& operator*() const { return *m_x; }
  old_iterator& operator++() { return *this; }
  old_iterator operator++(int) { return *this; }
  old_iterator& operator--() { return *this; }
  old_iterator operator--(int) { return *this; }
  old_iterator& operator+=(std::ptrdiff_t) { return *this; }
  old_iterator operator+(std::ptrdiff_t) { return *this; }
  old_iterator& operator-=(std::ptrdiff_t) { return *this; }
  old_iterator operator-(std::ptrdiff_t) const { return *this; }
  std::ptrdiff_t operator-(const old_iterator&) const { return 0; }
  bool operator==(const old_iterator&) const { return false; }
  bool operator!=(const old_iterator&) const { return false; }
  bool operator<(const old_iterator&) const { return false; }
  int* m_x;
};
old_iterator operator+(std::ptrdiff_t, old_iterator x) { return x; }

int
main()
{
  typedef boost::iterator_traversal<new_iterator>::type traversal_category;

  BOOST_STATIC_ASSERT(
      (boost::is_convertible<traversal_category, boost::random_access_traversal_tag>::value
      ));
                          
  boost::function_requires<
    boost_concepts::WritableLvalueIteratorConcept<int*> >();
  boost::function_requires<
    boost_concepts::RandomAccessTraversalConcept<int*> >();

  boost::function_requires<
    boost_concepts::ReadableLvalueIteratorConcept<const int*> >();
  boost::function_requires<
    boost_concepts::RandomAccessTraversalConcept<const int*> >();

  boost::function_requires<
    boost_concepts::WritableLvalueIteratorConcept<new_iterator> >();
  boost::function_requires<
    boost_concepts::RandomAccessTraversalConcept<new_iterator> >();

  boost::function_requires<
    boost_concepts::WritableLvalueIteratorConcept<old_iterator> >();
  boost::function_requires<
    boost_concepts::RandomAccessTraversalConcept<old_iterator> >();
  return 0;
}
