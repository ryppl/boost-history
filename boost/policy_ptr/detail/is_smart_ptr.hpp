//compile-time test for if type is a smart_ptr
#ifndef BOOST_POLICY_PTR_DETAIL_IS_SMART_PTR_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_IS_SMART_PTR_HPP_INCLUDED
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

//----------------------------------------------------------------------------
// Utility Macros
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
# define BOOST_SP_CONVERT_ARGUMENT(Policy, obj, field)                       \
    ((obj).field)
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type, arity)                \
    template <BOOST_PP_ENUM_PARAMS(arity, class Param)>                      \
    friend class type;
# define BOOST_SP_PRIVATE private
#else // BOOST_NO_MEMBER_TEMPLATE_FRIENDS
# define BOOST_SP_CONVERT_ARGUMENT(policy, obj, field)                       \
    (reinterpret_cast<policy const&>(obj).field)
# define BOOST_SP_DECLARE_TEMPLATE_FRIEND(Param, type, arity)
# define BOOST_SP_PRIVATE public
#endif // BOOST_NO_MEMBER_TEMPLATE_FRIENDS

namespace boost
{
    namespace policy_ptr
    {
    namespace detail
    {
        //--------------------------------------------------------------------
        struct empty_policy
        {
            typedef mpl::void_ policy_category;
        };

    }//exit boost namespace
    }//exit policy_ptr namespace
    //------------------------------------------------------------------------
    // Type generator smart_ptr
    template <
        class P1 = policy_ptr::detail::empty_policy,
        class P2 = policy_ptr::detail::empty_policy,
        class P3 = policy_ptr::detail::empty_policy,
        class P4 = policy_ptr::detail::empty_policy
    >
    struct smart_ptr
    {
        template <typename T>
        class to;
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(to, T, 1)
        BOOST_SP_DECLARE_TEMPLATE_FRIEND(smart_ptr, Q, 4)
    BOOST_SP_PRIVATE:
        template <typename SmartPtr>
        static typename SmartPtr::stored_type 
        clone(const SmartPtr& ptr, const typename SmartPtr::stored_type& p)
        {
            return ptr.do_clone(p);
        }

        template <typename SmartPtr>
        static typename SmartPtr::stored_type 
        clone(const SmartPtr& ptr, typename SmartPtr::stored_type& p)
        {
            return ptr.do_clone(p);
        }
    };
    
    namespace policy_ptr
    {
    namespace detail
    {
        //BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_enclosing_class, enclosing_class, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_first_policy, first_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_second_policy, second_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_third_policy, third_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_fourth_policy, fourth_policy, true)
        BOOST_MPL_HAS_XXX_TRAIT_NAMED_DEF(has_element_type, element_type, true)

        template<class SmartPtr>
        struct is_smart_ptr_impl 
            : is_same<
                  SmartPtr,
                  typename 
                  smart_ptr<
                      typename SmartPtr::first_policy,
                      typename SmartPtr::second_policy,
                      typename SmartPtr::third_policy,
                      typename SmartPtr::fourth_policy
                  >::template to<typename SmartPtr::element_type>
              >
            { };

        template<class SmartPtr>
        struct is_smart_ptr
            : mpl::and_< // Default arity of and_ is 5.
                  mpl::and_<
                      has_first_policy<SmartPtr>,
                      has_second_policy<SmartPtr>,
                      has_third_policy<SmartPtr>,
                      has_fourth_policy<SmartPtr>,
                      has_element_type<SmartPtr>
                  >,
                  is_smart_ptr_impl<typename remove_cv<SmartPtr>::type>
              >
            { };
            
    }//exit boost namespace
    }//exit policy_ptr namespace
}//exit boost namespace
//------------------------------------
//ChangeLog:
//  2005-06-24 Larry Evans
//    WHAT:
//      Moved from ../smart_ptr.hpp
//    WHY:
//      To reuse in ./shared_count_hook_sp_counted_accepting.hpp
//      in order to solve problem mentioned in 2005-06-23 ChangeLog
//      entry in that file.
#endif
