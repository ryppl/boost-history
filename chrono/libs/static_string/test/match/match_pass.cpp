//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

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

using namespace boost::static_string;
int main()
{

    // no range matches nil
    {
        std::string str="";
        std::string::iterator b=str.begin();
        std::string::iterator e=str.end();
        std::ios_base::iostate err;
        BOOST_TEST((match(nil(), b, e, err) == -1));
    }
    
    {
        std::string str="abc";
        std::string::iterator b=str.begin();
        std::string::iterator e=str.end();
        std::ios_base::iostate err;
        BOOST_TEST((match(nil(), b, e, err) == -1));
        BOOST_TEST((*b == 'a'));
    }
    
    // no range t matches empty forest
    {
        typedef forest<> tree;
        {
            std::string str="";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST((match(tree(), b, e, err) == -1));
        }
        {
            std::string str="a b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST((match(tree(), b, e, err) == -1));
            BOOST_TEST((*b == 'a'));
        }
    }
    // empty range doesn't matches tree
    {
        using namespace boost;
        typedef insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type tree;
        {
            std::string str="";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST((match(tree(), b, e, err) == -1));
        }
        // empty range doesn't matches tree
        {
            std::string str="b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST((match(tree(), b, e, err) == -1));
            BOOST_TEST((*b == 'b'));
        }
        // matching range matches tree
        {
            std::string str="a";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST((match(tree(), b, e, err) == 1));
            BOOST_TEST((b == e));
        }
        {
            std::string str="a b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 1);
            BOOST_TEST((*b == 'b'));
        }
    }
    
    {
        using namespace boost;
        typedef 
        insert<
            insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type,    
            mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > 
        >::type    
        tree;
        {
            std::string str="b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
            BOOST_TEST((*b == 'b'));
        }
        {
            std::string str="a";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 1);
            BOOST_TEST((b == e));
        }
        {
            std::string str="ab";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 2);
            BOOST_TEST((b == e));
        }
    }
    {
        using namespace boost;
        typedef 
        insert<
            insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<2> > >::type,    
            mpl::pair<str_1<'a'>::type, mpl::int_<1> > 
        >::type    
        tree;
        {
            std::string str="b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
            BOOST_TEST((*b == 'b'));
        }
        {
            std::string str="a";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 1);
            BOOST_TEST((b == e));
        }
        {
            std::string str="ab";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 2);
            BOOST_TEST((b == e));
        }
    }    
    {
        using namespace boost;
        typedef     
            insert<
                insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type,  
                mpl::pair<str_1<'b'>::type, mpl::int_<2> >
            >::type 
            tree;
        {
            std::string str="";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
        }
        {
            std::string str="c";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
            BOOST_TEST((*b == 'c'));
        }
        {
            std::string str="a";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 1);
            BOOST_TEST((b == e));
        }
        {
            std::string str="b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 2);
            BOOST_TEST((b == e));
        }
    }
    {
        using namespace boost;
        typedef     
            insert<
                insert<nil, mpl::pair<str_2<'a', 'b'>::type, mpl::int_<1> > >::type,  
                mpl::pair<str_1<'b'>::type, mpl::int_<2> >
            >::type tree;
            
        {
            std::string str="";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
        }
        {
            std::string str="c";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
            BOOST_TEST((*b == 'c'));
        }
        {
            std::string str="a";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == -1);
            BOOST_TEST((b == e));
        }
        {
            std::string str="ab";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 1);
            BOOST_TEST((b == e));
        }
        {
            std::string str="b";
            std::string::iterator b=str.begin();
            std::string::iterator e=str.end();
            std::ios_base::iostate err;
            BOOST_TEST(match(tree(), b, e, err) == 2);
            BOOST_TEST((b == e));
        }
    }
    return boost::report_errors();
}

