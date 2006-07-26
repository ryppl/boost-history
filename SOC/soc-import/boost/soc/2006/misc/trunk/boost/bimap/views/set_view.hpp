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
    public BOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor, reverse_iterator, const_reverse_iterator
    ),

    public ::boost::bimap::detail::set_view_base< set_view< IndexType >, IndexType >
{
    typedef BOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor, reverse_iterator, const_reverse_iterator

    ) base_;

    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(set_view,IndexType);

    public:

    set_view(typename base_::base_type & c) : base_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename base_::iterator,typename base_::iterator>
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
    public BOOST_BIMAP_CONST_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor, reverse_iterator, const_reverse_iterator
    )
{
    typedef BOOST_BIMAP_CONST_SET_VIEW_CONTAINER_ADAPTOR(
        set_adaptor, reverse_iterator, const_reverse_iterator

    ) base_;

    public:

    const_set_view(typename base_::base_type & c) : base_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename base_::iterator,typename base_::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(lower,upper);
    }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_SET_VIEW_HPP


