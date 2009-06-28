/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_REC_DELAY_HPP_
#define BOOST_LUID_ICCL_REC_DELAY_HPP_

#include <queue>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/luid/iccl/timestamped.hpp"


BOOST_PARAMETER_NAME(delay)    // Note: no semicolon

namespace boost { namespace luid {

 
namespace iccl {

template <typename Inner>
struct rec_fifo_delay
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    struct node_type 
        : timestamped<typename config::time_traits,    
                        typename inner_type::node_type>    
    {
        typedef timestamped<
            typename config::time_traits,    
            typename inner_type::node_type> super_type;
        typedef typename config::time_traits time_traits;
        typedef typename time_traits::duration_type duration_type;
        typedef typename config::numeric_type numeric_type;

        node_type(const duration_type delay, const numeric_type e) 
        : super_type(delay, e)
        {
        }
    };

    struct header_type  : inner_type::header_type
    {
        typedef typename inner_type::header_type super_type;
        typedef typename config::time_traits time_traits;
        typedef typename time_traits::duration_type duration_type;
        typedef typename config::allocator_type allocator_type;
        
        template <typename ArgsParam> 
        header_type(const ArgsParam& args, allocator_type& allo) 
        : super_type(args, allo)
        , delay_(args[_delay|time_traits::def()]) 
        {}
   
        duration_type delay() { return delay_;}
        duration_type delay_;
    };
    struct type : inner_type::type
    {
        friend class backdoor;
        typedef typename inner_type::type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::time_traits time_traits;
        typedef typename time_traits::duration_type duration_type;
        typedef typename config::node_type node_type;
        typedef typename config::node_pointer_type node_pointer_type;
                    
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {
        }
            
        node_type* make_node(numeric_type e) {
            //std::cout <<    "**** make_node" << std::endl;        
            // TODO threre is a problems with this new expression
            return new node_type(super_type::header().delay(), e);
            
//            return new(super_type::allocate_node()) 
//                        node_type(super_type::header().delay(), e);
        }
    
        bool valid_node(node_pointer_type e) const {
            return !e->alive() && super_type::valid_node(e);  
        }
    };
};

} } }

#endif // BOOST_LUID_ICCL_REC_DELAY_HPP_
