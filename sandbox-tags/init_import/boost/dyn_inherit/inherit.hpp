//enable "dynamic inheritance", i.e. inherit from base class pointer.
#ifndef BOOST_DYN_INHERIT_INHERIT_HPP_LJE20040223
#define BOOST_DYN_INHERIT_INHERIT_HPP_LJE20040223
//  (C) Copyright Larry Joe Evans 2004.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
#include "base.hpp"
    
namespace boost
{
namespace dyn_inherit
{
template
  < typename Referent
  >
class wrap_ptr
//Purpose:
//  A simple smart_ptr to serve as a supertype
//  in inherit which is defined below.
{
 public:
        typedef
      Referent
    ref_type
    ;
    
    wrap_ptr(ref_type* a_ref)
    : my_ref(0)
    {
        my_ref=a_ref;
    }
    
      ref_type const*
    get_ref(void)const
    { 
        return my_ref;
    }
    
      ref_type*
    get_ref(void)
    { 
        return my_ref;
    }
    
 private:
 
      ref_type*
    my_ref
    ;
};//end wrap_ptr<Referent>

template
  < typename DynSuperType
    //the dynamic supertype.
    //REQUIREMENTS:
    //  derived from dyn_inherit::base.
  , typename DeltaType
    //the "delta" or additions to DynSuperType which form this_type.
  , template<typename>class Proxy=wrap_ptr
  >
  class 
inherit
  : public base
  , public Proxy<DynSuperType> 
    //smart pointer to "dynamic" super type.
  , public DeltaType 
//Purpose:
//  Enable instances of DynSuperType to be "decorated" (in the sense of
//  the GOF Decorator pattern) with a DeltaType dynamically.
//  The difference from the Decorator pattern is:
//    1)The DynSuperType (which corresponds to the Component participant 
//      in the GOF Decorator pattern) is not inherited by this class
//      (which corresponds with the Decorator participant in the
//      Decorator pattern) but is simply pointed to by the
//      Proxy<DynSuperType> superclass, which corresponds to the
//      "component" reference in the Decorator class diagram.
//    2)The DynSuperType has a pointer back to this as it's subtype part.
//
//  This class also embodies some of the features of the GOF strategy
//  pattern in that the DynSuperType (which corresponds with the Context
//  participant in the strategy pattern) has a pointer to the this
//  class (which corresponds to the Strategy paricipant in the
//  strategy pattern).
{
 public:
        typedef 
      Proxy<DynSuperType>
    super_type
    ;
        typedef
      inherit
      < DynSuperType
      , DeltaType
      , Proxy
      >
    this_type
    ;
    
    inherit
    ( DynSuperType* a_super
    )
    : super_type
      ( static_cast<DynSuperType*>(a_super->put_subtype(this))
      ) //tell super_type that this is its subtype
    {}
    
      DynSuperType const*
    get_supertype
    ( void
    )const
    {
        return super_type::get_ref();
    }
      DynSuperType*
    get_supertype
    ( void
    )
    {
        return super_type::get_ref();
    }
        static
      this_type*
    get_subtype
    ( DynSuperType* a_super
    )
    {
        void*l_vp=a_super->get_subtype();
        this_type*l_tp=static_cast<this_type*>(l_vp);
        return l_tp;
    }
 private:
    inherit
    ( inherit const&
    )
    ;
};
}//exit dyn_inherit namespace  
}//exit boost namespace  
#endif
