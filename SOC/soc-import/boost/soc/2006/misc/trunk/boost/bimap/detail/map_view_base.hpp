// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file detail/map_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
#define BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP

#include <boost/bimap/relation/support/data_extractor.hpp>
#include <boost/bimap/relation/support/opposite_tag.hpp>
#include <boost/bimap/relation/support/pair_type_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>

namespace boost {
namespace bimap {
namespace detail {

/// \brief Metafunction to compute the iterator_to_base functor needed in map views.

template< class Tag, class BimapType >
struct map_view_iterator_to_base
{
    typedef container_adaptor::support::iterator_facade_to_base
    <
        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type

    > type;
};

/// \brief Common base for map views.

template< class Derived, class Tag, class BimapType>
class map_view_base
{
    typedef typename map_view_iterator_to_base<Tag,BimapType>::type iterator_to_base;

    public:

    bool replace(typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type position,
                 typename relation::support::pair_type_by
                 <
                     Tag,
                     typename BimapType::relation

                 >::type const & x)
    {
        return derived().base().replace(
            derived().template functor<iterator_to_base>()(position),
            typename BimapType::relation(x)
        );
    }

    template<typename Modifier>
    bool modify(typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type position,Modifier mod)
    {
        return derived().base().modify(

            derived().template functor<iterator_to_base>()(position),

            ::boost::bimap::detail::relation_modifier_adaptor
            <
                Modifier,
                typename BimapType::relation,
                typename relation::support::data_extractor
                <
                    Tag,
                    typename BimapType::relation

                >::type,
                typename relation::support::data_extractor
                <
                    typename relation::support::opossite_tag<Tag,BimapType>::type,
                    typename BimapType::relation

                >::type

            >(mod)
        );
    }

    protected:

    typedef map_view_base map_view_base_;

    private:

    // Curiously Recurring Template interface.

    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }
};


} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_MAP_VIEW_BASE_HPP
