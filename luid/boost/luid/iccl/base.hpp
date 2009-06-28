//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/luid for documentation.
//
//////////////////////////////////////////////////////////////////////////////


#ifndef BOOST_LUID_BASE_HPP_
#define BOOST_LUID_BASE_HPP_

namespace boost
{
namespace luid
{
namespace iccl
{

/**
 * This is the base template on the GenVoca hierarchy. It must define
 * the config type
 * constructor from the args parameter
 */

template <typename Generator>
struct base {
    typedef typename Generator::config config;
    struct node_type {};
    struct header_type {
        typedef typename config::allocator_type allocator_type;
        template <typename ArgsParam>              
        header_type(const ArgsParam& args, const allocator_type& allo) {}
    };
    struct type {
        typedef typename config::final final_type;
        template <typename ArgsParam>              
        type(const ArgsParam& args) {} 
    
        final_type& final() {
            return  *static_cast<final_type*>(this);
        }
        const final_type& final() const {
            return  *static_cast<const final_type*>(this);
        }
    };
};


} // iccl
} // luid
} // boost
#endif // BOOST_LUID_BASE_HPP_



