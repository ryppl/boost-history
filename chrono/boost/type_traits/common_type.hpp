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
#ifndef BOOST_NO_STATIC_ASSERT
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(BOOST_COMMON_TYPE_USES_STATIC_ASSERT)
#include <boost/static_assert.hpp>
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND, MSG, TYPES) BOOST_STATIC_ASSERT(CND)
#elif defined(BOOST_COMMON_TYPE_USES_MPL_ASSERT)
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    BOOST_MPL_ASSERT_MSG(boost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#elif defined(BOOST_COMMON_TYPE_USES_ARRAY_ASSERT)
#define BOOST_COMMON_TYPE_CONCAT(A,B) A##B
#define BOOST_COMMON_TYPE_NAME(A,B) BOOST_COMMON_TYPE_CONCAT(A,B)
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND, MSG, TYPES) static char BOOST_COMMON_TYPE_NAME(boost_common_type_test_,__LINE__)[CND];
#else
#define BOOST_COMMON_TYPE_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#if !defined(BOOST_COMMON_TYPE_USES_MPL_ASSERT)
#define BOOST_COMMON_TYPE_MUST_BE_A_COMPLE_TYPE "must be complete type"
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
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(T) > 0, "must be complete type",(T));
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
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(T) > 0, "must be complete type",(T));
        BOOST_COMMON_TYPE_STATIC_ASSERT(sizeof(U) > 0, "must be complete type",(T));
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
