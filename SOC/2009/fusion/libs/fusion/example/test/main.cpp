// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <iostream>

#include <boost/fusion/adapted/mpl.hpp>
#include <boost/fusion/container/vector/vector10.hpp>

#include <boost/fusion/container/set.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>

#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/vector/convert.hpp>
#include <boost/fusion/sequence/intrinsic/begin.hpp>
#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/next.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#include <boost/fusion/container/generation/vector_tie.hpp>

#include <boost/fusion/container/map.hpp>
#include <boost/fusion/container/generation/make_map.hpp>

#include <boost/fusion/container/list/list.hpp>
#include <boost/fusion/container/list/cons.hpp>
#include <boost/fusion/container/generation/make_cons.hpp>

#include <boost/fusion/sequence/io/out.hpp>

#include <boost/fusion/view/zip_view.hpp>

namespace fusion=boost::fusion;

template<class... Args> class get_vec_type
{
public:
    typedef fusion::result_of::as_vector<boost::mpl::vector<char,float> >::type type;
    //typedef fusion::vector<Args...> type;
};

typedef get_vec_type<char,short,int,long,float,double>::type vec;
typedef const vec cvec;

typedef vec& lvec;
typedef vec&& rvec;
typedef const vec& clvec;
typedef const vec&& crvec;

class moveable
{
public:
    moveable()=default;
    moveable(const moveable&)=delete;
    moveable(moveable&&){}
};

int main()
{
    {
        using namespace fusion::result_of;

        static_assert(std::is_same<deref<begin<vec>::type>::type,char&>::value,"");
        static_assert(std::is_same<deref<begin<cvec>::type>::type,const char&>::value,"");
        static_assert(std::is_same<deref<begin<lvec>::type>::type,char&>::value,"");
        static_assert(std::is_same<deref<begin<rvec>::type>::type,char>::value,"");
        static_assert(std::is_same<deref<begin<clvec>::type>::type,const char&>::value,"");
        //TODO cschmidt: !!!
        static_assert(std::is_same<deref<begin<crvec>::type>::type,const char>::value,"");
    }

    {
        using namespace fusion;
        vector<moveable> m;
        vector<moveable> m2=std::move(m);
        m=std::move(m2);

        int i=0;
        make_vector(boost::cref(i));
    }

    {
        using namespace fusion;
        set<int,float,double> s(0,0,0);
        at_key<int>(s)=0;
    }

    {
        using namespace fusion;
        std::cout << deref(begin(make_map<int>(1))) << std::endl;

        map<pair<char,char>,pair<int,int> > m('X', 123);
        std::cout << m << std::endl;
        std::cout << make_map<char, int>('X', 123) << std::endl;
    }
}
