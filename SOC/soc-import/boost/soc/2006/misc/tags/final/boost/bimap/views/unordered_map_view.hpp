// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/unordered_map_view.hpp
/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_map.

#ifndef BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP

#include <boost/bimap/container_adaptor/unordered_map_adaptor.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/bimap/detail/operator_bracket_proxy.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a side of a bimap that is signature compatible with tr1::unordered_map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_map.

See also const_unordered_map_view.
                                                                                    **/


template< class Tag, class BimapType >
class unordered_map_view
:
    public BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by
    ),

    public ::boost::bimap::detail::map_view_base< unordered_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by

    ) base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(unordered_map_view,Tag,BimapType);

    public:

    unordered_map_view(typename base_::base_type & c)
        : base_(c) {}

    ::boost::bimap::detail::operator_bracket_proxy<unordered_map_view>
        operator[](const typename base_::key_type & k)
    {
        return ::boost::bimap::detail::operator_bracket_proxy<unordered_map_view>(*this,k);
    }

    typename base_::data_type const &
        operator[](const typename base_::key_type & k) const
    {
        typename base_::const_iterator i = this->find(k);
        if( i == this->end() )
        {
            ::boost::throw_exception( ::boost::bimap::value_not_found() );
        }
        return i->second;
    }

};

/// \brief Constant view of a side of a bimap that is signature compatible with tr1::unordered_map.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a tr1::unordered_map.

See also unordered_map_view.
                                                                                    **/

template< class Tag, class BimapType >
class const_unordered_map_view
:
    public BOOST_BIMAP_CONST_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by
    )
{
    typedef BOOST_BIMAP_MAP_VIEW_CONTAINER_ADAPTOR(
        unordered_map_adaptor,
        Tag,BimapType,
        local_iterator_type_by,const_local_iterator_type_by

    ) base_;

    public:

    const_unordered_map_view(typename base_::base_type & c)
        : base_(c) {}

    typename base_::data_type const &
        operator[](const typename base_::key_type & k) const
    {
        typename base_::const_iterator i = this->find(k);
        if( i == this->end() )
        {
            ::boost::throw_exception( ::boost::bimap::value_not_found() );
        }
        return i->second;
    }

};

} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_UNOREDERED_MAP_VIEW_HPP


