/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009 Christopher Schmidt

    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_THAT_PTR_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_DETAIL_THAT_PTR_HPP

#include <boost/fusion/support/internal/small_big_type.hpp>
#include <boost/fusion/support/internal/ref.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/type_traits/config.hpp>
#include <boost/type_traits/add_volatile.hpp>
#include <boost/get_pointer.hpp>
#include <boost/utility/addressof.hpp>

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

    namespace adl_barrier
    {
        using boost::get_pointer;
        void const* get_pointer(...);
  
        template<typename T>
        small_type const_tester(T*);

        template<typename T>
        big_type const_tester(T const*);

        template <typename Ptr>
        struct const_pointee_impl
        {
            static Ptr* what;

            typedef
                mpl::bool_<
                    sizeof(const_tester(get_pointer(*what)))==
                    sizeof(big_type)
                >
            type;
        };

        template<typename T>
        small_type volatile_tester(T*);

        template<typename T>
        big_type volatile_tester(T volatile*);

        template <typename Ptr>
        struct volatile_pointee_impl
        {
            static Ptr* what;

            typedef
                mpl::bool_<
                    sizeof(volatile_tester(get_pointer(*what)))==
                    sizeof(big_type)
                >
            type;
        };
    }

    template <typename PtrOrSmartPtr>
    struct const_pointee
        : adl_barrier::const_pointee_impl<
              typename identity<PtrOrSmartPtr>::type
          >::type
    {};

    template <typename PtrOrSmartPtr>
    struct volatile_pointee
        : adl_barrier::volatile_pointee_impl<
              typename identity<PtrOrSmartPtr>::type
          >::type
    {};
}}}

#endif
