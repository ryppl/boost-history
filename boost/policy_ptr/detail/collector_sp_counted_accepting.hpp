//precise collector for refcounted pointers based on sp_counted_accepting.
#ifndef BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_LJE20050523
#define BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_LJE20050523
#include "boost/policy_ptr/detail/collector.hpp"
#include "boost/policy_ptr/detail/shared_count_sp_counted_accepting.hpp"
#include <algorithm>
#include <deque>

//#define BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
namespace boost
{
namespace policy_ptr
{
namespace detail
{

  template
  <
  >
  class
collector
  < sp_counted_accepting
  >
/**@brief
 *  Collector for smart pointers with detached refcounts of type
 *  sp_counted_accepting.
 */
{
 private:
        typedef
      sp_counted_accepting
    vertex_type
    ;
        typedef
      vertex_type::this_set_type
    vertex_set_type
    ;
        typedef
      shared_count<vertex_type>
    arc_type
    ;
        typedef
      std::deque<arc_type*>
    arc_set_type
    ;
 public:
        static
      void
    collect(void)
    /**
     * @reference
     *  [chri84] on http://www.cs.kent.ac.uk/people/staff/rej/gcbib/gcbibC.html
     *
     */
    {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("collector_sp_counted_accepting::collect");
            mout()<<"#vertices="<<vertex_type::our_instances.size()<<"\n";
#endif
        vertex_set_type::iterator iter_end=vertex_type::our_instances.end();
        vertex_set_type::iterator iter_beg=vertex_type::our_instances.begin();
        {   
           /**@brief
             *  For each vertex in vertex_type::our_instances,
             *  decrements the ref count of each child of that
             *  vertex.
             */
            remove_internal visitor;
            std::for_each(iter_beg,iter_end,visitor);
        }        
        {   
            /**@brief
             *  For each vertex in vertex_type::our_instances,
             *  if refcount > 0, restores the refcount for that vertex and 
             *  all vertices reachable from that to the value before
             *  remove_internal decremented the counts.  Also,
             *  labels the other vertices with is_dead status.
             */
            filter_live visitor;
            std::for_each(iter_beg,iter_end,visitor);
        }        
        {
            /**@brief
             *  Deletes all is_dead vertices.
             *  
             * @reference
             *  Although [chri84] recovers the memory, it does not
             *  call destructors.  The code in this block does this
             *  by breaking the cycles at some arc in the cycle, thus
             *  recoverying the memory via the normal c++ means of
             *  calling delete on pointers.  However, this is at
             *  the cost of uncertainty in the programmer's mind
             *  about where the cycle might be broken.
             */
            arc_set_type cycle_arcs;
            {
                /**@brief
                 *  Restores refcount of all is_dead vertices
                 *  but also notes, in cycle_arcs, where cycles
                 *  are closed.
                 *
                 * @rationale
                 *  The cycles cannot be broken yet, because to do
                 *  so would modify vertex_type::our_instances and
                 *  thus render the iterators invalid.  Hence,
                 *  cycle_arcs is needed.
                 */
                find_cycles visitor(cycle_arcs);
                std::for_each(iter_beg,iter_end,visitor);
            }
            {
                /**@brief
                 *  Breaks all arcs in cycle_arcs.  A side effect of
                 *  this breaking of cycles is that the graph becauses
                 *  a dag; hence, the normal memory recovery method of
                 *  refcounting comes into play, and memory is recovered.
                 */
                break_cycle breaker;
                std::for_each(cycle_arcs.begin(),cycle_arcs.end(),breaker);
            }
        }        
    }
    
 private:
      struct
    break_cycle
    {
          void
        operator()(arc_type* a_arc)
        {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("break_cycle::operator()(arc_type* a_arc)");
#endif
            arc_type l_arc;
            a_arc->swap(l_arc);
        }
    };
    
      struct
    remove_internal
      : public shared_count_visitor_abs
      , private gc_strong_count_decrementer
    {
          void
        checked(vertex_type*vertex_ptr)
        {
            if(vertex_ptr->status_ != refcycle_status::rm_internal)
            {
                vertex_ptr->status_ = refcycle_status::rm_internal;
                vertex_ptr->accept_each(*this);
            }
        }
        
          void
        operator()(vertex_type*vertex_ptr)
        {
            checked(vertex_ptr);
        }
        
     private:
          void
        visit_count(arc_type& a_arc)
        {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("remove_internal::visit_count");
#endif
            vertex_type* vertex_ptr=get_counter(a_arc);
            decrement_strong(*vertex_ptr);
        }
    };
      struct
    restore_internal
      : public shared_count_visitor_abs
    {
          void
        checked(vertex_type*vertex_ptr)
        {
            if(vertex_ptr->status_ != refcycle_status::is_done)
            {
                unchecked(vertex_ptr);
            }
        }
          void
        unchecked(vertex_type*vertex_ptr)
        {
            vertex_ptr->status_ = refcycle_status::is_done;
            vertex_ptr->accept_each(*this);
        }
        
     private:
          void
        visit_count(arc_type& a_arc)
        {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("restore_internal::visit_count");
#endif
            vertex_type* vertex_ptr=get_counter(a_arc);
            vertex_ptr->add_ref_copy();
            checked(vertex_ptr);
        }
    };
      struct
    filter_live
      : public shared_count_visitor_abs
    {
          void
        checked(vertex_type*vertex_ptr)
        {
            if(vertex_ptr->status_ == refcycle_status::rm_internal)
            {
                long use_count=vertex_ptr->use_count();
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
                utility::trace_scope ts("filter_live::checked:==rm_internal");
                mout()<<"use_count="<<use_count<<"\n";
#endif                
                if(use_count > 0)
                {
                    my_restorer.unchecked(vertex_ptr);
                }
                else
                {
                    vertex_ptr->status_ = refcycle_status::is_dead;
                    vertex_ptr->accept_each(*this);
                }
            }
        }
        
          void
        operator()(vertex_type*vertex_ptr)
        {
            checked(vertex_ptr);
        }
        
     private:
          void
        visit_count(arc_type& a_arc)
        {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("filter_live::visit_count");
#endif
            vertex_type* vertex_ptr=get_counter(a_arc);
            checked(vertex_ptr);
        }
          restore_internal
        my_restorer
        ;
    };
      struct
    find_cycles
      : public shared_count_visitor_abs
    {
        find_cycles(arc_set_type& a_cycles)
          : my_cycles(a_cycles)
        {}
          bool
        checked(vertex_type*vertex_ptr)
        {
            bool do_break=false;
            if(vertex_ptr->status_ == refcycle_status::is_dead)
            {
                vertex_ptr->status_ = refcycle_status::brk_cycles;
                vertex_ptr->add_ref_copy();
                vertex_ptr->accept_each(*this);
            }
            else if(vertex_ptr->status_ == refcycle_status::brk_cycles)
            {
                do_break=true;
            }
            return do_break;
        }
        
          void
        operator()(vertex_type*vertex_ptr)
        {
            checked(vertex_ptr);
        }
        
     private:
          arc_set_type&
        my_cycles
        ;
          void
        visit_count(arc_type& a_arc)
        {
#ifdef  BOOST_POLICY_PTR_DETAIL_COLLECTOR_SP_COUNTED_ACCEPTING_HPP_TRACE
            utility::trace_scope ts("find_cycles::visit_count");
#endif
            vertex_type* vertex_ptr=get_counter(a_arc);
            if(checked(vertex_ptr))
            {
               my_cycles.push_back(&a_arc);
            }
        }
    };
};

}//exit detail namespace
}//exit policy_ptr namespace
}//exit boost namespace
#endif
