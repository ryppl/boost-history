/* Copyright 2008 Vicente J. Botet Escriba
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/luid for library home page.
 */

#ifndef BOOST_LUID_ICCL_FIFO_HPP_
#define BOOST_LUID_ICCL_FIFO_HPP_

#include "boost/luid/iccl/backdoor.hpp"

#include <boost/intrusive/list.hpp>
#include <boost/mpl/assert.hpp>


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
struct fifo
{
    typedef Inner inner_type;
    typedef typename inner_type::config config;

    struct node_type : 
        inner_type::node_type, 
        boost::intrusive::make_list_base_hook<
            boost::intrusive::void_pointer<
                typename config::void_pointer_type
            >
        >::type 
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
        typedef typename config::node_type node_type;
        typedef typename config::allocator_type allocator_type;
//        typedef typename boost::intrusive::list<node_type, boost::intrusive::constant_time_size<false> > queue_type;
        typedef typename boost::intrusive::make_list<node_type>::type queue_type;

        template <typename ArgsParam> 
        header_type(const ArgsParam& args, allocator_type& allo) 
        : super_type(args, allo)
            , queue_() 
            {}
   
        queue_type& queue() { return queue_;}
        const queue_type& queue() const { return queue_;}
        queue_type queue_;
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
        
        typedef typename config::header_type::queue_type queue_type;
       
        queue_type& queue() { return super_type::header().queue(); }
        const queue_type& queue() const { return super_type::header().queue(); }
        
    public:
        bool valid_node(node_pointer_type n) const {
            return true;  
        }
    
        node_pointer_type make_node(typename config::numeric_type e) {
    
//            TODO void* n =super_type::allocate_node();
//            return new(n) node_type(e);
//            boost::detail::allocator::construct(&x->value(),v);
            node_pointer_type n = new node_type(e);
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
        : super_type(args) {}

        ~type() {
            clear();
        }
    
        numeric_type make() {
            if (queue().empty()) return super_type::make();

            node_pointer_type head = &queue().front();
            if (!backdoor::valid_node(super_type::final(),head)) {        
                return super_type::make();
            }
            queue().pop_front();
            numeric_type res = head->val_;
            dispose_node(head);
            return res;
        }
        void release(numeric_type n) {
            queue().push_back(*backdoor::make_node<node_pointer_type>(
                        super_type::final(), n));        
        }
    
        void clear() {
            queue().clear_and_dispose(disposer<final_type>(super_type::final()));
            super_type::clear();
        }
    
        numeric_type count() const {
            return super_type::count()-queue().size();
        }
    };
};

}
}
}
#endif // BOOST_LUID_ICCL_FIFO_HPP_
