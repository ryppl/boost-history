//----------------------------------------------------------------------------
// boost mpl/structure/member.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_STRUCTURE_MEMBER_HPP
#define BOOST_MPL_STRUCTURE_MEMBER_HPP

#include "boost/mpl/apply.hpp"

namespace boost {
namespace mpl {

template<typename T, typename MemberTag>
struct member
{
    template<typename Derived>
    struct apply
    {
        typedef typename mpl::apply2<
              MemberTag
            , T
            , Derived
            >::type type;
    };
};

} // namespace mpl
} // namespace boost

#define BOOST_MPL_STRUCTURE_GEN_MEMBER(name)\
struct name\
{\
    template<typename T, typename Derived>\
    struct member\
    {\
        member(T const& x = T()) : m_##name(x) {}\
        Derived& name(T const& x) { m_##name = x; return static_cast<Derived&>(*this); }\
        T const& name() const { return m_##name; }\
    private:\
        T m_##name;\
    };\
    template<typename T, typename Derived>\
    struct apply { typedef member<T, Derived> type; };\
} \
/**/

#endif // BOOST_MPL_STRUCTURE_MEMBER_HPP
