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

/// \file views/list_set_view.hpp
/// \brief View of a side of a bimap that is signature compatible with std::list.

#ifndef BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP
#define BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

#include <boost/config.hpp>

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
    public ::boost::bimap::container_adaptor::list_adaptor
    <
        IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator
    >,

    public ::boost::bimap::detail::set_view_base< list_set_view< IndexType >, IndexType >
{
    BOOST_BIMAP_SET_VIEW_BASE_FRIEND(list_set_view,IndexType);

    typedef ::boost::bimap::container_adaptor::list_adaptor
    <
        IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator

    > base_;

    public:

    list_set_view(typename base_::base_type & c) :
        base_(c) {}

    list_set_view & operator=(const list_set_view & v) { this->base() = v.base(); return *this; }
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
    public ::boost::bimap::container_adaptor::list_adaptor
    <
        const IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator
    >
{
    typedef ::boost::bimap::container_adaptor::list_adaptor
    <
        const IndexType,
        typename IndexType::iterator,         typename IndexType::const_iterator,
        typename IndexType::reverse_iterator, typename IndexType::const_reverse_iterator

    > base_;

    public:

    const_list_set_view(typename base_::base_type & c) :
        base_(c) {}
};


} // namespace views
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_VIEWS_LIST_SET_VIEW_HPP

