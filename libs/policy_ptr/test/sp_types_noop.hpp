//"NO OPeration" sp_types (used as arg to templates binary_node.hpp) for testing purposes
#ifndef LIBS_POLICY_PTR_TEST_SP_TYPES_SCOPED_HPP
#define LIBS_POLICY_PTR_TEST_SP_TYPES_SCOPED_HPP
#include <vector>
#include "source_auto_ptr.hpp"

namespace boost
{

  struct sp_types_noop
  {
        template<class Referent>
        struct
      source
      {
              typedef 
            source_auto_ptr<Referent>
          type
          ;
      };
      
        template<class Referent>
        struct
      scalar
      {
       public:
              typedef 
            scalar
          type
          ;
          scalar(Referent* a_ptr=0)
            : my_ptr(a_ptr)
          { }
          
            Referent*
          get(void)const
          {
              return my_ptr;
          }
            Referent*
          operator->(void)const
          {
              return get();
          }
              friend
            Referent*
          get_impl(scalar const& a_scalar)
          {
              return a_scalar.get();
          }
       private:
            Referent*
          my_ptr
          ;
      };
      
        template<class Value>
        struct
      container
      {
              typedef 
            std::vector
            < Value
            >
          type
          ;
      };
      
  };//end sp_types_noop
  
}//exit boost namespace    

#endif //include guard
