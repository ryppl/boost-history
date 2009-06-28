/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_SET_HPP_
#define BOOST_LUID_ICCL_SET_HPP_

#include "boost/luid/iccl/backdoor.hpp"

#include <boost/intrusive/set.hpp>
#include <boost/mpl/assert.hpp>

#ifndef BOOST_LUID_ICCL_LOW
#define BOOST_LUID_ICCL_LOW
BOOST_PARAMETER_NAME(low)
#endif

#ifndef BOOST_LUID_ICCL_HIGH
#define BOOST_LUID_ICCL_HIGH

BOOST_PARAMETER_NAME(high)

#endif


namespace boost
{
namespace luid
{
namespace iccl
{

/**
 * This class implements the recoverable strategy. 
 * It store on a queue the uid which are no more needed by the application. 
 * It not store realy the uid, but an element_type which could be redefined by 
 * other layers, and that is obtained using the factory method make_element. 
 * When the application request an uid, the take function forwards the call to 
 * the lower layer when the queue is empty.
 * When non empty, and before to return the front element on the queue, the 
 * take function check if the element stored on the front of the queue is valid. 
 * When not valid the take function forwards the call to the lower layer.
 */    
template <typename Inner>
struct set
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;

    struct node_type : 
        inner_type::node_type, 
        boost::intrusive::set_base_hook<
            boost::intrusive::void_pointer<
                typename config::void_pointer_type
            >
        > 
    {
        typedef typename inner_type::node_type super_type;
        typedef typename config::numeric_type numeric_type;
        node_type(numeric_type v):val_(v) {}
        operator numeric_type() const {
            return val_;
        }         
        numeric_type val_;
    };

    struct header_type : inner_type::header_type { 
        typedef typename inner_type::header_type super_type;
        typedef typename config::numeric_type numeric_type;
        typedef typename config::node_type node_type;
        typedef typename config::allocator_type allocator_type;
//        typedef typename boost::intrusive::make_set<node_type, boost::intrusive::constant_time_size<false> > queue_type;
        typedef typename boost::intrusive::make_set<node_type>::type set_type;

        template <typename ArgsParam> 
        header_type(const ArgsParam& args, allocator_type& allo) 
        : super_type(args, allo)
        , lower_bound_(args[_low|boost::integer_traits<numeric_type>::const_min])
        , upper_bound_(args[_high|boost::integer_traits<numeric_type>::const_max]) 
        , set_() 
        {
        }
   
        set_type& set() { return set_;}
        const set_type& set() const { return set_;}
        // accesors
        numeric_type& lower_bound() {return lower_bound_;}
        numeric_type lower_bound() const {return lower_bound_;}
        numeric_type& upper_bound() {return upper_bound_;}
        numeric_type upper_bound() const {return upper_bound_;}
    
        numeric_type lower_bound_;
        numeric_type upper_bound_;
        set_type set_;
    };
    
    struct type : inner_type::type
    {
        friend class backdoor;
        typedef typename inner_type::type super_type;
        typedef typename config::final final_type;
        typedef typename config::numeric_type numeric_type;
    
        typedef typename config::node_type node_type;
        typedef typename config::node_pointer_type node_pointer_type;
        
        template <typename FINAL>
        struct disposer
        {
            FINAL& final;
           disposer(FINAL& f) : final(f) {}
           void operator()(node_pointer_type delete_this)
           {  final.dispose_node(delete_this);  }
        };
        
        typedef typename config::header_type::set_type set_type;
       
        set_type& set() { return super_type::header().set(); }
        const set_type& set() const { return super_type::header().set(); }
        
    public:
        node_pointer_type make_node(typename config::numeric_type e) {
                    std::cout <<    "BEGIN recoverable make_node" << std::endl;        
    
//            TODO void* n =super_type::allocate_node();
//            return new(n) node_type(e);
//            boost::detail::allocator::construct(&x->value(),v);
            node_pointer_type n = new node_type(e);
            std::cout <<    "END recoverable make_node" << std::endl;        
            return n;
//            return new node_type(e);
           
        }

        void dispose_node(node_pointer_type n) {
          n->~node_type();
          super_type::deallocate_node(n);
        }

        
        // forwards construction
        template <typename ArgsParam>              
        type(const ArgsParam& args) 
        : super_type(args) {
            for (numeric_type i = lower_bound(); i<upper_bound(); ++i) {
                node_pointer_type ptr = backdoor::make_node<node_pointer_type>(
                        super_type::final(), i);
                std::pair< typename set_type::iterator, bool > res = set().insert(*ptr);
            }
        }
        ~type() {
            clear();
        }
        // accesors
        numeric_type& upper_bound() {return super_type::header().upper_bound();}
        numeric_type upper_bound() const {return super_type::header().upper_bound();}
        numeric_type lower_bound() const {return super_type::header().lower_bound();}
        numeric_type get_upper_bound() const {return super_type::header().upper_bound();}
        numeric_type error() const {
        return std::numeric_limits<numeric_type>::has_infinity 
            ? std::numeric_limits<numeric_type>::infinity() 
            : boost::math::tools::max_value<numeric_type>();
        }

        bool on_overflow() const {
                return (set().empty());
        }    
        
        numeric_type make() {
            typename set_type::iterator head = set().begin();
            numeric_type res = head->val_;
            set().erase_and_dispose(head, disposer<final_type>(super_type::final()));
            return res;
        }

        void release(numeric_type n) {
            node_pointer_type ptr = backdoor::make_node<node_pointer_type>(
                    super_type::final(), n);
            std::pair< typename set_type::iterator, bool > res = set().insert(*ptr);
            if (!res.second) {
                throw already_used();
            } else {
                super_type::final().dispose_node(ptr);
            }
        }
    
        void clear() {
            set().clear_and_dispose(disposer<final_type>(super_type::final()));
//            super_type::clear();
        }
    
        numeric_type set_upper_bound(numeric_type upper) {
            if (upper > upper_bound()) {
                for (numeric_type i = upper_bound(); i<upper; ++i) {
                    node_pointer_type ptr = backdoor::make_node<node_pointer_type>(
                            super_type::final(), i);
                    std::pair< typename set_type::iterator, bool > res = set().insert(*ptr);
                }
                upper_bound()=upper;
            }
            return upper_bound();
    
        }

        numeric_type count() const {
            return set().size();
        }
    };
};

}
}
}
#endif // BOOST_LUID_ICCL_FIFO_HPP_
