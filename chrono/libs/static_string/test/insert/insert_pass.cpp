//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#define BOOST_MPL_LIMIT_STRING_SIZE 64

#include <boost/static_string/static_string.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <iostream>
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <boost/chrono/detail/static_assert.hpp>
#include <boost/mpl/string.hpp>
//#include <boost/mpl/wstring.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector_c.hpp>
#include <string>
#include <cstring>
#include <iostream>

#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

using namespace boost;
using namespace boost::static_string;
int main()
{
    {
        typedef basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<1> > tree;
        typedef insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type btree;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                NOTHING, (btree));
    }  
    {
        using namespace boost;
        typedef     
            basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<1>, 
                 basic_ptree<mpl::integral_c<char, 'b'>, mpl::int_<2>  >  
        > tree;
        typedef 
        insert<
            insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type,    
            mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > 
        >::type    
        btree;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                NOTHING, (insert<
                        insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type,    
                        mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > 
                    >::type));
    }
    {
        typedef     
            basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<1>, 
                 basic_ptree<mpl::integral_c<char, 'b'>, mpl::int_<2>  
                 >  
        > tree;
        typedef 
        insert<
            insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > >::type,    
            mpl::pair<str_1<'a'>::type, mpl::int_<1> > 
        >::type    
        btree;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                NOTHING, (insert<
                        insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > >::type,    
                        mpl::pair<str_1<'a'>::type, mpl::int_<1> > 
                    >::type));
    }    
    {
        typedef     
            forest<
                basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<1>  >,
                basic_ptree<mpl::integral_c<char, 'b'>, mpl::int_<2>  >
            > tree;
        typedef     
            insert<
                insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type,  
                mpl::pair<str_1<'b'>::type, mpl::int_<2> >
            >::type 
            btree;
            BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                    NOTHING, (btree));
    }
    {
        typedef     
            forest<
                basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<-1>, 
                    basic_ptree<mpl::integral_c<char, 'b'>, mpl::int_<1>  > 
                >,
                basic_ptree<mpl::integral_c<char, 'b'>, mpl::int_<2>  >
            > tree;
        typedef     
            insert<
                insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<1> > >::type,  
                mpl::pair<str_1<'b'>::type, mpl::int_<2> >
            >::type btree;
            
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                NOTHING, (insert<
                        insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<1> > >::type,  
                        mpl::pair<str_1<'b'>::type, mpl::int_<2> >
                    >::type));
    }
    return boost::report_errors();
}

