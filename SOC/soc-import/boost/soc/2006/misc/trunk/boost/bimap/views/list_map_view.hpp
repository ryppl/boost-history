// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file views/list_map_view.hpp
/// \brief View of a side of a bimap.

#ifndef BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/container_adaptor/list_map_adaptor.hpp>
#include <boost/bimap/relation/support/pair_by.hpp>
#include <boost/bimap/support/iterator_type_by.hpp>
#include <boost/bimap/detail/map_view_base.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>

namespace boost {
namespace bimap {
namespace views {

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

template< class Tag, class BimapType >
struct list_map_view_base
{
    typedef ::boost::bimap::container_adaptor::list_map_adaptor
    <
        typename BimapType::core_type::template index<Tag>::type,
        typename ::boost::bimap::support::      iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::      reverse_iterator_type_by<Tag,BimapType>::type,
        typename ::boost::bimap::support::const_reverse_iterator_type_by<Tag,BimapType>::type,
        ::boost::bimap::container_adaptor::support::iterator_facade_to_base
        <
            typename ::boost::bimap::support::
                iterator_type_by<Tag,BimapType>::type,
            typename ::boost::bimap::support::
                const_iterator_type_by<Tag,BimapType>::type

        >,
        ::boost::mpl::na,
        ::boost::mpl::na,
        ::boost::mpl::na,
        ::boost::bimap::relation::support::
            get_pair_functor<Tag, typename BimapType::relation >,

        typename ::boost::bimap::relation::support::data_extractor
        <
            Tag,
            typename BimapType::relation

        >::type

    > type;
};

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

/// \brief View of a side of a bimap.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core.

See also const_list_map_view.
                                                                                    **/
template< class Tag, class BimapType >
class list_map_view
:
    public list_map_view_base<Tag,BimapType>::type,
    public ::boost::bimap::detail::map_view_base< list_map_view<Tag,BimapType>,Tag,BimapType >

{
    typedef typename list_map_view_base<Tag,BimapType>::type base_;

    BOOST_BIMAP_MAP_VIEW_BASE_FRIEND(list_map_view,Tag,BimapType);

    public:

    list_map_view(typename base_::base_type & c) :
        base_(c) {}

    list_map_view & operator=(const list_map_view & v) { this->base() = v.base(); return *this; }

    BOOST_BIMAP_VIEW_FRONT_BACK_IMPLEMENTATION

    // Rearrange Operations

    void relocate(typename base_::iterator position, typename base_::iterator i)
    {
        this->base().relocate(
            this->template functor<typename base_::iterator_to_base>()(position),
            this->template functor<typename base_::iterator_to_base>()(i)
        );
    }

    void relocate(typename base_::iterator position,
                  typename base_::iterator first, typename base_::iterator last)
    {
        this->base().relocate(
            this->template functor<typename base_::iterator_to_base>()(position),
            this->template functor<typename base_::iterator_to_base>()(first),
            this->template functor<typename base_::iterator_to_base>()(last)
        );
    }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_LIST_MAP_VIEW_HPP

