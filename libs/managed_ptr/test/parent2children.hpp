//person with 2 children of same type(i.e. a binary tree) stored in smart pointers
#include "chk_dbl_die.hpp"
#include "basis_adaptor.hpp"
#include "boost/managed_ptr/smart_ptr.hpp"
#ifdef TEST_STLCONT
#include "boost/managed_ptr/stlcont_prox_extern/scoped_cyclic_container.hpp"
#include <vector>
#endif
namespace boost
{

  using mpl::_;
  namespace parent2children
  {
  
    template
      < template<typename Referent> class OwnershipPolicy
      >
    class parent_scalar
    : public chk_dbl_die
    {
     public:
            typedef
          parent_scalar<OwnershipPolicy>
        this_type
        ;
            typedef 
          smart_ptr<this_type, scalar_storage<_>, OwnershipPolicy<_>, assert_check<_> > 
        sp_type
        ;
            typedef 
          managed_ptr::basis_adaptor<OwnershipPolicy<this_type*> > 
        basis_adapt_type
        ;
          sp_type
        my_left_sp_child
        ;
          sp_type
        my_right_sp_child
        ;
          sp_type&
        left(void)
        {
            return my_left_sp_child;
        }
          sp_type&
        right(void)
        {
            return my_right_sp_child;
        }
          sp_type const&
        left(void)const
        {
            return my_left_sp_child;
        }
          sp_type const&
        right(void)const
        {
            return my_right_sp_child;
        }
    };//end parent_scalar
    
  #ifdef TEST_STLCONT
    template
      < template<typename Referent> class OwnershipPolicy
      >
    class parent_vector
    : public chk_dbl_die
    {
     public:
            typedef
          parent_vector<OwnershipPolicy>
        this_type
        ;
            typedef 
          smart_ptr<this_type, scalar_storage<_>, OwnershipPolicy<_>, assert_check<_> > 
        sp_type
        ;
            typedef 
          managed_ptr::basis_adaptor<OwnershipPolicy<this_type*> > 
        basis_adapt_type
        ;
            typedef
          managed_ptr
          ::stlcont_prox_extern
          ::scoped_cyclic_container
            < std::vector
            , sp_type
            >
        container_type
        ;
          container_type
        my_sp_children
        ;
        
        ~parent_vector(void)
        {
          #ifdef BOOST_MANAGED_PTR_TRACE_MODE
            mout()<<"parent_vector-"<<":this="<<this<<":obj_id="<<id_get()<<"\n";
          #endif
        }
        
        parent_vector(void)
        : my_sp_children(2)
        {
          #ifdef BOOST_MANAGED_PTR_TRACE_MODE
            mout()<<"parent_vector+"<<":this="<<this<<":obj_id="<<id_get()<<"\n";
          #endif
        }
          sp_type&
        left(void)
        {
            return my_sp_children[0];
        }
          sp_type&
        right(void)
        {
            return my_sp_children[1];
        }
          sp_type const&
        left(void)const
        {
            return my_sp_children[0];
        }
          sp_type const&
        right(void)const
        {
            return my_sp_children[1];
        }
    };//end parent_vector
  #endif  
  }//exit parent2children namespace   
}//exit boost namespace
