// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/set_view.hpp
/// \brief View of a bimap that is signature compatible with std::set.

#ifndef BOOST_BIMAP_VIEWS_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_SET_VIEW_HPP

#include <boost/config.hpp>

#include <boost/bimap/container_adaptor/set_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::set.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::set.

See also const_set_view.
                                                                                    **/

template< class IndexType >
class set_view
:
    public container_adaptor::set_adaptor
    <
        IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >,

    public ::boost::bimap::detail::set_view_base< set_view< IndexType >, IndexType >
{
    #if defined(BOOST_MSVC)
        typedef ::boost::bimap::detail::set_view_base< set_view< IndexType >, IndexType >
            friend_set_view_base;
        friend class friend_set_view_base;
    #else
        friend class ::boost::bimap::detail::set_view_base< set_view< IndexType >, IndexType >;
    #endif

    typedef set_view this_type;

    public:

    set_view() {}
    set_view(typename set_view::base_type & c) : set_view::set_adaptor_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename this_type::iterator,typename this_type::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(lower,upper);
    }
};

/// \brief Constant view of a bimap that is signature compatible with std::set.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::set.

See also set_view.
                                                                                    **/


template< class IndexType >
class const_set_view
:
    public container_adaptor::set_adaptor
    <
        const IndexType,
        typename IndexType::iterator,
        typename IndexType::const_iterator,
        typename IndexType::reverse_iterator,
        typename IndexType::const_reverse_iterator,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        container_adaptor::use_default,
        typename IndexType::key_from_value
    >
{
    typedef const_set_view this_type;

    public:

    const_set_view() {}
    const_set_view(typename const_set_view::base_type & c) : const_set_view::set_adaptor_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename this_type::iterator,typename this_type::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(lower,upper);
    }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_SET_VIEW_HPP


