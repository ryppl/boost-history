/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_FACADE_HPP_
#define BOOST_LUID_ICCL_FACADE_HPP_

#include "boost/parameter.hpp"
#include <iostream>
namespace boost { namespace luid { namespace iccl {
    
/**
 * This is the outer most mixin class on the genvoca stack.
 * It provides the public interface.
 * */


template <typename Inner>
struct facade
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type :  protected inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef type facade_type;
        typedef typename super_type::node_type node_type;
    
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args) {}
    
        //     public operations
        type& facade() {
            return  *this;
        }
        const type& facade() const {
            return  *this;
        }
        
        //!makes a new luid
        //!Returns end() if no more luid available ot throw an exception 
        //!    depending on on_overflow policy.
        numeric_type make() {
            return super_type::make();
        }
    
        //!release a luid
        //!this luid could be reused by the make function 
        //!    depending on the on_release policy
        //!Could throws an exception if the luid was already released 
        //!    depending on check_is_free policy.
        void release(numeric_type n) {
            super_type::release(n);
        }
    
        //!return the non valid numeric value  
        numeric_type error() const {
            return super_type::error();
        }

        //!return the non valid numeric value  
        numeric_type upper_bound() const {
            return super_type::upper_bound();
        }
        //!return the non valid numeric value  
        numeric_type lower_bound() const {
            return super_type::lower_bound();
        }
        
        //!reset the upper bound  
        //!return the new upper bound  
        numeric_type set_upper_bound(numeric_type upper) {
               std::cout << " set_upper_bound facade b" << upper << std::endl;
            return super_type::set_upper_bound(upper);
        }
        
        //!reinit the luid generator as it was after construction  
        void clear() {
            super_type::clear();
        }
    
        numeric_type count() const {
            return super_type::count();
        }
        
    };
};

} } }
#endif // BOOST_LUID_FACADE_HPP_
