//----------------------------------------------------------------------------
// boost mpl/structure/generator.hpp header file
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

#ifndef BOOST_MPL_STRUCTURE_GENERATOR_HPP
#define BOOST_MPL_STRUCTURE_GENERATOR_HPP

#include "boost/mpl/accumulate.hpp"
#include "boost/mpl/apply.hpp"
#include "boost/mpl/select_type.hpp"
#include "boost/mpl/null.hpp"

namespace boost {
namespace mpl {

namespace aux {

// Dave Abrahams' technique: factoring out 'struct_t'
// definition from 'struct_t_gen' (below) clears up the 
// generated type "signature"
template<typename BaseStruct, typename T>
struct struct_t : BaseStruct, T
{
    struct_t() {}

    template<typename U> struct_t(U const& other)
        : BaseStruct(other)
        , T(static_cast<T const&>(other))
    {
    }

    template<typename U> struct_t& operator=(U const& other)
    {
        BaseStruct::operator=(other);
        T::operator=(static_cast<T const&>(other));
        return *this;
    }
};

template<typename Derived>
struct struct_t_gen
{
    template<typename BaseStruct, typename T>
    struct apply
    {
     private:
        // another Dave's technique: 'select_type' is used to avoid 
        // a need to have/inherit from a dummy base class
        typedef typename mpl::apply1<T, Derived>::type member_;
     public:
        typedef typename boost::mpl::select_type<
              boost::is_same<BaseStruct, boost::mpl::null_t>::value
            , member_
            , struct_t<BaseStruct, member_>
            >::type type;
    };
};

} // namespace aux

template<typename Derived, typename Memebers>
struct structure_gen
{
    typedef typename boost::mpl::accumulate<
          Memebers
        , boost::mpl::null_t
        , aux::struct_t_gen<Derived>
        >::type type;
};

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_STRUCTURE_GENERATOR_HPP
