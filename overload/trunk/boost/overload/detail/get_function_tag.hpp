/*=============================================================================
    Copyright (c) 2007 Marco Cecchetti

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef _BOOST_OVERLOAD_DETAIL_GET_FUNCTION_TAG_HPP
#define _BOOST_OVERLOAD_DETAIL_GET_FUNCTION_TAG_HPP


#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/ref.hpp>

#include "if_metafunction.hpp"

namespace boost{ namespace overloads{ namespace detail{

///////////////////////////////////////////////////////////////////////////////
// Tags used to decide between different types of functions
// This implementation is based on Douglas Gregor's code 
// see boost/function/function_base.hpp, I bring some minor change in.

struct function_ptr_tag {};
struct function_obj_tag {};
struct member_ptr_tag {};
struct function_obj_ref_tag {};

template<typename F>
class get_function_tag
{
    typedef typename if_c<(boost::is_pointer<F>::value),
                           function_ptr_tag,
                           function_obj_tag>::type ptr_or_obj_tag;

    typedef typename if_c<(boost::is_member_pointer<F>::value),
                           member_ptr_tag,
                           ptr_or_obj_tag>::type ptr_or_obj_or_mem_tag;

    typedef typename if_c<(boost::is_reference_wrapper<F>::value),
                           function_obj_ref_tag,
                           ptr_or_obj_or_mem_tag>::type or_ref_tag;

  public:
    typedef or_ref_tag type;
};


template<typename F>
struct is_ptr_or_memb_ptr
{
    static const bool value 
        = boost::is_pointer<F>::value 
        || boost::is_member_pointer<F>::value;
};

} } } // end namespaces

#endif // _BOOST_OVERLOAD_DETAIL_GET_FUNCTION_TAG_HPP

