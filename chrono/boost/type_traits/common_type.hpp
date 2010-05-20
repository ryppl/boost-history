//  common_type.hpp  ---------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_COMMON_TYPE_HPP
#define BOOST_COMMON_TYPE_HPP

#include <boost/config.hpp>

//----------------------------------------------------------------------------//
#if defined(BOOST_NO_VARIADIC_TEMPLATES)
#define BOOST_COMMON_TYPE_ARITY 3
#endif

//----------------------------------------------------------------------------//
#if defined(BOOST_NO_RVALUE_REFERENCES) \
    || defined(BOOST_NO_DECLTYPE)
#define BOOST_TYPEOF_SILENT
#include <boost/typeof/typeof.hpp>   // boost wonders never cease!
#endif

//----------------------------------------------------------------------------//
#if !defined(BOOST_NO_STATIC_ASSERT)
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND,MSG) \
        static_assert(CND, MSG)
#else
#include <boost/static_assert.hpp>   // boost wonders never cease!
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND,MSG) \
        BOOST_STATIC_ASSERT(CND)
#endif    


//----------------------------------------------------------------------------//
//                                                                            //
//                           C++03 implementation of                          //
//             20.6.7 Other transformations [meta.trans.other]                //
//                          Written by Howard Hinnant                         //
//                       Adapted for Boost by Beman Dawes                     //
//                                                                            //
//----------------------------------------------------------------------------//

namespace boost {

// prototype    
#if !defined(BOOST_NO_VARIADIC_TEMPLATES)
    template<typename... T>
    struct common_type;
#else // or no specialization
    template <class T, class U = void, class V = void>
    struct common_type 
    {
    public:
        typedef typename common_type<typename common_type<T, U>::type, V>::type type;
    };
#endif    


// 1 arg
    template<typename T>
#if !defined(BOOST_NO_VARIADIC_TEMPLATES)
    struct common_type<T> 
#else
    struct common_type<T, void, void>

#endif    
    {
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(T) > 0, "must be complete type");
    public:
        typedef T type;
    };

// 2 args
    
#if !defined(BOOST_NO_VARIADIC_TEMPLATES)
    template <class T, class U>
    struct common_type<T, U> 
#else
    template <class T, class U>
    struct common_type<T, U, void>
#endif    
    {
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(T) > 0, "must be complete type");
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(U) > 0, "must be complete type");
        static bool m_f();  // workaround gcc bug; not required by std
#if !defined(BOOST_NO_RVALUE_REFERENCES) \
    && !defined(BOOST_NO_DECLTYPE)
            static T&& m_t();
            static U&& m_u();

        public:
            typedef decltype(m_f() ? m_t() : m_u()) type;
#else
            static T m_t();
            static U m_u();
        public:
            typedef BOOST_TYPEOF_TPL(m_f() ? m_t() : m_u()) type;
#endif        
    };
    
// 3 or more args
#if !defined(BOOST_NO_VARIADIC_TEMPLATES)  
    template<typename T, typename U, typename... V>
    struct common_type<T, U, V...> {
    public:
        typedef typename common_type<typename common_type<T, U>::type, V...>::type type;
    };
#endif
}  // namespace boost

#endif  // BOOST_COMMON_TYPE_HPP
