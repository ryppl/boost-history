//subclass of ref_counted which is able to collect cycles
#ifndef BOOST_MANAGED_PTR_REFCYCLE_COUNTED_CURRY_PROX_VISITOR_HPP_LJE20031009
#define BOOST_MANAGED_PTR_REFCYCLE_COUNTED_CURRY_PROX_VISITOR_HPP_LJE20031009
//  (C) Copyright Larry Evans 2003.
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
#include "boost/managed_ptr/basis_specializer4refcycle_counted_curry_prox_visitor.hpp"
#include "boost/managed_ptr/refcnt_ovhd_prox_referent.hpp"
#include "boost/managed_ptr/refcycle_overhead_abs.hpp"
#include "boost/managed_ptr/prox_children.hpp"
#include "boost/managed_ptr/smart_ptr.hpp"
#include "boost/type_traits/remove_pointer.hpp"
namespace boost
{
namespace managed_ptr
{

template
  < typename Referent
  , template<typename>class Overhead
  >
    template
      < template<typename>class StoragePolicy
      , template<typename>class OwnershipPolicy
      >
auto_overhead
  < Referent
  , Overhead
  >
  ::
    auto_overhead
      ( detail::resource_manager
        < StoragePolicy<Referent>
        , OwnershipPolicy<Referent*> 
        >& a_rm
      )
    : super_type
      ( get_impl(static_cast<StoragePolicy<Referent>&>(a_rm))
      , static_cast<OwnershipPolicy<Referent*>&>(a_rm).release_overhead()
      )
    {
        typedef StoragePolicy<Referent> storage_policy;
        storage_policy& l_store=a_rm;
        get_impl_ref(l_store) = storage_policy::default_value();
    }
    
template<typename ProxVisitor>
      class 
refcycle_counted_curry_prox_visitor<ProxVisitor>::
    owner_void
        : public refcnt_ovhd_prox_referent<void*>
    {
     public:
                typedef 
            refcnt_ovhd_prox_referent<void*>
        super_type
        ;
                typedef 
            owner_void
        this_type
        ;
                typedef 
            refcycle_overhead_described_abs<ProxVisitor>
        overhead_type
        ;
                typedef
            typename ProxVisitor::prox_iter_desc_type
        prox_iter_desc_type
        ;
        
     protected:
     
        owner_void
            ( overhead_type* a_ovhd
            )
            : super_type(a_ovhd)
        {
        }
        
     public:
     
        
        ~owner_void
            ( void
            )
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("~owner_void(void)");
            #endif
            if(super_type::does_count_exist() && risky_use_count(*this) == 0)
            {
                refcycle_overhead_abs* l_ovhd=get_overhead();
                super_type::reset(0);
                #ifdef TRACE_SCOPE_HPP
                mout()<<"~owner_void(void):delete overhead="<<l_ovhd<<std::endl;
                #endif
                delete l_ovhd;
            };
        }
    
        owner_void
            ( void
            )
        {
            overhead_type*l_ovhd=this->get_overhead();
          #ifdef DO_TRACE_SCOPE
            mout()<<"+owner::CTOR(void):overhead="<<l_ovhd<<std::endl;
          #endif
        }
    
        owner_void(this_type const& rhs)
            : super_type(rhs)
        {
          #ifdef DO_TRACE_SCOPE
            overhead_type*l_ovhd=this->get_overhead();
            mout()<<"+owner::CTOR(this_type const&):overhead="<<l_ovhd<<std::endl;
          #endif
        }
        
        owner_void(this_type& rhs)
            : super_type(rhs)
        {
            refcycle_overhead_abs*l_ovhd=this->get_overhead();
          #ifdef DO_TRACE_SCOPE
            mout()<<"+owner::CTOR(this_type&):overhead=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<std::endl;
          #endif
        }
        
     public:
         
            overhead_type* 
        get_overhead(void)
        {
            overhead_type* l_ovhd = 0;
            typename super_type::overhead_type* l_super_ovhd = super_type::get_overhead();
            if(l_super_ovhd)
            {
                l_ovhd = static_cast<overhead_type*>(l_super_ovhd);
            }
            return l_ovhd;
        }
        
            overhead_type* 
        operator->(void)
        {
            return get_overhead();
        }
        
            typedef
          refcycle_counted_curry_prox_visitor<ProxVisitor>
        nesting_type
        ;
        
