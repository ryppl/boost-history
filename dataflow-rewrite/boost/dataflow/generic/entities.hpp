/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__GENERIC__ENTITIES_HPP
#define BOOST__DATAFLOW__GENERIC__ENTITIES_HPP


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
    struct entities_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef detail::not_specialized result_type;

        template<typename T1>
        result_type operator()(T1 &t1)
        {
            // Error: entities_impl has not been
            // specialized appropriately.
            BOOST_MPL_ASSERT(( mpl::bool_<sizeof(T1)==0> ));
            return result_type();
        }
    };
    
    template<typename Traits1,typename Enable=void>
    struct entities_will_succeed_impl
    {
        BOOST_MPL_ASSERT(( is_same<Enable, void> ));
        
        typedef bool result_type;

        template<typename T1>
        result_type operator()(T1 &t1)
        {
            return
                !is_same<
                        typename  extension::entities_impl<
                            Traits1
                            >::result_type,
                    detail::not_specialized
                >::value;
        }
    };
}

namespace detail
{

    template<typename T1,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
    struct has_entities_specialized
        : public mpl::true_
    {};
    
    template<typename T1,typename Framework1>
    struct has_entities_specialized<
        T1,Framework1
        ,
        typename enable_if<
            is_same<
               typename  extension::entities_impl<
                typename traits_of<T1, Framework1>::type
                >::result_type,
                detail::not_specialized>
        >::type>
        : public mpl::false_
    {};

}

template<typename T1,typename Framework1=typename default_framework_of<T1>::type,typename Enable=void>
struct has_entities
    : public mpl::false_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

template<typename T1,typename Framework1>
struct has_entities<
    T1,Framework1,
    typename enable_if<
        detail::has_entities_specialized<
            T1,Framework1>
     >::type>
    : public mpl::true_
{
    BOOST_MPL_ASSERT((mpl::and_<is_framework_entity<T1,Framework1> >));
};

namespace result_of {

    template<typename T1,typename Framework1=typename default_framework_of<T1>::type>
    struct entities
    {
        typedef typename boost::result_of<
            extension::entities_impl<
                typename traits_of<T1, Framework1>::type
            >
            (T1)
        >::type type;
    };
    
}


template<typename Framework1,typename T1>
inline typename result_of::entities<
    T1,Framework1
    >::type 
entities_framework(T1 &t1)
{
    return extension::entities_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename T1>
inline typename result_of::entities<
    T1,typename default_framework_of<T1>::type
    >::type 
entities(T1 &t1)
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::entities_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename Framework1,typename T1>
inline bool
entities_will_succeed_framework(T1 &t1)
{
    return extension::entities_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename T1>
inline bool 
entities_will_succeed(T1 &t1)
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::entities_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename Framework1,typename T1>
inline typename result_of::entities<
    T1,Framework1
    >::type 
entities_framework(const T1 &t1)
{
    return extension::entities_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename T1>
inline typename result_of::entities<
    T1,typename default_framework_of<T1>::type
    >::type 
entities(const T1 &t1)
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::entities_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename Framework1,typename T1>
inline bool
entities_will_succeed_framework(const T1 &t1)
{
    return extension::entities_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

template<typename T1>
inline bool 
entities_will_succeed(const T1 &t1)
{
    typedef typename default_framework_of<T1>::type Framework1;
    return extension::entities_will_succeed_impl<
        typename traits_of<T1, Framework1>::type
        >()(t1);
}

}}


#endif // BOOST__DATAFLOW__GENERIC__ENTITIES_HPP
