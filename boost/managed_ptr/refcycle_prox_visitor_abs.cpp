#include "boost/managed_ptr/refcycle_prox_visitor_abs.hpp"
#include "boost/managed_ptr/refcycle_counted_curry_prox_visitor.hpp"
namespace boost{namespace managed_ptr{

refcycle_prox_visitor_abs::
    refcycle_prox_visitor_abs
    ( unsigned a_max_depth
    )
    : my_max_depth(a_max_depth)
    , my_cur_depth(0)
    {
    }
    
      void
refcycle_prox_visitor_abs::
    accept_each
    ( prox_iter_acc_type& a_iter
    )
    {
        //utility::trace_scope ts("refcycle_prox_visitor_abs::accept_each");
        for(; !a_iter.empty(); a_iter.increment())
        {
            //utility::trace_scope ts("accept_each.for body");
            a_iter.accept(*this);
        }
    }
    
      template<typename Value>
        void 
refcycle_prox_visitor_abs::
      visit_prox
      ( Value& a_val
      )
      { 
        #ifdef TRACE_SCOPE_HPP
          utility::trace_scope("template<Value>::visit_prox(Value&)");
        #endif
          ++my_cur_depth;
          if(my_cur_depth > my_max_depth)
          {
              mout()<<"max recur depth="<<my_max_depth<<" exceeded\n";
              return;
          }
          typedef typename prox_children<this_type>
            ::detail_iterators::prox_iterator_con_of<Value> iter_type;
          iter_type l_iter(&a_val);
          accept_each(l_iter);
          --my_cur_depth;
      }

        void 
refcycle_prox_visitor_abs::
      visit_prox
      ( void_owner_type& a_void_owner
      )
      {
        #ifdef TRACE_SCOPE_HPP
          utility::trace_scope ts("visit_prox(void_owner_type&)");
        #endif
          ++my_cur_depth;
          if(my_cur_depth > my_max_depth) //just debugging tool. Absent in production code
          {
            #ifdef TRACE_SCOPE_HPP
              mout()<<"max recur depth="<<my_max_depth<<" exceeded\n";
            #endif
              return;
          }
          if(a_void_owner.does_count_exist())
          {
            #ifdef TRACE_SCOPE_HPP
              utility::trace_scope ts("visit_prox(void_owner_type&).if(is_valid)");
            #endif
              void_owner_type::iterator_child l_iter(a_void_owner);
              visit_children(a_void_owner,l_iter);
          }
          --my_cur_depth;
      }
}}//exit boost::managed_ptr namespace
