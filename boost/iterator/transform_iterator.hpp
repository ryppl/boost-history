// (C) Copyright David Abrahams 2002.
// (C) Copyright Jeremy Siek    2002.
// (C) Copyright Thomas Witt    2002.
// Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
#ifndef BOOST_TRANSFORM_ITERATOR_23022003THW_HPP
#define BOOST_TRANSFORM_ITERATOR_23022003THW_HPP

#include <boost/iterator.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost
{
  template <class AdaptableUnaryFunction, class Iterator>
  class transform_iterator;

  namespace detail 
  {

    // Given the transform iterator's transformation and iterator, this
    // is the type used as its traits.
    template <class UnaryFunction, class Iterator>
    struct transform_iterator_base
    {
    private:

      // transform_iterator does not support writable/swappable iterators
#if !BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
      BOOST_STATIC_ASSERT((is_tag< readable_iterator_tag, typename access_category<Iterator>::type >::value));
#endif
 
      typedef typename UnaryFunction::result_type result_type;

      typedef typename remove_reference< result_type >::type cv_value_type;

      typedef typename mpl::if_< 
          is_reference< result_type >
        , typename mpl::if_<
              is_const< cv_value_type >
            , readable_lvalue_iterator_tag
            , writable_lvalue_iterator_tag
          >::type
        , readable_iterator_tag
      >::type maximum_access_tag;
  
      typedef typename minimum_category<
          maximum_access_tag
        , typename access_category<Iterator>::type
      >::type access_category;

    public:
      typedef iterator_adaptor<
          transform_iterator<UnaryFunction, Iterator>
        , Iterator
        , cv_value_type  
        , iterator_tag<
             access_category
           , typename traversal_category<Iterator>::type
          >
        , result_type  
      > type;
    };

  }

  template <class AdaptableUnaryFunction, class Iterator>
  class transform_iterator
    : public detail::transform_iterator_base<AdaptableUnaryFunction, Iterator>::type
  {
    typedef typename
      detail::transform_iterator_base<AdaptableUnaryFunction, Iterator>::type
    super_t;

    friend class iterator_core_access;

  public:
    transform_iterator() { }

    transform_iterator(Iterator const& x, AdaptableUnaryFunction f)
      : super_t(x), m_f(f) { }

    template<class OtherIterator>
    transform_iterator(
          transform_iterator<AdaptableUnaryFunction, OtherIterator> const& t
        , typename enable_if_convertible<OtherIterator, Iterator>::type* = 0
    )
      : super_t(t.base()), m_f(t.functor()) {}

    AdaptableUnaryFunction functor() const
      { return m_f; }

  private:
    typename super_t::reference dereference() const
    { return m_f(*this->base()); }

    // Probably should be the initial base class so it can be
    // optimized away via EBO if it is an empty class.
    AdaptableUnaryFunction m_f;
  };

  template <class UnaryFunction, class Iterator>
  transform_iterator<UnaryFunction, Iterator> make_transform_iterator(Iterator it, UnaryFunction fun)
  {
    return transform_iterator<UnaryFunction, Iterator>(it, fun);
  }

} // namespace boost

#endif // BOOST_TRANSFORM_ITERATOR_23022003THW_HPP