            bool 
        release
            ( void*
            )
        //Paraphrase:
        //  1) decrement ref_count 
        //     if non-zero,
        //       do local mark-scan
        //         if true
        //           delete ref_count and referent
        //     otherwise:
        //       delete ref_count and referent
        //  2) ALWAYS return false
        //  3) ALWAYS release the overhead
        //Rationale:
        //  2) this indicates to storage_policy to
        //     release referent; hence, NOT delete
        //     it.  This is because, in this
        //     owner policy's case, it is the owner
        //     responsible for deleting the referent
        //     (since it was allocated in the same
        //     memory block as the ref_count).
        //  3) to prevent deletion by ~super_type
        //     since it's deleted here, if needed.
        {
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("owner::release(ReferentPtr const&)");
          #endif
            overhead_type*l_ovhd=get_overhead();
          #ifdef TRACE_SCOPE_HPP
            mout()<<":entry l_ovhd=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<std::endl;
          #endif
            if(l_ovhd == 0) return false;
            l_ovhd->decrement();
            bool keep=l_ovhd->as_count()>0;
          #ifdef TRACE_SCOPE_HPP
            mout()<<":b4 no_cycle_garbage:keep="<<keep<<":l_ovhd=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<"\n";
          #endif
            if(keep)
            {
                keep = no_cycle_garbage();
            }
          #ifdef TRACE_SCOPE_HPP
            mout()<<":af keep="<<keep<<":l_ovhd=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<"\n";
          #endif
            prevent_dtor_delete();//prevent deletion by ~super_type().
            if(!keep)
            {
              #ifdef TRACE_SCOPE_HPP
                mout()<<"delete overhead="<<static_cast<refcycle_overhead_abs*>(l_ovhd)<<std::endl;
              #endif
                delete l_ovhd;
            }
            return false;
        }
    
            typename overhead_type::count_type 
        break_cycle
            ( void
            )
        //Purpose:
        //  Break a cycle in smart_ptr's.
        //Requires:
        //  Should only be called by something that has already determined
        //  that this count is part of a cycle, and the cycle is to be 
        //  broken at this point.
        {
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("break_cycle");
          #endif
            overhead_type*l_ovhd=get_overhead();
            typename overhead_type::count_type result=l_ovhd->as_count();
            prevent_dtor_delete();
          #ifdef TRACE_SCOPE_HPP
            mout()<<"break_cycle():overhead=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<"\n";
          #endif
            return result;
        }
        
            class
        iterator_child
        : public prox_iter_desc_type
        {
         public:
            iterator_child(void)
            {}
            iterator_child(owner_void& a_owner)
                : prox_iter_desc_type
                  ( a_owner->referent_start()
                  , a_owner->descriptor_con()
                  )
            {}
        };    
        
     private:
            typedef
          typename ProxVisitor::prox_iter_acc_type
        prox_iter_acc_type
        ;
            typedef
          typename ProxVisitor::void_owner_type
        void_owner_type
        ;
            class
        remove_internal
            : public ProxVisitor
        {
         public:
            
                virtual 
              void 
            visit_children
            ( void_owner_type& a_void_owner
            , prox_iter_acc_type& a_iter
            )
            //DEFINITIONS:
            //  internal pointers of a_void_owner: 
            //    these are the pointers reachable from a_void_owner->referent_start().
            //  external pointers of a_void_owner:
            //    all other pointers, including root pointers.
            //  dead:
            //    applied to a referent which is garbage.
            //  live:
            //    applied to a not-dead referent.
            //WHAT:
            //  remove reference counts due to interal pointers.
            //WHY:
            //  This determines which referents (pointees) are
            //  garbage.  Any such referents that have 0 reference
            //  counts after this removal means they are not referenced
            //  by any external pointers, which in turn means they
            //  cannot be accessed even indirectly from any root
            //  pointers; hence, they're dead.
            {
              #ifdef TRACE_SCOPE_HPP
                utility::trace_scope ts("remove_internal::visit_children");
              #endif
                overhead_type* l_ovhd = a_void_owner.get_overhead();
              #ifdef TRACE_SCOPE_HPP
                mout()<<"remove_internal:l_ovhd=";
                marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                mout()<<"\n";
              #endif
                if(l_ovhd->status_get() != refcycle_status::rm_internal)
                {
                    l_ovhd->decrement();
                    l_ovhd->status_put(refcycle_status::rm_internal);
                  #ifdef TRACE_SCOPE_HPP
                    mout()<<"remove_internal:status change=";
                    marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                    mout()<<"\n";
                  #endif
                    accept_each(a_iter);
                }
            }

        };//end remove_internal class

