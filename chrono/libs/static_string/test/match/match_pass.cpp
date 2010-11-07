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

namespace boost { namespace static_string 
{

    template<typename T, T N>
    struct value_type<mpl::integral_c<T,N> > {
        typedef T type;
    };
    template<typename T0,typename T1,typename T2,typename T3,typename T4,
            typename T5,typename T6,typename T7,typename T8,typename T9,
            typename T10,typename T11>
    struct value_type<mpl::vector12<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> > {
        typedef typename value_type<T0>::type type;
    };
}
}
using namespace boost::static_string;
int main()
{
#if 0
    {
        using namespace boost;
        typedef mpl::string<'hell','o wo','rld'> hello;
//        typedef mpl::string<'h','e','l','l','o',' ','w','o','r','l','d'> hello3;
        typedef mpl::push_back<hello, mpl::char_<'!'> >::type hello2;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<hello::value_type,char>::value),
                NOTHING, (hello::value_type));
        std::cout << mpl::size<hello>::type::value << std::endl;
        BOOST_TEST((mpl::size<hello>::type::value == 11));
        std::cout << mpl::front<hello>::type::value << std::endl;
        std::cout << int('h') << std::endl;
        BOOST_TEST((mpl::front<hello>::type::value == 'h'));
        BOOST_TEST((std::string(mpl::c_str<hello2>::value)== "hello world!"));

    }
    {
        using namespace boost;
//        typedef mpl::vector_c<char, 'a','b','c','d','e'> rgc;
//        BOOST_TEST(0 == std::strcmp("abcde", static_string::c_str<rgc>::value));
//        typedef mpl::push_back<rgc, mpl::char_<'!'> >::type rgc2;
//        BOOST_TEST((std::string(static_string::c_str<rgc2>::value)== "abcde!"));
        
        typedef mpl::vector_c<char, 'h','e','l','l','o',' ','w','o','r','l','d'> hello;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<hello::value_type,char>::value),
                NOTHING, (hello::value_type));
//        std::cout << mpl::size<hello>::type::value << std::endl;
//        BOOST_TEST((mpl::size<hello>::type::value == 11));
//        std::cout << mpl::front<hello>::type::value << std::endl;
//        std::cout << 'h' << std::endl;
//        BOOST_TEST((mpl::front<hello>::type::value == 'h'));
//        BOOST_TEST((std::string(static_string::c_str<hello>::value)== "hello world"));
        typedef mpl::push_back<hello, mpl::char_<'!'> >::type hello2;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<static_string::value_type<hello2>::type,char>::value),
                NOTHING, (static_string::value_type<hello2>::type));
        BOOST_TEST((std::string(static_string::c_str<hello2>::value)== "hello world!"));
    }

#ifndef BOOST_NO_STD_WSTRING
#if 0    
    {
        using namespace boost;
        //typedef mpl::wstring<L'hell',L'o wo',L'rld'> hello;
//        typedef mpl::wstring<L'he',L'll',L'o ',L'wo',L'rl', L'd'> hello;
        typedef mpl::wstring<L'h',L'e',L'l',L'l',L'o',L' ',L'w',L'o',L'r',L'l',L'd'> hello;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<hello::value_type,wchar_t>::value),
                NOTHING, (hello::value_type));
        std::cout << mpl::size<hello>::type::value << std::endl;
        BOOST_TEST((mpl::size<hello>::type::value == 11));
        std::cout << mpl::front<hello>::type::value << std::endl;
        std::cout << L'h' << std::endl;
        BOOST_TEST((mpl::front<hello>::type::value == L'h'));
        BOOST_TEST((std::wstring(mpl::c_str<hello>::value)== L"hello world"));
        typedef mpl::push_back<hello, mpl::wchar_t_<L'!'> >::type hello2;
        BOOST_TEST((std::wstring(mpl::c_str<hello2>::value)== L"hello world!"));
    }
#endif
    {
        using namespace boost;
        typedef mpl::vector_c<wchar_t, L'h',L'e',L'l',L'l',L'o',L' ',L'w',L'o',L'r',L'l',L'd'> hello;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<hello::value_type,wchar_t>::value),
                NOTHING, (hello::value_type));
        std::cout << mpl::size<hello>::type::value << std::endl;
        BOOST_TEST((mpl::size<hello>::type::value == 11));
        std::cout << mpl::front<hello>::type::value << std::endl;
        std::cout << L'h' << std::endl;
        BOOST_TEST((mpl::front<hello>::type::value == L'h'));
        BOOST_TEST((std::wstring(static_string::c_str<hello>::value)== L"hello world"));
        typedef mpl::push_back<hello, mpl::integral_c<wchar_t, L'!'> >::type hello2;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<static_string::value_type<hello2>::type,wchar_t>::value),
                NOTHING, (mpl::value_type<hello2>::type));
        BOOST_TEST((std::wstring(static_string::c_str<hello2>::value)== L"hello world!"));
    }
#endif
#endif
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
        typedef basic_ptree<mpl::integral_c<char, 'a'>, mpl::int_<1> > tree;
        typedef insert<nil, mpl::pair<str_1<'a'>::type, mpl::int_<1> > >::type btree;
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<tree,btree>::value), 
                NOTHING, (btree));
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
#if 0
#endif    
    return boost::report_errors();
}

