/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_DYNAMIC_LOWER_BOUND_HPP_
#define BOOST_LUID_ICCL_DYNAMIC_LOWER_BOUND_HPP_

#include "boost/integer_traits.hpp"
#include <boost/math/policies/error_handling.hpp>
#include "boost/mpl/push_front.hpp"
#include "boost/parameter.hpp"

#ifndef BOOST_LUID_ICCL_LOW
#define BOOST_LUID_ICCL_LOW
BOOST_PARAMETER_NAME(low)
#endif
namespace boost { namespace luid {

namespace iccl
{
    
template <typename LowerLayer>
struct dynamic_lower_bound 
{
    typedef LowerLayer lower_layer;
    typedef typename lower_layer::config config;
    typedef typename lower_layer::node_type node_type;

    struct header_type :  lower_layer::header_type
    {
        typedef typename lower_layer::header_type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::allocator_type allocator_type;

        
        template <typename ArgsParam> 
        header_type(const ArgsParam& args, const allocator_type& allo) 
        : super_type(args, allo)
        , lower_bound_(args[_low|boost::integer_traits<numeric_type>::const_min])
        {
        }

        const numeric_type lower_bound_;
        // accesors
//        numeric_type& lower_bound() {return lower_bound_;}
        numeric_type lower_bound() const {return lower_bound_;}
    
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
        numeric_type lower_bound() const {return super_type::header().lower_bound();}
    
    };
};

} } }



#endif // BOOST_LUID_ICCL_DYNAMIC_LOWER_BOUND_HPP_
