#include "boost/managed_ptr/prox_visitor_refcycle_abs.hpp"
#include "boost/managed_ptr/curry_prox_visitor_refcycle_counted.hpp"
namespace boost{namespace managed_ptr{

prox_visitor_refcycle_abs::
    prox_visitor_refcycle_abs
    ( unsigned a_max_depth
    )
    : my_max_depth(a_max_depth)
    , my_cur_depth(0)
    {
    }
    
      void
prox_visitor_refcycle_abs::
    accept_each
    ( prox_iter_acc_type& a_iter
    )
    {
        //utility::trace_scope ts("prox_visitor_refcycle_abs::accept_each");
        for(; !a_iter.empty(); a_iter.increment())
        {
            //utility::trace_scope ts("accept_each.for body");
            a_iter.accept(*this);
        }
    }
    
      template<typename Value>
        void 
prox_visitor_refcycle_abs::
      visit_prox
      ( Value& a_val
      )
      { 
          utility::trace_scope("template<Value>::visit_prox(Value&)");
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
prox_visitor_refcycle_abs::
      visit_prox
      ( void_owner_type& a_void_owner
      )
      {
          utility::trace_scope ts("visit_prox(void_owner_type&)");
          ++my_cur_depth;
          if(my_cur_depth > my_max_depth) //just debugging tool. Absent in production code
          {
              mout()<<"max recur depth="<<my_max_depth<<" exceeded\n";
              return;
          }
          if(a_void_owner.does_count_exist())
          {
              utility::trace_scope ts("visit_prox(void_owner_type&).if(is_valid)");
              void_owner_type::iterator_child l_iter(a_void_owner);
              visit_children(a_void_owner,l_iter);
          }
          --my_cur_depth;
      }
}}//exit boost::managed_ptr namespace
