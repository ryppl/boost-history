//----------------------------------------------------------------------------
// boost mpl/runtime_function.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//----------------------------------------------------------------------------
//
// Copyright (c) 2000-01
// Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#ifndef BOOST_MPL_RUNTIME_FUNCTION_HPP
#define BOOST_MPL_RUNTIME_FUNCTION_HPP

namespace boost {
namespace mpl {

#if defined(BOOST_MPL_NO_RUNTIME_FUNCTION_TRAITS)
#   define BOOST_MPL_RUNTIME_FUNCTION(f) f
#else
#   define BOOST_MPL_RUNTIME_FUNCTION(f) mpl::runtime_function<f>
    template<typename NullaryFunction>
    struct runtime_function
    {
        template<typename T>
        static inline void execute(T t)
        {
            NullaryFunction::execute(t);
        }
        
        static inline void execute()
        {
            NullaryFunction::execute();
        }
    };
#endif // BOOST_MPL_NO_RUNTIME_FUNCTION_TRAITS

} // namespace mpl
} // namespace boost 

#endif // BOOST_MPL_RUNTIME_FUNCTION_HPP
