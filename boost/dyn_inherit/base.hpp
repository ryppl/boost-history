//define base class for "dynamic inheritance"
#ifndef BOOST_DYN_INHERIT_BASE_HPP_LJE20040521
#define BOOST_DYN_INHERIT_BASE_HPP_LJE20040521
#include "virtual_dtor_abs.hpp"
    
namespace boost
{
namespace dyn_inherit
{
  class
base
//Purpose:
//  Serve as base class for classes which
//  can be used for "dynamic inheritance"
//  (see ./inherit.hpp).
  : public virtual_dtor_abs
{

 public:
    ~base(void)
    {
        delete my_subtype;
    }
    base(void)
    : my_subtype(0)
    {}
      virtual_dtor_abs*
    get_subtype(void)const
    {
        return my_subtype;
    }
      virtual_dtor_abs*
    put_subtype(virtual_dtor_abs* a_subtype)
    {
        delete my_subtype;
        my_subtype=a_subtype;
        return this;
    }
    
 private:
      virtual_dtor_abs*
    my_subtype
    //The "dynamic" subtype, if any, of this.
    ;

};//end base class

}//exit dyn_inherit namespace  
}//exit boost namespace  
#endif
