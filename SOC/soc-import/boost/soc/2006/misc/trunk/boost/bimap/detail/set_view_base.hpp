// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file detail/set_view_base.hpp
/// \brief Helper base for the construction of the bimap views types.

#ifndef BOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP
#define BOOST_BIMAP_DETAIL_SET_VIEW_BASE_HPP

#include <boost/bimap/relation/member_at.hpp>
#include <boost/bimap/relation/support/data_extractor.hpp>
#include <boost/bimap/detail/modifier_adaptor.hpp>

namespace boost {
namespace bimap {
namespace detail {

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
                typename relation::support::data_extractor
                <
                    relation::member_at::left,
                    typename Index::value_type

                >::type,
                typename relation::support::data_extractor
                <
                    relation::member_at::right,
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