            class
        restore_internal
            : public ProxVisitor
        {
         public:
                virtual 
              void 
            visit_children
            ( void_owner_type& a_void_owner
            , prox_iter_acc_type& l_iter
            )
            //Purpose:
            //  Mark a_void_owner as live and restore reference counts 
            //  and mark all objects accessible from a_void_owner as live. 
            {
              #ifdef TRACE_SCOPE_HPP
                utility::trace_scope ts("restore_internal");
              #endif
                overhead_type* l_ovhd = a_void_owner.get_overhead();
              #ifdef TRACE_SCOPE_HPP
                mout()<<"restore_internal:l_ovhd=";
                marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                mout()<<"\n";
              #endif
                if(l_ovhd->status_get() != refcycle_status::is_done)
                {
                    l_ovhd->status_put(refcycle_status::is_done);
                    l_ovhd->increment();
                  #ifdef TRACE_SCOPE_HPP
                    mout()<<"restore_internal:status change=";
                    marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                    mout()<<"\n";
                  #endif
                    accept_each(l_iter);
                }
            }
                    
        };//end restore_internal class

            class
        filter_live
            : public ProxVisitor
        {
         public:
                virtual 
              void 
            visit_children
            ( void_owner_type& a_void_owner
            , prox_iter_acc_type& l_iter
            )
            //Purpose:
            //  Filter live from dead store_owner's.
            {
              #ifdef TRACE_SCOPE_HPP
                utility::trace_scope ts("filter_live");
              #endif
                overhead_type* l_ovhd = a_void_owner.get_overhead();
              #ifdef TRACE_SCOPE_HPP
                mout()<<"filter_live:l_ovhd=";
                marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                mout()<<"\n";
              #endif
                if(l_ovhd->status_get() == refcycle_status::rm_internal)
                {
                    if(l_ovhd->as_count() > 0)
                    {
                        restore_internal l_visitor;
                        l_visitor.visit_children(a_void_owner,l_iter);
                    }
                    else
                    {
                        l_ovhd->status_put(refcycle_status::is_dead);
                      #ifdef TRACE_SCOPE_HPP
                        mout()<<"filter_live:status change=";
                        marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                        mout()<<"\n";
                      #endif
                        accept_each(l_iter);
                    }
                }
            }
            
        };//end filter_live class

            class
        break_cycles
            : public ProxVisitor
        {
         public:
                virtual 
              void 
            visit_children
            ( void_owner_type& a_void_owner
            , prox_iter_acc_type& l_iter
            )
            //Purpose:
            //  Similar to restore_internal except where cycles
            //  are found.  In that case, the cycle is broken and
            //  the count is not incremented.
            {
              #ifdef TRACE_SCOPE_HPP
                utility::trace_scope ts("break_cycles");
              #endif
                overhead_type* l_ovhd = a_void_owner.get_overhead();
              #ifdef TRACE_SCOPE_HPP
                mout()<<"break_cycles:l_ovhd=";
                marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                mout()<<"\n";
              #endif
                if(l_ovhd->status_get() == refcycle_status::is_dead)
                {
                    l_ovhd->status_put(refcycle_status::brk_cycles);
                    l_ovhd->increment();
                  #ifdef TRACE_SCOPE_HPP
                    mout()<<"break_cycles:increment status change=";
                    marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                    mout()<<"\n";
                  #endif
                    accept_each(l_iter);
                }
                else if(l_ovhd->status_get() == refcycle_status::brk_cycles)
                {
                    l_ovhd->decrement();
                  #ifdef TRACE_SCOPE_HPP
                    mout()<<"break_cycles:decrement status change=";
                    marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                    mout()<<"\n";
                  #endif
                    a_void_owner.break_cycle(); //break refcount cycle
                  #ifdef TRACE_SCOPE_HPP
                    mout()<<"break_cycles:broken cycle=";
                    marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
                    mout()<<"\n";
                  #endif
                }
            }
            
        };//end break_cycles class

