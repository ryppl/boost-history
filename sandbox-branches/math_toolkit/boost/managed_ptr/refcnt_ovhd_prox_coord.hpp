//reference count proxy coordinated with smart ptr class
#ifndef BOOST_MANAGED_PTR_REFCNT_OVHD_PROX_COORD_LJE20031109_HPP
#define BOOST_MANAGED_PTR_REFCNT_OVHD_PROX_COORD_LJE20031109_HPP
#include "boost/managed_ptr/refcnt_overhead.hpp"
#include <stdexcept>                    // std::runtime_error
namespace boost{namespace managed_ptr{

    struct bad_release : public std::runtime_error
    {
        bad_release() : std::runtime_error("Bad Release: Too many references")
        { }
    };

    class refcnt_ovhd_prox_coord
    //Purpose:
    //  Proxy for reference count used in "coordination" through
    //  refcnt_ovhd_prox_referent with resource_manager class 
    //  defined in smart_ptr.hpp.
    {
    public:
        typedef refcnt_overhead overhead_type;
        
    protected:
        
        ~refcnt_ovhd_prox_coord()
        //REQUIRES:
        //  this->deletable()
        { 
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("~refcnt_ovhd_prox_coord()");
            mout()<<":count_="<<count_<<":deletable="<<deletable()<<"\n";
          #endif
            delete count_; 
        }

        refcnt_ovhd_prox_coord()
        : count_(0)
        {
        }

        refcnt_ovhd_prox_coord(overhead_type* subtype_overhead)
        : count_(subtype_overhead)
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("refcnt_ovhd_prox_coord(overhead_type*)");
            #endif
            increment();
        }
        
        refcnt_ovhd_prox_coord(refcnt_ovhd_prox_coord const& rhs)
        : count_(rhs.count_)
        {
            #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("refcnt_ovhd_prox_coord(this_type const&)");
            #endif
            //MAINTENANCE_NOTE:
            //  call to increment() not done here because it's done by
            //  subclass, refcnt_ovhd_prox_referent<T*>::clone(T*const&),
            //  which is called by 
            //    template<typename U,...>smart_ptr(smart_ptr<U,...> const&rhs)
        }
        
        void swap(refcnt_ovhd_prox_coord& rhs)
        {
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("refcnt_ovhd_prox_coord::swap");
            mout()<<"this count_="<<(void*)count_<<"\n";
            mout()<<"that count_="<<(void*)rhs.count_<<"\n";
          #endif
            std::swap(count_, rhs.count_);
        }
        
        bool deletable(void)const
        {
            return  ( count_ == 0 )
                 || ( count_->as_count() == 0)
                 ;
        }
        
        bool releasable(void)const
        {
            return  ( count_ == 0 )
                 || ( count_->as_count() == 1)
                 ;
        }
        
    public:
        static void on_release(refcnt_ovhd_prox_coord& sp)
        //Purpose:
        //  Prevent release while there remain references.
        {
            if (!sp.releasable()) 
            {
              #ifdef TRACE_SCOPE_HPP
                mout()<<"on_release::bad_release():use_count="<<use_count(sp)<<"\n";
              #endif
                throw bad_release();
            }
        }

        static void reset(refcnt_ovhd_prox_coord& sp)
        {
            if (sp.count_)
            {
                sp.decrement();
                sp.prevent_dtor_delete();
            }
        }

        friend inline overhead_type::count_type risky_use_count(refcnt_ovhd_prox_coord const& p)
        { 
            return p.count_->as_count(); 
        }

        bool does_count_exist()const
        {
            return count_ != 0;
        }

        friend inline overhead_type::count_type use_count(refcnt_ovhd_prox_coord const& p)
        { 
            return p.does_count_exist() ? risky_use_count(p) : 0; 
        }

    public:
        bool is_count_valid()const
        {
            return does_count_exist() && count_->is_valid();
        }

        bool operator==(refcnt_ovhd_prox_coord const& rhs)
        {
            return count_ == rhs.count_;
        }

        bool operator!=(refcnt_ovhd_prox_coord const& rhs)
        {
            return !(this->operator==(rhs));
        }
        
    protected:
        //allow use of next methods by
        //  curry_prox_visitor_refcycle_counted<ProxVisitor>::owner<ReferentPtr>:
        
        void increment()
        {
            if(does_count_exist()) count_->increment();
        }
        
        overhead_type::count_type decrement()
        {
            overhead_type::count_type l_val=0;
            if(does_count_exist())
            {
                count_->decrement();
                l_val = count_->as_count();
            }
            return l_val;
        }
        
        bool release()
        //Purpose:
        //  Indicates whether this is the last release; hence,
        //  ~storage_policy should delete it's pointee.
        //
        //  See ~resource_manager.
        //Paraphrase:
        //  if count is already released:
        //    return true.
        //  otherwise:
        //    decrement count and 
        //    if result is zero:
        //      DO NOT release count
        //      (so that ~refcnt_ovhd_prox_coord will delete it)
        //      and return true.
        //    otherwise:
        //      DO release count
        //      (so that ~refcnt_ovhd_prox_coord will NOT delete it)
        //      and return false.
        {
          #ifdef TRACE_SCOPE_HPP
            utility::trace_scope ts("refcnt_ovhd_prox_coord::release");
            mout()<<"b4 count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),count_);
            mout()<<"\n";
          #endif
            if (!decrement()) return true;
            prevent_dtor_delete(); //i.e. release count
          #ifdef TRACE_SCOPE_HPP
            mout()<<"af count_=";
            marg_ostream_insert_refcnt_overhead_ptr(mout(),count_);
            mout()<<"\n";
          #endif
            return false;
        }
        
        void reset(overhead_type* a_ovhd)
        //used by refcycle_counted
        {
            reset(*this);
            count_ = a_ovhd;
            count_->increment();
        }
        
        overhead_type* get_overhead()const
        {
            return count_;
        }

        void prevent_dtor_delete()
        //REQUIRES:
        //  !(this->deletable())
        //CONTEXT:
        //  ~refcnt_ovhd_prox_coord will "soon" be called.
        //  (for example, after this->release(ReferentPtr) is
        //  called in ~resource_manager() ).
        //WHAT:
        //  Prevent delete of count_ by the DTOR.
        //WHY:
        //  The count is shared by someone else; hence,
        //  it must not be deleted by DTOR.
        {
            count_ = 0;
        }
        
    private:
        refcnt_ovhd_prox_coord& operator=(refcnt_ovhd_prox_coord const&);

        // Data
        overhead_type* count_;
        
    };//end refcnt_ovhd_prox_coord class

}}//exit boost::managed_ptr namespace
#endif
