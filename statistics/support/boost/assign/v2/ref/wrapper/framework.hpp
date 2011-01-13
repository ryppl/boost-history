//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_WRAPPER_FRAMEWORK_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_WRAPPER_FRAMEWORK_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
namespace ref{

	template<typename Tag,typename T>
    struct wrapper{
        // specialize on Tag
    };

namespace result_of{
	template<typename Tag, typename T>
    struct wrap{
        typedef ref::wrapper<Tag,T> type;
    };
}


template<typename Tag, typename T>
inline ref::wrapper<Tag,T>
wrap(T & t)
{
    typedef ref::wrapper<Tag,T> result_;
    return result_( t );
}

template<typename Tag, typename T>
inline ref::wrapper<Tag, T const>
wrap(T const & t)
{
    typedef ref::wrapper<Tag,T const> result_;
    return result_( t );
}

}// ref
}// v2
}// assign
}// boost

#include <boost/mpl/bool.hpp>
#include <boost/ref.hpp>

// The part below extends boost/ref.hpp.

namespace boost{

#  define AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(U) \
template<typename Tag,typename T> \
class is_reference_wrapper<U> \
    : public mpl::true_ \
{ \
}; \
\
template<typename Tag, typename T> \
class unwrap_reference<U> \
{ \
 public: \
    typedef T type; \
}; \
/**/

#define u assign::v2::ref::wrapper<Tag, T>

AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(u)
#if !defined(BOOST_NO_CV_SPECIALIZATIONS)
AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(u const )
AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(u volatile )
AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF(u const volatile )
#endif

#undef u
#undef AUX_REFERENCE_WRAPPER_METAFUNCTIONS_DEF

}// boost

#endif
