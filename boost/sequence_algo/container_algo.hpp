// (C) Copyright Jeremy Siek 2001. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

// Thu May 30     I strongly disagree to have two different type for
//                begin() and end() in std::pair<Iter1,Iter2>
//                -- makes no sense if they describe a range [Herve]

// Thu May 30     Proposed to put container interfaces
//                into separate file for <algorithm> extensions [Herve]

#ifndef BOOST_CONTAINER_ALGORITHM_HPP
#define BOOST_CONTAINER_ALGORITHM_HPP

// STL algorithm extensions 
#include <boost/config.hpp>
#include <boost/sequence_algo/algorithm.hpp>

namespace boost {

#ifdef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
  // We need the tags because the compiler will not know how to
  // choose between begin( pair<T1,T2>& ) and begin( Container& )...
  // Instead we rely on full class specialization which is
  // supported by most compilers

  namespace detail {

    // tag classes for dispatch 
    class container_algo_pair_tag {};
    class container_algo_container_tag {};

    template <Container>
    class container_algo_make_tag
    {
      typedef container_algo_container_tag       tag;
      typedef typename Container::size_type      size_type;
      typedef typename Container::iterator       result_type;
      typedef typename Container::const_iterator const_result_type;
    };

    template < class T >
    class container_algo_make_tag< std::pair<T,T> >
    {
      typedef container_algo_pair_tag            tag;
      typedef typename iterator_traits<T>::difference_type size_type;
      typedef T                                  result_type;
      typedef T                                  const_result_type;
    };

    // for container_tag
    template <typename Container>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::result_type
    begin(Container& c, container_algo_container_tag const&)
      { return c.begin(); }

    template <typename Container>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::const_result_type
    begin(const Container& c, container_algo_container_tag const&)
      { return c.begin(); }

    template <typename Container>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::result_type
    end(Container& c, container_algo_container_tag const&)
      { return c.end(); }

    template <typename Container>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::const_result_type
    end(const Container& c)
      { return c.end(); }

    template <typename Container>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::size_type
    size(const Container& c)
      { return c.size(); }

    // for pair_tag
    template <typename Iter1, typename Iter2>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::result_type
    begin(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
      { return p.first; }

    template <typename Iter1, typename Iter2>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::const_result_type
    end(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
      { return p.second; }

    template <typename Iter1, typename Iter2>
    inline typename
    container_algo_make_tag<container_algo_pair_tag>::size_type
    size(const std::pair<Iter1, Iter2>& p, container_algo_pair_tag const&)
      { return std::distance(p.first, p.second); }

  } // namespace detail

  template <typename T>
  inline typename
  container_algo_make_tag<container_algo_pair_tag>::result_type
  begin(T& t)
  {
    return detail::begin(t, typename container_algo_make_tag<T>::tag());
  }

  template <typename T>
  inline typename
  container_algo_make_tag<container_algo_pair_tag>::const_result_type
  begin(const T& t)
  { return detail::begin(t, typename container_algo_make_tag<T>::tag()); }

  template <typename T>
  inline typename
  container_algo_make_tag<container_algo_pair_tag>::result_type
  end(T& t)
  { return detail::end(t, typename container_algo_make_tag<T>::tag()); }

  template <typename T>
  inline typename
  container_algo_make_tag<container_algo_pair_tag>::const_result_type
  end(const T& t)
  { return detail::end(t, typename container_algo_make_tag<T>::tag()); }

  template <typename T>
  inline typename
  container_algo_make_tag<container_algo_pair_tag>::size_type
  size(const T& t)
  {
    return detail::size(t, typename container_algo_make_tag<T>::tag());
  }
#else
  // The compiler will handle it fine like this

  template <typename Container>
  typename Container::iterator
  begin(Container& c) { return c.begin(); }

  template <typename Container>
  typename Container::const_iterator
  begin(const Container& c) { return c.begin(); }

  template <typename Container>
  typename Container::iterator
  end(Container& c) { return c.end(); }

  template <typename Container>
  typename Container::const_iterator
  end(const Container& c) { return c.end(); }

  template <typename Container>
  typename Container::size_type
  size(const Container& c) { return c.size(); }

  template <typename Iter1, typename Iter2>
  Iter1 begin(const std::pair<Iter1, Iter2>& p) { return p.first; }

  template <typename Iter1, typename Iter2>
  Iter2 end(const std::pair<Iter1, Iter2>& p) { return p.second; }

  template <typename Iter1, typename Iter2>
  typename boost::detail::iterator_traits<Iter1>::difference_type
  size(const std::pair<Iter1, Iter2>& p) {
    return std::distance(p.first, p.second);
  }
#endif

  // container iota
  template <typename Container, typename T>
  void iota(Container& c, const T& value)
  {
    iota(begin(c), end(c), value);
  }
 
  // container copy
  template <typename Container, typename OutIter>
  OutIter copy(const Container& c, OutIter result)
  {
    return std::copy(begin(c), end(c), result);
  }

  // container equal
  template <typename Container1, typename Container2>
  bool equal(const Container1& c1, const Container2& c2)
  {
    if (size(c1) != size(c2))
      return false;
    return std::equal(begin(c1), end(c1), begin(c2));
  }

  // container sort
  template <typename Container>
  void sort(Container& c) { std::sort(begin(c), end(c)); }

  template <typename Container, typename Predicate>
  void sort(Container& c, const Predicate& p) { 
    std::sort(begin(c), end(c), p);
  }

  // container stable_sort
  template <typename Container>
  void stable_sort(Container& c) { std::stable_sort(begin(c), end(c)); }

  template <typename Container, typename Predicate>
  void stable_sort(Container& c, const Predicate& p) { 
    std::stable_sort(begin(c), end(c), p);
  }

  // container contains
  template <typename Container, typename T>
  bool contains(const Container& c, const T& value)
  {
    return contains(begin(c), end(c), value);
  }

  // container all
  template <typename Container, typename Predicate>
  bool all(const Container& c, Predicate p)
  {
    return all(begin(c), end(c), p);
  }

  // container none
  template <typename Container, typename Predicate>
  bool none(const Container& c, Predicate p)
  {
    return none(begin(c), end(c), p);
  }

  // container count
  template <typename Container, typename T>
  std::size_t count(const Container& c, const T& value)
  {
    return std::count(begin(c), end(c), value);
  }

  // container count_if
  template <typename Container, typename Predicate>
  std::size_t count_if(const Container& c, Predicate p)
  {
    return std::count_if(begin(c), end(c), p);
  }

  // container is_sorted
  template <typename Container>
  bool is_sorted(const Container& c)
  {
    return is_sorted(begin(c), end(c));
  }

  template <typename Container, typename StrictWeakOrdering>
  bool is_sorted(const Container& c, StrictWeakOrdering comp)
  {
    return is_sorted(begin(c), end(c), comp);
  }

} // namespace boost

#endif // BOOST_CONTAINER_ALGORITHM_HPP
