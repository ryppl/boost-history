// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/multimap_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::multimap.

#ifndef BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MULTIMAP_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/multimap_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with std::multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multimap.

See also const_multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class multimap_view
:
    public container_adaptor::multimap_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,

        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        typename ::boost::bimap::detail::map_view_iterator_to_base<Tag,BimapType>::type,

        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >,

    public ::boost::bimap::detail::map_view_base< multimap_view<Tag,BimapType>,Tag,BimapType >

{
    typedef multimap_view this_type;

    #if defined(BOOST_MSVC)
        typedef ::boost::bimap::detail::map_view_base< multimap_view<Tag,BimapType>,Tag,BimapType >
            friend_map_view_base;
        friend class friend_map_view_base;
    #else
        friend class ::boost::bimap::detail::map_view_base< multimap_view<Tag,BimapType>,Tag,BimapType >;
    #endif

    public:

    multimap_view() {}
    multimap_view(typename this_type::base_type & c)
        : this_type::multimap_adaptor_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename this_type::iterator,typename this_type::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        std::pair<

            typename this_type::base_type::const_iterator,
            typename this_type::base_type::const_iterator

        > r( this->base().range(lower,upper) );

        return std::pair
        <
            typename this_type::const_iterator,
            typename this_type::const_iterator
        >(
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.first ),
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.second )
        );
    }

};

/// \brief Constant view of a side of a bimap that is signature compatible with std::multimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multimap.

See also multimap_view.
                                                                                    **/

template< class Tag, class BimapType >
class const_multimap_view
:
    public container_adaptor::multimap_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename ::boost::bimap::support::iterator_type_by<Tag,BimapType>::type,
            typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    typedef const_multimap_view this_type;

    public:

    const_multimap_view() {}
    const_multimap_view(typename this_type::base_type & c) : this_type::map_adaptor_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename this_type::iterator,typename this_type::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        std::pair<

            typename this_type::base_type::const_iterator,
            typename this_type::base_type::const_iterator

        > r( this->base().range(lower,upper) );

        return std::pair
        <
            typename this_type::const_iterator,
            typename this_type::const_iterator
        >(
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.first ),
            this_type::template functor<
                typename this_type::iterator_from_base
            >()                                         ( r.second )
        );
    }
};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

