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
struct subinp //derived class of inparm
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
        
          struct
        inparm_source
        {
                typedef
              inparm&
            referent_type
            ;
              referent_type
            my_ref
            ;
            inparm_source
              ( referent_type a_ref
              )
            : my_ref(a_ref)
            {}
        };
        
        template
          < typename Referent
          , typename OuterParm
          >
          struct 
        basis_specializer
          < Referent
          , outer_yes_tmpl<OuterParm>
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
            
              struct
            basis_source_type
            : public inparm_source
            {
                basis_source_type(referent_type a_ref)
                  : inparm_source(a_ref)
                {}
                
                template
                  < typename SuperRef
                  >
                  rebind<SuperRef>::other::basis_source_type
                as_super(void)
                {
                    typedef rebind<SuperRef>::other::basis_source_type supr_type;
                    inparm_source& l_vs=*this;
                    supr_type& l_ss = static_cast<supr_type&>(l_vs);
                    return l_ss;
                }
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

    smart_ptr
      ( basis_source_type a_basis
      )
      : my_basis(a_basis)
    {}
    
      basis_source_type
    my_basis
    ;
    
};

}//exit boost namespace
using namespace boost;    
int main(void)
{
    {
    typedef smart_ptr<inparm,non_nested_tmpl> sp_base_type;
    inparm*l_base_ptr=new inparm;
    sp_base_type::basis_source_type l_base_src=l_base_ptr;
    sp_base_type l_base_fr_base(l_base_src);
    typedef smart_ptr<subinp,non_nested_tmpl> sp_sub_type;
    subinp*l_sub_ptr=new subinp;
    sp_sub_type::basis_source_type l_sub_src=l_sub_ptr;
    sp_base_type l_base_fr_sub(l_sub_src);
    }
    {
    typedef outer_yes_tmpl<outparm> out_tmpl;
    typedef smart_ptr<inparm,out_tmpl::yes_nested_tmpl> sp_base_type;
    inparm l_base_ref;
    sp_base_type::basis_source_type l_base_src=l_base_ref;
    sp_base_type l_base_fr_base(l_base_src);
    typedef smart_ptr<subinp,out_tmpl::yes_nested_tmpl> sp_sub_type;
    subinp l_sub_ref;
    sp_sub_type::basis_source_type l_sub_src=l_sub_ref;
    sp_base_type l_base_fr_sub(l_sub_src.as_super<inparm>());
    }
    return 0;
}    

