#ifndef BOOST_POLICY_PTR_DETAIL_SP_DEBUG_CDTOR_HOOKS_HPP_INCLUDED
#define BOOST_POLICY_PTR_DETAIL_SP_DEBUG_CDTOR_HOOKS_HPP_INCLUDED

// Debug hooks
#ifdef BOOST_SP_DEBUG_MODE
# define BOOST_SP_CONSTRUCTOR_HOOK(p, s)                sp_constructor_hook(p, s)
# define BOOST_SP_DESTRUCTOR_HOOK(p, s)                 sp_destructor_hook(p, s)
#else
# define BOOST_SP_CONSTRUCTOR_HOOK(p, s)
# define BOOST_SP_DESTRUCTOR_HOOK(p, s)
#endif // BOOST_SP_DEBUG_MODE

#endif
