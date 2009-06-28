/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_COUNTER_HPP_
#define BOOST_LUID_ICCL_COUNTER_HPP_

#include "boost/integer_traits.hpp"
#include <boost/math/policies/error_handling.hpp>
#include "boost/mpl/push_front.hpp"
#include "boost/parameter.hpp"

namespace boost { namespace luid {

namespace iccl
{
    
/**
 * This is class implements the simple way to obtain a unique identifier, 
 * i.e. maintain a counter which will be increased each time a uid is made.
 * The uid could range on the possible values on the numeric_type, but the 
 * application can reduce this range, giving the low value on the low parameter 
 * and the high value.
 * The single problem with this approach is the uid which are no more used are 
 * discarded.
 * As a consequence, the limit will be reached at some time. 
 * This can be only a useful strategy if the limit is enough big related to the 
 * lifetime of the application. 
 * In any case when this limit is reached, there are two possibilities:
 * either return a invalid value representing that no more uid are available or 
 * throw an exception. This decision is not the responsability of this class, 
 * so this class delegates to the function on_overflow on the final 
 * instance which will do the rigth strategy.
 * A default implementation consists in returning the invalid value, which is 
 * equal to the limit. 
 */    

template <typename LowerLayer>
struct counter 
{
    typedef LowerLayer lower_layer;
    typedef typename lower_layer::config config;
    typedef typename lower_layer::node_type node_type;

    struct header_type :  lower_layer::header_type
    {
        typedef typename lower_layer::header_type super_type;
        typedef typename config::void_pointer_type void_pointer_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::allocator_type allocator_type;

        
        template <typename ArgsParam> 
        header_type(const ArgsParam& args, const allocator_type& allo) 
        : super_type(args, allo)
        , current_(super_type::lower_bound())
        {
        }

        numeric_type current_;
        // accesors
        numeric_type& current() {return current_;}
        numeric_type current() const {return current_;}
    };
    
    struct type : lower_layer::type
    {
        typedef typename lower_layer::type super_type;
        typedef typename config::numeric_type numeric_type;
    
        // forward construction to super type
        template <typename ArgsParam> 
        type(const ArgsParam& args) 
        : super_type(args)
        {
        }
    
        // accesors
        numeric_type& current() {return super_type::header().current();}
        numeric_type current() const {return super_type::header().current();}
        numeric_type count() const {return super_type::header().current();}
    
        numeric_type make(numeric_type& d) {
            if (super_type::upper_bound()-current()>d) {
                d=super_type::upper_bound()-current();
            }
            current() +=d;
            return current();
        }
    
        // monotonic counter
        numeric_type make() {
                return current()++;
        }    
        
        void release(numeric_type n) {
            // n must be less that the current value
            // assert(n< value());
        }

        bool on_overflow() const {
            return (current()==super_type::upper_bound());
        }    
        
        numeric_type set_upper_bound(numeric_type upper) {
            if (upper > current()) {
                super_type::upper_bound()=upper;
            }
            return super_type::upper_bound();
    
        }
    
        void clear() {
            current() = super_type::lower_bound();
        }
    
    };
};

} } }



#endif // BOOST_LUID_ICCL_COUNTER_HPP_
