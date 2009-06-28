/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ON_OVERFLOW_HPP_
#define BOOST_LUID_ON_OVERFLOW_HPP_

#include <exception>
#include <ostream>
#include <boost/math/policies/error_handling.hpp>

namespace boost { namespace luid {

struct overflow : std::exception {
    virtual ~overflow() throw() {}
    virtual const char* what() const throw() {
      return "overflow";
    }
};

namespace iccl {

template <typename Inner>
struct return_error_value_on_overflow
{    
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
//        friend class backdoor;
        typedef typename config::numeric_type numeric_type;
        
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {
        }
    
        numeric_type make() {
            if (super_type::on_overflow()) {
                return super_type::error();
            } else {
                return super_type::make();
            }
        }    
    };
};

template <typename Inner>
struct errno_on_overflow
{    
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
//        friend class backdoor;
        typedef typename config::numeric_type numeric_type;
        
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {
        }
    
        numeric_type make() {
            if (super_type::on_overflow()) {
                errno = ERANGE;
                return super_type::error();
            } else {
                return super_type::make();
            }
        }    
    };
};


template <typename Inner>
struct throw_exception_on_overflow
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type : inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::numeric_type numeric_type;
       
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {}
        
        numeric_type make() {
            if (super_type::on_overflow()) {
                throw overflow();
            } else {
                return super_type::make();
            }
        }    
    };
};

} } }

#endif // BOOST_LUID_ON_OVERFLOW_HPP_
