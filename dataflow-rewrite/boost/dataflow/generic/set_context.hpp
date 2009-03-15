/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__SET_CONTEXT_HPP
#define BOOST__DATAFLOW__GENERIC__SET_CONTEXT_HPP


#include <boost/mpl/assert.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/dataflow/generic/framework_entity/default_framework_of.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/detail/not_specialized.hpp>


#include <boost/dataflow/generic/framework_entity.hpp>
namespace boost { namespace dataflow {

namespace extension
{
    template<typename Traits1,typename Enable=void>
    struct set_context_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef detail::not_specialized result_type;

        template<typename T1,typename Context>
        result_type operator()(T1 &t1,Context & context)
        {
            // Error: set_context_impl has not been
            // specialized appropriately.
            BOOST_MPL_ASSERT(( mpl::bool_<sizeof(T1)==0> ));
            return result_type();
        }
    };
    
    template<typename Traits1,typename Enable=void>
    struct set_context_will_succeed_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef bool result_type;

        template<typename T1,typename Context>
        result_type operator()(T1 &t1,Context & context)
        {
            return
                !is_same<
                        typename  extension::set_context_impl<
                            Traits1
                            >::result_type,
                    detail::not_specialized
                >::value;
        }
    };
}

namespace detail
{

    template<typename T1,typename Context,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
    struct has_set_context_specialized
        : public mpl::true_
    {};
    
    template<typename T1,typename Context,typename Framework1>
    struct has_set_context_specialized<
        T1,Context,Framework1
        ,
        typename enable_if<
            is_same<
               typename  extension::set_context_impl<
                typename traits_of<T1, Framework1>::type
                >::result_type,
                detail::not_specialized>
        >::type>
        : public mpl::false_
    {};

}

template<typename T1,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
struct has_set_context
    : public mpl::false_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

template<typename T1,typename Framework1>
struct has_set_context<
    T1,Framework1,
    typename enable_if<
        detail::has_set_context_specialized<
            T1,Framework1>
     >::type>
    : public mpl::true_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

namespace result_of {

    template<typename T1,typename Context,typename Framework1=typename default_framework_of<T1>::type>
    struct set_context
    {
        typedef typename boost::result_of<
            extension::set_context_impl<
                typename traits_of<T1, Framework1>::type
            >
            (T1,Context)
        >::type type;
    };
    
}


template<typename Context,typename Framework1,typename T1>
inline typename result_of::set_context<
    T1,Context,Framework1
    >::type 
set_context_framework(T1 &t1,Context & context=Context())
{
    return extension::set_context_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename T1>
inline typename result_of::set_context<
    T1,Context,typename default_framework_of<T1>::type
    >::type 
set_context(T1 &t1,Context & context=Context())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::set_context_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename Framework1,typename T1>
inline bool
set_context_will_succeed_framework(T1 &t1,Context & context=Context())
{
    return extension::set_context_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename T1>
inline bool 
set_context_will_succeed(T1 &t1,Context & context=Context())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::set_context_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename Framework1,typename T1>
inline typename result_of::set_context<
    T1,Context,Framework1
    >::type 
set_context_framework(const T1 &t1,Context & context=Context())
{
    return extension::set_context_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename T1>
inline typename result_of::set_context<
    T1,Context,typename default_framework_of<T1>::type
    >::type 
set_context(const T1 &t1,Context & context=Context())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::set_context_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename Framework1,typename T1>
inline bool
set_context_will_succeed_framework(const T1 &t1,Context & context=Context())
{
    return extension::set_context_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

template<typename Context,typename T1>
inline bool 
set_context_will_succeed(const T1 &t1,Context & context=Context())
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::set_context_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1,context);
}

}}


#endif // BOOST__DATAFLOW__GENERIC__SET_CONTEXT_HPP
