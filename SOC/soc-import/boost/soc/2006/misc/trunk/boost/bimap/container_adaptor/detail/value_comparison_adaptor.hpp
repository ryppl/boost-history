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

#include <boost/mpl/placeholders.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_base_of.hpp>

#include <boost/mpl/inherit_linearly.hpp>
#include <boost/mpl/inherit.hpp>

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

template < class Data, class FunctorList >
struct DataWithFunctorBag :

    public mpl::inherit_linearly<

        FunctorList,
        mpl::if_< is_base_of< mpl::_2, mpl::_1 >,
        //   {
                 mpl::_1,
        //   }
        //   else
        //   {
                 mpl::inherit< mpl::_1, mpl::_2 >
        //   }
        >

    >::type
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


