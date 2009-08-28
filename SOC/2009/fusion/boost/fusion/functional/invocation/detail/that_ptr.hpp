/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_THAT_PTR_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_THAT_PTR_HPP

#include <boost/get_pointer.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/type_traits/config.hpp>
#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace fusion { namespace detail
{
    template<typename WantedRef>
    struct that_ptr
    {
    private:
        typedef typename detail::remove_reference<WantedRef>::type* wanted;

        template<typename T>
        static inline wanted
        do_get_pointer(BOOST_FUSION_R_ELSE_LREF(T), wanted ptr)
        {
            return ptr;
        }

        template<typename T>
        static inline wanted
        do_get_pointer(BOOST_FUSION_R_ELSE_LREF(T) t, void const*)
        {
            return get_pointer(BOOST_FUSION_FORWARD(T,t));
        }

    public:
        static inline wanted
        get(WantedRef x)
        {
            return boost::addressof(x);
        }

        static inline wanted
        get(wanted ptr)
        {
            return ptr;
        }

        template <typename T>
        static inline wanted
        get(BOOST_FUSION_R_ELSE_LREF(T) t)
        {
            return do_get_pointer(
                    BOOST_FUSION_FORWARD(T,t),
                    boost::addressof(BOOST_FUSION_FORWARD(T,t)));
        }
    };

#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES) || 1
    template <typename PtrOrSmartPtr>
    struct non_const_pointee;

    namespace adl_barrier
    {
        using boost::get_pointer;
        void const * BOOST_TT_DECL get_pointer(...); // fallback
  
        template< typename T> char const_tester(T *);
        template< typename T> long const_tester(T const *);

        template <typename Ptr>
        struct non_const_pointee_impl
        {
            static Ptr & what;

            static bool const value =
                sizeof(const_tester(get_pointer(what))) == 1;
        };
    }

    template <typename PtrOrSmartPtr> struct non_const_pointee
        : adl_barrier::non_const_pointee_impl< 
              typename remove_cv<
                  typename remove_reference<PtrOrSmartPtr>::type >::type >
    {
        typedef non_const_pointee type;
        typedef bool value_type;
    };
#endif
}}}

#endif
