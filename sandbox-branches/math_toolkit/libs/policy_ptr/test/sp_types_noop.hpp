//"NO OPeration" sp_types (used as arg to templates binary_node.hpp) for testing purposes
#ifndef LIBS_POLICY_PTR_TEST_SP_TYPES_SCOPED_HPP
#define LIBS_POLICY_PTR_TEST_SP_TYPES_SCOPED_HPP
#include <vector>
#include "source_auto_ptr.hpp"
#include "boost/utility/obj_id.hpp"

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
          
            struct
          count_prox
            : public utility::obj_id
          /**@brief
           *  Models part of the shared_count iterface
           */
          {
                  typedef
                utility::obj_id
              count_type
              ;
          
                count_type const*      
              count_ptr_con(void)const
              {
                  return this;
              }
          };
          
              typedef
            count_prox
          count_prox_type
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
            count_prox_type const&
          count_prox_con(void)const
          {
              return my_count_prox;
          }
       private:
            Referent*
          my_ptr
          ;
            count_prox_type
          my_count_prox
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
      
        struct
      collector
      {
              static
            void
          collect(void)
          {}
      };
      
  };//end sp_types_noop
  
}//exit boost namespace    
//ChangeLog:
//  2005-12-07 Larry Evans
//    WHAT:
//      Added sp_types_noop::collector
//    WHY:
//      To enable change mentioned in ChangeLog on 2005-08-15
//      in binary_node.hpp.
#endif //include guard
