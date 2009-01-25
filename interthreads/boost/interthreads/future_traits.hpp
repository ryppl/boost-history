#ifndef BOOST_INTERTHREADS_FUTURE_TRAITS__HPP
#define BOOST_INTERTHREADS_FUTURE_TRAITS__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/interthreads/act_traits.hpp>
#include <boost/thread/detail/move.hpp>
//#include <boost/thread/thread.hpp>
#include <boost/futures/future.hpp>
//#include <boost/utility/result_of.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/if.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

        template<typename T>
        struct act_traits<unique_future<T> >
        {
            typedef boost::scoped_ptr<T> storage_type;
#ifdef BOOST_HAS_RVALUE_REFS
            typedef T const& source_reference_type;
            struct dummy;
            typedef typename boost::mpl::if_<boost::is_fundamental<T>,dummy&,T&&>::type rvalue_source_type;
            typedef typename boost::mpl::if_<boost::is_fundamental<T>,T,T&&>::type move_dest_type;
#else
            typedef T& source_reference_type;
            typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T const&>::type rvalue_source_type;
            typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T>::type move_dest_type;
#endif

            static void init(storage_type& storage,source_reference_type t)
            {
                storage.reset(new T(t));
            }
            
            static void init(storage_type& storage,rvalue_source_type t)
            {
                storage.reset(new T(static_cast<rvalue_source_type>(t)));
            }

            static void cleanup(storage_type& storage)
            {
                storage.reset();
            }
        };
        
        template<typename T>
        struct act_traits<unique_future<T&> >
        {
            typedef T* storage_type;
            typedef T& source_reference_type;
            struct rvalue_source_type
            {};
            typedef T& move_dest_type;

            static void init(storage_type& storage,T& t)
            {
                storage=&t;
            }

            static void cleanup(storage_type& storage)
            {
                storage=0;
            }
        };

        template<>
        struct act_traits<unique_future<void> >
        {
            typedef bool storage_type;
            typedef void move_dest_type;

            static void init(storage_type& storage)
            {
                storage=true;
            }

            static void cleanup(storage_type& storage)
            {
                storage=false;
            }

        };    

        template<typename T>
        struct act_traits<shared_future<T> >
        {
            typedef boost::scoped_ptr<T> storage_type;
#ifdef BOOST_HAS_RVALUE_REFS
            typedef T const& source_reference_type;
            struct dummy;
            typedef typename boost::mpl::if_<boost::is_fundamental<T>,dummy&,T&&>::type rvalue_source_type;
#else
            typedef T& source_reference_type;
            typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T const&>::type rvalue_source_type;
#endif
            //typedef const T& move_dest_type;
            typedef T move_dest_type;

            static void init(storage_type& storage,source_reference_type t)
            {
                storage.reset(new T(t));
            }
            
            static void init(storage_type& storage,rvalue_source_type t)
            {
                storage.reset(new T(static_cast<rvalue_source_type>(t)));
            }

            static void cleanup(storage_type& storage)
            {
                storage.reset();
            }
        };
        
        template<typename T>
        struct act_traits<shared_future<T&> >
        {
            typedef T* storage_type;
            typedef T& source_reference_type;
            struct rvalue_source_type
            {};
            typedef T& move_dest_type;

            static void init(storage_type& storage,T& t)
            {
                storage=&t;
            }

            static void cleanup(storage_type& storage)
            {
                storage=0;
            }
        };

        template<>
        struct act_traits<shared_future<void> >
        {
            typedef bool storage_type;
            typedef void move_dest_type;

            static void init(storage_type& storage)
            {
                storage=true;
            }

            static void cleanup(storage_type& storage)
            {
                storage=false;
            }

        };            

}
}


#include <boost/config/abi_suffix.hpp>

#endif

