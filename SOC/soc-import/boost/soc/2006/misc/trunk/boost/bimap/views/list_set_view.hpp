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

#ifndef BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

#include <boost/bimap/container_adaptor/list_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::list.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::list.

See also const_list_set_view.
                                                                                    **/

template< class IndexType >
class list_set_view
:
    public container_adaptor::list_adaptor
    <
        IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator
    >,

    public bimap::detail::set_view_base< list_set_view< IndexType >, IndexType >
{
    friend class bimap::detail::set_view_base< list_set_view< IndexType >, IndexType >;

    public:

    list_set_view() {}
    list_set_view(typename list_set_view::base_type & c) :
        list_set_view::list_adaptor_(c) {}
};

/// \brief Constant view of a bimap that is signature compatible with std::set.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::list.

See also set_view.
                                                                                    **/


template< class IndexType >
class const_list_set_view
:
    public container_adaptor::list_adaptor
    <
        const IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator
    >
{
    public:

    const_list_set_view() {}
    const_list_set_view(typename const_list_set_view::base_type & c) :
        const_list_set_view::list_adaptor_(c) {}

};


} // namespace views
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

