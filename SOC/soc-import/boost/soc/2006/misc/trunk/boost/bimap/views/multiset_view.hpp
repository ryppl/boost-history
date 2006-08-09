// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/multiset_view.hpp
/// \brief View of a bimap that is signature compatible with std::multiset.

#ifndef BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP

#include <boost/bimap/container_adaptor/multiset_adaptor.hpp>
#include <boost/bimap/detail/set_view_base.hpp>

namespace boost {
namespace bimap {
namespace views {

/// \brief View of a bimap that is signature compatible with std::multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multiset.

See also const_multiset_view.
                                                                                    **/

template< class IndexType >
class multiset_view
:
    public BOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        multiset_adaptor, reverse_iterator, const_reverse_iterator
    ),

    public ::boost::bimap::detail::set_view_base< multiset_view< IndexType >, IndexType >
{
    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(multiset_view,IndexType);

    typedef BOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(
        multiset_adaptor, reverse_iterator, const_reverse_iterator

    ) base_;

    public:

    multiset_view(typename base_::base_type & c) : base_(c) {}

    template<typename LowerBounder, typename UpperBounder>
    std::pair<typename base_::iterator, typename base_::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(lower,upper);
    }

    multiset_view & operator=(const multiset_view & v) { this->base() = v.base(); return *this; }
};

/// \brief Constant view of a bimap that is signature compatible with std::multiset.
/**

This class uses container_adaptor and iterator_adaptor to wrapped a index of the
multi_index bimap core so it can be used as a std::multiset.

See also multiset_view.
                                                                                    **/

template< class IndexType >
class const_multiset_view
:
    public BOOST_BIMAP_CONST_SET_VIEW_CONTAINER_ADAPTOR(
        multiset_adaptor, reverse_iterator, const_reverse_iterator
    )
{
    typedef BOOST_BIMAP_CONST_SET_VIEW_CONTAINER_ADAPTOR(
        multiset_adaptor, reverse_iterator, const_reverse_iterator

    ) base_;

    public:

    const_multiset_view(typename base_::base_type & c) : base_(c) {}

    template<typename LowerBounder,typename UpperBounder>
    std::pair<typename base_::iterator, typename base_::iterator>
        range(LowerBounder lower,UpperBounder upper) const
    {
        return this->base().range(lower,upper);
    }
};


} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_MULTISET_VIEW_HPP

