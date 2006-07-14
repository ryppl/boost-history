// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file container_adaptor/detail/functor_bag.hpp
/// \brief Defines a EBO optimizacion helper for functors.

#ifndef BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_FUNCTOR_BAG_HPP
#define BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_FUNCTOR_BAG_HPP

#include <boost/mpl/deref.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost {
namespace bimap {
namespace container_adaptor {
namespace detail {

/// \brief EBO optimizacion helper for functors
/**

This class is a generalization of a helper class explained in an article by
Nathan C. Myers.\n
See it at \link http://www.cantrip.org/emptyopt.html
                                                                                    **/

struct checker {};

template< class IterBegin, class IterEnd, class Enable = void >
struct BaseExpander;

template< class IterBegin, class IterEnd >
struct BaseExpander
<
    IterBegin, IterEnd,
    typename enable_if< is_same<IterBegin,IterEnd> >::type
>

{};

template< class IterBegin, class IterEnd >
struct BaseExpander
<
    IterBegin, IterEnd,
    typename enable_if< mpl::not_< is_same<IterBegin,IterEnd> > >::type
>
 :
    public mpl::deref<IterBegin>::type,
    public BaseExpander< typename mpl::next<IterBegin>::type, IterEnd >

{};



template < class Data, class FunctorList >
struct DataWithFunctorBag :
    BaseExpander
    <
        typename mpl::begin<FunctorList>::type,
        typename mpl::end<FunctorList>::type
    >
{
    Data data;
    DataWithFunctorBag() {}
    DataWithFunctorBag(typename add_reference<Data const>::type d) : data(d) {}

    template< class Functor >
    Functor& functor()
    {
        return *(static_cast<Functor*>(this));
    }

    template< class Functor >
    const Functor& functor() const
    {
        return *(static_cast<Functor const *>(this));
    }
};

} // namespace detail
} // namespace container_adaptor
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_CONTAINER_ADAPTOR_DETAIL_FUNCTOR_BAG_HPP


