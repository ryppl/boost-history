/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_COHERENCY_BITSET_HPP_
#define BOOST_LUID_ICCL_COHERENCY_BITSET_HPP_

#include "boost/dynamic_bitset.hpp"
#include <exception>

namespace boost
{
namespace luid
{
namespace iccl
{

struct already_used : std::exception {
    virtual ~already_used() throw() {}
    virtual const char* what() const throw() {
      return "already_used";
    }
};

template <typename Inner>
struct coherency_bitset
{
    typedef Inner lower_layer;
    typedef typename lower_layer::config config;

    typedef typename lower_layer::node_type node_type;
    struct header_type :  lower_layer::header_type
    {
        typedef typename lower_layer::header_type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::allocator_type allocator_type;  
        typedef  dynamic_bitset<unsigned long,
                    typename boost::detail::allocator::rebind_to<
            typename config::allocator_type,
            unsigned long 
        >::type> bitset;
    
        template <typename ArgsParam> 
        header_type(const ArgsParam& args, allocator_type& allo) 
        : super_type(args, allo)
        , used_(super_type::upper_bound(), 0, allo)
        {
        }
        bitset& used() { return used_;}
        const bitset& used() const { return used_;}
        bitset used_;

    };
    struct type :  lower_layer::type
    {
        typedef typename lower_layer::type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::allocator_type allocator_type;  
//        typedef  dynamic_bitset<unsigned long,
//                    typename boost::detail::allocator::rebind_to<
//            typename config::allocator_type,
//            unsigned long 
//        >::type> bitset;
        typedef typename config::header_type::bitset bitset;
        
        bitset& used() { return super_type::header().used();}
        const bitset& used() const { return super_type::header().used();}
        bitset* used_;
    
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {
    //        std::cout << " coherency_bitset" << std::endl;
            
        }
    
//        ~type() {
//            used_->bitset::~bitset();
//            super_type::template deallocate<bitset, bitset*>(used_);
//        }
        
        bool is_in(numeric_type n) const {
            return used().test(n);
        }
    
        void remove(numeric_type n) {
            used().reset(n);  
        }
        
        void insert(numeric_type n) {
               std::cout << " insert coherency_bitset " << n << std::endl;
            used().set(n);
        }
        
        //!reset the upper bound  
        //!return the new upper bound  
        numeric_type set_upper_bound(numeric_type upper) {
            numeric_type n = super_type::set_upper_bound(upper);
//               std::cout << " set_upper_bound coherency_bitset b" << used().size() << std::endl;
            used().resize(n);
//               std::cout << " set_upper_bound coherency_bitset a" << used().size() << std::endl;
//            BOOSt_CHECK(used().size()==n);
            return n;
        }
        
        void clear() {
            super_type::clear();
            used().reset();
        }
        
        numeric_type number_of_reusable() {
            return used().count();
        }
    };
};

template <typename Inner>
struct coherency : public Inner
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;
    typedef typename inner_type::node_type node_type;
    typedef typename inner_type::header_type header_type;
    struct type :  inner_type::type
    {
        typedef typename inner_type::type super_type;
        typedef typename config::numeric_type numeric_type;
    
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args)
        {}
    
        numeric_type make() {
            numeric_type n = super_type::make();
            super_type::remove(n);
            return n;
        }
        
        void release(numeric_type n) {
            if(super_type::is_in(n)) {
                throw already_used();
            }
    
            super_type::release(n);
            super_type::insert(n);
        }
    };
};


} } }
#endif // BOOST_LUID_ICCL_COHERENCY_BITSET_HPP_
