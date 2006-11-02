// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/map_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::map.

#ifndef BOOST_BIMAP_VIEWS_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

#include <boost/bimap/container_adaptor/map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>
#include <boost/bimap/detail/operator_bracket_proxy.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/call_traits.hpp>

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
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by
    ),
    public ::boost::bimap::detail::map_view_base< map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        map_adaptor,
        Tag,BimapType,
        reverse_iterator_type_by,const_reverse_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(map_view,Tag,BimapType);

    public:

    map_view(typename base_::base_type & c) : base_(c) {}

    ::boost::bimap::detail::operator_bracket_proxy<map_view>
        operator[](typename ::boost::call_traits< typename base_::key_type >::param_type k)
    {
        return ::boost::bimap::detail::operator_bracket_proxy<map_view>(*this,k);
    }

    typename base_::data_type const &
        operator[](typename ::boost::call_traits< typename base_::key_type >::param_type k) const
    {
        typename base_::const_iterator i = this->find(k);
        if( i == this->end() )
        {
            ::boost::throw_exception( ::boost::bimap::value_not_found() );
        }
        return i->second;
    }

    BOOST_BIMAP_MAP_VIEW_RANGE_IMPLEMENTATION(base_)

    map_view & operator=(const map_view & v) { this->base() = v.base(); return *this; }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MAP_VIEW_HPP

