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

/// \file detail/set_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef BOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP
#define BOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP

#include <boost/config.hpp>
#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>

namespace boost {
namespace bimap {
namespace detail {

// The next macro can be converted in a metafunctor to gain code robustness.

/********************************************************************************/
#define BOOST_BIMAP_SET_VIEW_CONTAINER_ADAPTOR(                                  \
    CONTAINER_ADAPTOR, OTHER_ITER, CONST_OTHER_ITER                              \
)                                                                                \
::boost::bimap::container_adaptor::CONTAINER_ADAPTOR                             \
<                                                                                \
    IndexType,                                                                   \
    typename IndexType::iterator,                                                \
    typename IndexType::const_iterator,                                          \
    typename IndexType::OTHER_ITER,                                              \
    typename IndexType::CONST_OTHER_ITER,                                        \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    typename IndexType::key_from_value                                           \
>
/********************************************************************************/

/********************************************************************************/
#define BOOST_BIMAP_CONST_SET_VIEW_CONTAINER_ADAPTOR(                            \
    CONTAINER_ADAPTOR, OTHER_ITER, CONST_OTHER_ITER                              \
)                                                                                \
::boost::bimap::container_adaptor::CONTAINER_ADAPTOR                             \
<                                                                                \
    const IndexType,                                                             \
    typename IndexType::iterator,                                                \
    typename IndexType::const_iterator,                                          \
    typename IndexType::OTHER_ITER,                                              \
    typename IndexType::CONST_OTHER_ITER,                                        \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    ::boost::bimap::container_adaptor::use_default,                              \
    typename IndexType::key_from_value                                           \
>
/********************************************************************************/

#if defined(BOOST_MSVC)
/********************************************************************************/
#define BOOST_BIMAP_SET_VIEW_BASE_FRIEND(TYPE,INDEX_TYPE)                        \
    typedef ::boost::bimap::detail::set_view_base<                               \
        TYPE< INDEX_TYPE >, INDEX_TYPE > template_class_friend;                  \
    friend class template_class_friend;
/********************************************************************************/
#else
/********************************************************************************/
#define BOOST_BIMAP_SET_VIEW_BASE_FRIEND(TYPE,INDEX_TYPE)                        \
    friend class ::boost::bimap::detail::set_view_base<                          \
        TYPE< INDEX_TYPE >, INDEX_TYPE >;
/********************************************************************************/
#endif


/// \brief Common base for set views.

template< class Derived, class Index >
class set_view_base
{
    public:

    bool replace(typename Index::iterator position,
                 typename Index::value_type const & x)
    {
        return derived().base().replace(position,x);
    }

    template<typename Modifier>
    bool modify(typename Index::iterator position,Modifier mod)
    {
        return derived().base().modify(

            position,

            ::boost::bimap::detail::relation_modifier_adaptor
            <
                Modifier,
                typename Index::value_type,
                typename ::boost::bimap::relation::support::data_extractor
                <
                    ::boost::bimap::relation::member_at::left,
                    typename Index::value_type

                >::type,
                typename ::boost::bimap::relation::support::data_extractor
                <
                    ::boost::bimap::relation::member_at::right,
                    typename Index::value_type

                >::type

            >(mod)
        );
    }

    protected:

    typedef set_view_base set_view_base_;

    private:

    // Curiously Recurring Template interface.

    Derived& derived()
    {
        return *static_cast<Derived*>(this);
    }

    Derived const& derived() const
    {
        return *static_cast<Derived const*>(this);
    }
};



} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP
