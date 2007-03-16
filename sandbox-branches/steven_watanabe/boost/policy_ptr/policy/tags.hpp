#ifndef BOOST_POLICY_PTR_POLICY_TAGS_HPP_INCLUDED
#define BOOST_POLICY_PTR_POLICY_TAGS_HPP_INCLUDED
namespace boost
{
    //------------------------------------------------------------------------
    // Policy Tags
    struct copy_rvalue_tag { }; // Included for clarity.
    struct copy_mutable_lvalue_tag { };
    struct copy_const_lvalue_tag { };
    struct copy_lvalue_tag 
        : copy_mutable_lvalue_tag, 
          copy_const_lvalue_tag
        { };
    struct copy_semantics_tag 
        : copy_rvalue_tag, 
          copy_lvalue_tag 
        { };
    struct move_semantics_tag : copy_rvalue_tag { };
    struct auto_semantics_tag 
        : copy_rvalue_tag, 
          copy_mutable_lvalue_tag 
        { };
    struct no_copy_semantics_tag { };

    struct storage_policy_tag { };
    struct ownership_policy_tag { };
    struct conversion_policy_tag { };
    struct checking_policy_tag { };
}//exit boost namespace
#endif
