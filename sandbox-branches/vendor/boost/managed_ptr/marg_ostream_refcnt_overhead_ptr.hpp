#ifndef MARG_OSTREAM_REFCNT_OVERHEAD_PTR_HPP
#define MARG_OSTREAM_REFCNT_OVERHEAD_PTR_HPP
#include "boost/io/filters/marg_ostream.hpp"
#include "boost/managed_ptr/refcycle_overhead_abs.hpp"
#include "boost/utility/object_tracked.hpp"
  marg_ostream&
marg_ostream_insert_refcnt_overhead_ref
  ( marg_ostream& sout
  , boost::managed_ptr::refcnt_overhead const& rc_oh
  )
  //Purpose:
  //  Replacement for corresponding operator<<.
  //  For some reason, the operator<<  isn't found by
  //  the compiler in
  //  boost/managed_ptr/curry_prox_visitor_refcycle_counted.hpp
  {
  ; unsigned int rc=rc_oh.as_count()
  ; sout<<":count="
  ; sout<<rc
  ; return sout
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcnt_overhead const& rc_oh
  )
  {
  ; return marg_ostream_insert_refcnt_overhead_ref(sout,rc_oh)
  ;}
  
  marg_ostream&
marg_ostream_insert_refcnt_overhead_ptr
  ( marg_ostream& sout
  , boost::managed_ptr::refcnt_overhead const* rc_oh
  )
  {
  ; if(rc_oh)
    {
    ; marg_ostream_insert_refcnt_overhead_ref(sout,*rc_oh)
    ;}
    else
    {
    ; sout<<"(rcnt_overhead*)0"
    ;}
  ; return sout
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcnt_overhead const* rc_oh
  )
  {
  ; return marg_ostream_insert_refcnt_overhead_ptr(sout,rc_oh)
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcnt_overhead* rc_oh
  )
  {
  ; return sout<<static_cast<boost::managed_ptr::refcnt_overhead const*>(rc_oh)
  ;}
  
  marg_ostream&
marg_ostream_insert_refcycle_overhead_abs_ref
  ( marg_ostream& sout
  , boost::managed_ptr::refcycle_overhead_abs const& rc_oh
  )
  {
  ; marg_ostream_insert_refcnt_overhead_ref(sout, static_cast<boost::managed_ptr::refcnt_overhead const&>(rc_oh))
  ; sout<<":status="
  ; sout.super_insert(rc_oh.status_get())
  ; sout<<":obj_id="
  ; void*l_ref=rc_oh.referent_start()
  ; utility::obj_id*l_oid=static_cast<utility::obj_id*>(l_ref)
  ; sout<<l_oid->id_get()
  ; return sout
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcycle_overhead_abs const& rc_oh
  )
  {
  ; return marg_ostream_insert_refcycle_overhead_abs_ref(sout,rc_oh)
  ;}
  
  marg_ostream&
marg_ostream_insert_refcycle_overhead_abs_ptr
  ( marg_ostream& sout
  , boost::managed_ptr::refcycle_overhead_abs const* rc_oh
  )
  {
  ; if(rc_oh)
    {
    ; marg_ostream_insert_refcycle_overhead_abs_ref(sout,*rc_oh)
    ;}
    else
    {
    ; sout<<"(rcnt_overhead*)0"
    ;}
  ; return sout
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcycle_overhead_abs const* rc_oh
  )
  {
  ; sout<<"op<<refcycle_overhead_abs const*\n"
  ; return marg_ostream_insert_refcycle_overhead_abs_ptr(sout,rc_oh)
  ;}
  
  marg_ostream&
operator<<
  ( marg_ostream& sout
  , boost::managed_ptr::refcycle_overhead_abs* rc_oh
  )
  {
  ; sout<<"op<<refcycle_overhead_abs*\n"
  ; return marg_ostream_insert_refcycle_overhead_abs_ptr(sout,rc_oh)
  ;}
  
#endif