            bool 
        no_cycle_garbage
            ( void
            )
        //Purpose:
        //  do local mark-scan of subgraph starting here.  If *this is
        //  garbage, break any cycles in subgraph to avoid double
        //  deletion during "normal" refcount operation and return false;
        //  otherwise, return true (meaning "keep" *this).
        {
        
            this_type& l_me=*this;
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("owner_void::no_cycle_garbage()");
          #endif
            overhead_type* l_ovhd = get_overhead();
          #ifdef TRACE_SCOPE_HPP
            mout()<<"l_ovhd=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<"\n";
          #endif
            bool keep = l_ovhd->status_get() != refcycle_status::is_done;
            if(!keep)
            {
                {
                    remove_internal l_visitor;
                    iterator_child l_iter(l_me);
                    l_visitor.accept_each(l_iter);
                }
                {
                    filter_live l_visitor;
                    l_visitor.visit_prox(l_me);
                }
                keep = l_ovhd->as_count() > 0;
                if(!keep)
                {
                    break_cycles l_visitor;
                    l_visitor.visit_prox(l_me);
                }
            }
            return keep;
            
        }//end no_cycle_garbage
        
    
    };//end refcycle_counted_curry_prox_visitor<ProxVisitor>::owner_void class
    
template
  < typename Referent
  , typename StoragePolicy
  , typename OwnershipPolicy
  , typename ProxVisitor
  >
  class
cycle_basis_mgr_nester
  < Referent
  , StoragePolicy
  , OwnershipPolicy
  , refcycle_counted_curry_prox_visitor<ProxVisitor>
  >
: private prox_children<ProxVisitor>::builder_materials::make_recorder
{
 public:
        typedef 
      detail::resource_manager
      < StoragePolicy
      , OwnershipPolicy
      >
    resource_mgr_type
    ;
        typedef 
      typename prox_children<ProxVisitor>::builder_materials::make_recorder 
    recorder_type
    ;
        typedef
      Referent
    referent_type
    ;
        typedef
      refcycle_counted_curry_prox_visitor<ProxVisitor>
    nesting_type
    ;
        typedef
      basis_specializer
        < referent_type
        , nesting_type
        >
    basis_spec_type
    ;
        typedef 
      typename basis_spec_type::overhead_type
    overhead_type
    ;
        typedef 
      typename basis_spec_type::basis_source_type
    basis_source_type
    ;
        typedef 
      typename basis_spec_type::basis_sink_type
    basis_sink_type
    ;
    cycle_basis_mgr_nester(OwnershipPolicy* a_so)
    : recorder_type(a_so)
    {
    }
        static 
      void 
    release_to_basis
      ( resource_mgr_type& source
      , basis_sink_type& target
      )
    {
      #ifdef TRACE_SCOPE_HPP
        utility::trace_scope ts("cycle_basis_manager_nester<...,refcycle_counted_curry_prox_visitor<ProxVisitor> >::release_to_basis");
      #endif
        typename OwnershipPolicy::overhead_type* l_ovhd = source.get_overhead();
      #ifdef TRACE_SCOPE_HPP
        mout()<<"l_ovhd="<<std::endl;
        marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
        mout()<<"\n";
      #endif
        OwnershipPolicy::on_release(source);
        basis_sink_type l_sink(source);
        target = l_sink;
    }

};
  
template<typename ProxVisitor>
    template <typename ReferentPtr>
      class 
refcycle_counted_curry_prox_visitor<ProxVisitor>::
    owner
    : public owner_void
    {
     public:
    
            typedef 
          owner_void
        super_type
        ;
            typedef 
          owner<ReferentPtr> 
        this_type
        ;
            typedef 
          typename remove_pointer<ReferentPtr>::type
        referent_type
        ;
            typedef
          basis_specializer
            < referent_type
            , refcycle_counted_curry_prox_visitor<ProxVisitor>
            >
        basis_spec_type
        ;
            typedef 
          typename basis_spec_type::basis_source_type
        this_source_type
        ;
            typedef 
          typename basis_spec_type::basis_sink_type
        this_sink_type
        ;
            typedef 
          typename basis_spec_type::overhead_type
        overhead_type
        ;
            typedef 
          this_type 
        type
        ;
            friend
        this_sink_type
        ;
        
     public:
     
        ~owner(void)
        {
        #if defined(TRACE_SCOPE_HPP)
            utility::trace_scope ts("~owner<ReferentPtr>(void)");
        #endif
        }
        
        owner(void)
        {}

        owner
          ( this_source_type const& 
              a_basis_src
          )
        : super_type(a_basis_src.release_overhead())
        {
        }

        template<typename SubRef>
        owner
//#define DEMO_PROB_CXX_STD_14_8_2_4_P4
#ifdef DEMO_PROB_CXX_STD_14_8_2_4_P4
    //Purpose:
    //  Show a problem imposed by inability to deduce template arguments
    //  when they're used to name nested classes.  This problem is
    //  revealed by compilation of /libs/managed_ptr/test/sub_cpy_ctor.cpp
    //  with the above #define uncommented.
    //Reference:
    //  c++ standard 14.8.2.4 "Deducing template arguments from a type"
    //  paragraph 4.
          ( typename basis_spec_type::rebind<SubRef>::other::basis_source_type const&
#else
          ( auto_refwrap
            < SubRef
            , refcycle_overhead_described_curry_prox_visitor<ProxVisitor>
              ::template overhead_concrete
            > const&
#endif          
              a_basis_src
          )
        : super_type(a_basis_src.release_overhead())
        {
        }
        
        template <typename U>
        owner(owner<U> const& rhs)
    # ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : super_type(rhs)
    # else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        : super_type(reinterpret_cast<this_type const&>(rhs))
    # endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
        {
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("owner::CTOR(owner<U>const&)");
            mout()<<":overhead=";
            marg_ostream_insert_refcycle_overhead_abs_ptr( mout(), this->get_overhead());
            mout()<<"\n";
          #endif
        }
        
            ReferentPtr 
        clone(ReferentPtr const& val)
        {
            super_type::clone(val);
            return val;
        }

            bool 
        reset
            ( void
            )
        {
            if (use_count(*this) > 0) return true;
            overhead_type* l_ovhd = new overhead_type;
            typename super_type::overhead_type* l_super_ovhd = l_ovhd;
            super_type::reset(l_super_ovhd);
            return false;
        }
        #if 0
            bool 
        reset
            ( this_source_type const& a_basis_src
            )
        {
            bool result = this->release(0);
            overhead_type* l_ovhd = a_basis_src.release_overhead();
            l_ovhd->increment();
            mout()<<"reset(this_source_type const&):l_ovhd=";
            marg_ostream_insert_refcycle_overhead_abs_ptr(mout(),l_ovhd);
            mout()<<std::endl;
            typename super_type::overhead_type* l_super_ovhd = l_ovhd;
            super_type::reset(l_super_ovhd);
            return result;
        }
        #endif
            overhead_type* 
        get_overhead(void)
        {
            overhead_type* l_ovhd = 0;
            typename super_type::overhead_type* l_super_ovhd = super_type::get_overhead();
            if(l_super_ovhd)
            {
                l_ovhd = static_cast<overhead_type*>(l_super_ovhd);
            }
            return l_ovhd;
        }
        
            overhead_type* 
        operator->(void)
        {
            return get_overhead();
        }
        
     private:
            overhead_type*
        release_overhead(void)
        {
            overhead_type*l_ovhd=get_overhead();
            this->prevent_dtor_delete();
            l_ovhd->decrement();
            return l_ovhd;
        }
        
    }
    //end refcycle_counted_curry_prox_visitor<ProxVisitor>::owner<ReferentPtr> 
    ;
    
}//exit managed_ptr namespace
}//exit boost namespace
//////////////////////////////////////////////////////////////////////////////
// ChangeLog:
//   2003-12-05: Larry Evans
//     WHAT:
//       "merged" with download from latest, i.e. renamed cycle_mgr to
//       resource_manager and made changes to conform to downloaded
//       version.
//   2003-12-06: Larry Evans
//     WHAT:
//       reverted name back to cycle_mgr.
//     WHY:
//       it's easier to make cycle_mgr as superclass of revised 
//       resource_manager.
//   2003-12-08: Larry Evans
//     mv'ed definition of local_mark_scan here.  Renamed to no_cycle_garbage.
//   2004-03-08: Larry Evans
//     made changes to accommodate merging of cycl_mgr and release_mgr template
//     classes into cycle_basis_mgr template class.
//   2004-06-20: Larry Evans
//     WHAT:
//       renamed from curry_prox_visitor_refcycle_counted
//     WHY:
//       See ChangeLog: on this date in the *.fpp file.
//////////////////////////////////////////////////////////////////////////////
#endif
