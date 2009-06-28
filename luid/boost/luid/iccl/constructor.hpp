/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_CONSTRUCTOR_HPP_
#define BOOST_LUID_ICCL_CONSTRUCTOR_HPP_

#include "boost/parameter.hpp"

namespace boost { namespace luid { namespace iccl {
    
/**
 * This is the outer most mixin class on the genvoca stack.
 * It provides the public interface.
 * */


template <typename Inner>
struct constructor
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
    
        //! constructor with name parameters
        BOOST_PARAMETER_CONSTRUCTOR(
                type, (super_type), tag
          , (optional (low,*)) (optional (high,*)) 
              (optional (delay,*)) 
              (optional (shm_name,*)) (optional (shm_size,*))
              (optional (ext_mtx,*)) 
        ) // no semicolon

//        template <typename ArgsParam> 
//        luidg(const ArgsParam& args) 
//        : super_type(args)
//        {
//        }
    };
};

} } }
#endif // BOOST_LUID_ICCL_CONSTRUCTOR_HPP_
