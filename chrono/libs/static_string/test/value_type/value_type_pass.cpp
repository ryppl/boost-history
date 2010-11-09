//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/static_string/value_type.hpp>
#include <boost/detail/lightweight_test.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/char.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/deque.hpp>
#include <boost/mpl/set.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/list_c.hpp>
#include <boost/mpl/set_c.hpp>
#include <boost/mpl/string.hpp>

#include <boost/mpl/find.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/insert_range.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>

#include <string>
#include <cstring>
#include <iostream>

#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

using namespace boost::static_string;
using namespace boost;
int main()
{
    // mpl::char_<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::char_<'\0'> >::type,
                char
            >::value
        )); 
    }
    // mpl::bool_<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::bool_<true> >::type,
                bool
            >::value
        )); 
    }
    // mpl::int_<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::int_<0> >::type,
                int
            >::value
        )); 
    }
    // mpl::long_<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::int_<0> >::type,
                int
            >::value
        )); 
    }
    // mpl::size_t<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::size_t<0> >::type,
                std::size_t
            >::value
        )); 
    }
    // mpl::integral_c<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::integral_c<int,0> >::type,
                int
            >::value
        )); 
    }
    // mpl::char_<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::char_<'\0'> >::type,
                char
            >::value
        )); 
    }
    // mpl::vector<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::vector<mpl::char_<'a'> > >::type,
                char
            >::value
        )); 
    }
    // mpl::list<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::list<mpl::char_<'a'> > >::type,
                char
            >::value
        )); 
    }
    // mpl::deque<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::deque<mpl::char_<'a'> > >::type,
                char
            >::value
        )); 
    }
    // mpl::set<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::set<mpl::char_<'a'> > >::type,
                char
            >::value
        )); 
    }
    // mpl::range_c<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::range_c<char, 'a', 'z'> >::type,
                char
            >::value
        )); 
    }
    // mpl::vector_c<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::vector_c<char, 'a'> >::type,
                char
            >::value
        )); 
    }
    // mpl::list_c<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::list_c<char, 'a', 'b'> >::type,
                char
            >::value
        )); 
    }
    // mpl::set_c<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::list_c<char, 'a', 'b'> >::type,
                char
            >::value
        )); 
    }
    // mpl::string<>
    {
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<mpl::string<> >::type,
                char
            >::value
        )); 
    }
    // mpl::erase<>
    {
        typedef mpl::vector_c<int,1,0,5,1,7,5,0,5> values;
        typedef mpl::find< values, mpl::integral_c<int,7> >::type pos;
        typedef mpl::erase<values,pos>::type checked_type;
        
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<checked_type>::type,
                int
            >::value
        )); 
    }
    // mpl::insert<>
    {
        typedef mpl::vector_c<int,0,1,3,4,5,6,7,8,9> numbers;
        typedef mpl::find< numbers,mpl::integral_c<int,3> >::type pos;
        typedef mpl::insert< numbers,pos,mpl::integral_c<int,2> >::type checked_type;
        
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<checked_type>::type,
                int
            >::value
        )); 
    }
    // mpl::insert_range<>
    {
        typedef mpl::vector_c<int,0,1,7,8,9> numbers;
        typedef mpl::find< numbers,mpl::integral_c<int,7> >::type pos;
        typedef mpl::insert_range< numbers,pos,mpl::range_c<int,2,7> >::type checked_type;
        
        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<checked_type>::type,
                int
            >::value
        )); 
    }
    // mpl::push_back<>
    {
        typedef mpl::vector_c<bool,false,false,false,
            true,true,true,false,false> bools;
        typedef mpl::push_back<bools,mpl::false_>::type checked_type;

        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<checked_type>::type,
                bool
            >::value
        )); 
    }
    // mpl::push_front<>
    {
        typedef mpl::vector_c<bool,false,false,false,
            true,true,true,false,false> bools;
        typedef mpl::push_front<bools,mpl::false_>::type checked_type;

        BOOST_STATIC_ASSERT((
            boost::is_same<
                value_type<checked_type>::type,
                bool
            >::value
        )); 
    }
    return boost::report_errors();
}

