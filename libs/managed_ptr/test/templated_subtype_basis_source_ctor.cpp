//Purpose:
//  Attempt to reproduce error in smart_ptr_test.cpp
//  at sub_reset<child>(q, l_basis_adapt_der_src) in
//  copy_test where l_basis_adapt_der_src is a 
//  basis_source_type for a derived class of q's referent.
//
//  The versions used were:
//    smart_ptr_test.cpp.~1.12~
//    smart_ptr.hpp.~1.21~
//
//Conclusion:
//  The problem occurs because the templated CTOR argument type is:
//
//    typename basis_spec_type::rebind<SubRef>::other::basis_source_type
//
//  and since rebind<SubRef> is a templated qualifier, it can't
//  participate in template argument deduction according to:
//
//     c++ standard 14.8.2.4 "Deducing template arguments from a type"
//     paragraph 4.
//
#include "boost/managed_ptr/basis_specializer.hpp"
#include "boost/managed_ptr/rm_nondeduced.hpp"
struct outparm{};//outer template parameter
struct inparm{};//inner template parameter
struct subinparm //derived class of inparm
: public inparm
{};

template<typename MaybeNestTparm>
struct non_nested_tmpl
{
};

template<typename OuterTparm>
struct outer_yes_tmpl
{
    template<typename MaybeNestTparm>
    struct yes_nested_tmpl
    {
    };
};

namespace boost
{
    namespace managed_ptr
    {
        namespace rm_nondeduced
        {
    
            template
              < typename Referent
              >
            struct nester_of
              < outer_yes_tmpl<outparm>::yes_nested_tmpl<Referent*>
              >
            {
                typedef outer_yes_tmpl<outparm> type;
            };
          
        }//exit rm_nondeduced namespace
        
        template
          < typename Referent
          , typename OuterParm
          >
          struct 
        basis_specializer
          < Referent
          , typename outer_yes_tmpl<OuterParm>::template yes_nested_tmpl<Referent*>
          >
        {
                typedef
              Referent
            referent_type
            ;
                typedef
              outer_yes_tmpl<OuterParm>
            nesting_type
            ;
                typedef 
              referent_type&
            basis_source_type
            ;
            
            template
              < typename Other
              >
              struct
            rebind
            {
                    typedef
                  basis_specializer<Other,nesting_type>
                other
                ;
            };
       };//end basis_specializer<Referent      
    
    }//exit managed_ptr namespace
    
template<typename Referent, template<typename>class OwnershipPolicy>
class smart_ptr
{
 public:
        typedef 
      smart_ptr<Referent,OwnershipPolicy> 
    this_type
    ;
        typedef
      OwnershipPolicy<Referent*>
    ownership_policy
    ;
        typedef
      typename managed_ptr::basis_specializer
        < Referent
        , managed_ptr::rm_nondeduced::nester_of<ownership_policy>::type
        >
    basis_spec_type
    ;
        typedef
      typename basis_spec_type::basis_source_type
    basis_source_type
    ;

  #if 0
  
    smart_ptr
      ( typename basis_source_type a_basis
      )
      : my_basis(a_basis)
    {}
    
  #else
  
    template
      < typename SubRef
      >
    smart_ptr
      ( typename basis_spec_type::rebind<SubRef>::other::basis_source_type a_basis
      )
      : my_basis(a_basis)
    {}
    
  #endif
    
      basis_source_type
    my_basis
    ;
    
};

}//exit boost namespace
using namespace boost;    
int main(void)
{
    inparm* a_in=new inparm;
    typedef smart_ptr<inparm,non_nested_tmpl> sp_non_nested_type;
    sp_non_nested_type a_non_nested(a_in);
    return 0;
}    

