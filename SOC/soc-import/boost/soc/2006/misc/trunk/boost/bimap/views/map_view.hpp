// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/map_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::map.

#ifndef BOOST_BIMAP_VIEWS_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/relation/support/get_pair_functor.hpp>
#include <boost/bimap/container_adaptor/support/iterator_facade_converters.hpp>
#include <boost/bimap/container_adaptor/map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>
#include <boost/bimap/detail/operator_bracket_proxy.hpp>
#include <boost/bimap/detail/map_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {


/// \brief View of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also const_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class map_view
:
    public container_adaptor::map_adaptor
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

    public ::boost::bimap::detail::map_view_base< map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef map_view this_type;

    #if defined(BOOST_MSVC)
        typedef ::boost::bimap::detail::map_view_base< map_view<Tag,BimapType>,Tag,BimapType >
            friend_map_view_base;
        friend class friend_map_view_base;
    #else
        friend class ::boost::bimap::detail::map_view_base< map_view<Tag,BimapType>,Tag,BimapType >;
    #endif

    public:

    map_view() {}
    map_view(typename this_type::base_type & c) : this_type::map_adaptor_(c) {}

    ::boost::bimap::detail::operator_bracket_proxy<map_view>
        operator[](const typename this_type::key_type & k)
    {
        return ::boost::bimap::detail::operator_bracket_proxy<map_view>(*this,k);
    }


    typename this_type::data_type const &
        operator[](const typename this_type::key_type & k) const
    {
        // TODO
        // Add index check?
        return this->find(k)->second;
    }

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

/// \brief Constant view of a side of a bimap that is signature compatible with std::map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::map.

See also map_view.
                                                                                    **/
template< class Tag, class BimapType >
class const_map_view
:
    public container_adaptor::map_adaptor
    <
        const typename BimapType::core_type::template index<Tag>::type,

        typename support::iterator_type_by<Tag,BimapType>::type,
        typename support::const_iterator_type_by<Tag,BimapType>::type,
        typename support::reverse_iterator_type_by<Tag,BimapType>::type,
        typename support::const_reverse_iterator_type_by<Tag,BimapType>::type,

        container_adaptor::support::iterator_facade_to_base
        <
            typename support::iterator_type_by<Tag,BimapType>::type,
            typename support::const_iterator_type_by<Tag,BimapType>::type
        >,
        container_adaptor::use_default, // iterator from base converter
        container_adaptor::use_default, // reverse iterator from base converter
        container_adaptor::use_default, // value to base converter

        relation::support::GetPairFunctor<Tag, typename BimapType::relation >
    >
{
    typedef const_map_view this_type;

    public:

    const_map_view() {}
    const_map_view(typename this_type::base_type & c) : this_type::map_adaptor_(c) {}

    typename this_type::data_type const &
        operator[](const typename this_type::key_type & k) const
    {
        // TODO
        // Add index check?
        return this->find(k)->second;
    }

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

