/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/view.hpp>
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/support.hpp>
#include <boost/fusion/functional.hpp>

#include <boost/mpl/placeholders.hpp>

namespace fusion=boost::fusion;

struct dummy
{
    int i;

    dummy()
      : i(0)
    {}

    template<typename T>
    dummy(T const& t)
      : i(t.i)
    {}

    template<typename T,typename T2>
    dummy(T,T2)
      : i(0)
    {}
};

struct dummy2
{
    int i;

    dummy2()
      : i(0)
    {}

    template<class T>
    dummy2(T const& t)
      : i(t.i)
    {}
};

void assign_test()
{
    using namespace fusion;

    dummy d;

    {
        fusion::pair<int,std::string> a("test");
    }

    {
        vector<int> vec(0);
        vector<int> vec2(vec);
        vector<int> vec3(sequence_assign(vec));
        vector<long> vec4(vec);
    }

    {
        vector<dummy> vec(d);
        vector<dummy> vec2(vec);
        vector<dummy> vec3(sequence_assign(vec));
        //vector<dummy2> vec4(vec);
        vector<dummy2> vec5(sequence_assign(vec));
    }

    {
        vector<int,dummy> vec(0,d);
        vector<int,dummy> vec2(vec);
        vector<int,dummy> vec3(sequence_assign(vec));
        vector<int,dummy2> vec4(vec);
        vector<int,dummy2> vec5(sequence_assign(vec));
        vector<long,dummy2> vec6(vec);
    }

    {
        single_view<int> view(0);
        single_view<int> view2(view);
        single_view<int> view3(sequence_assign(view));
        single_view<long> view4(view);
    }

    map<pair<int,int> > m(make_pair<int>(0));
}

#if defined(BOOST_NO_RVALUE_REFERENCES) || defined(BOOST_NO_VARIADIC_TEMPLATES)
int main()
{
    assign_test();
}
#else
#include <type_traits>
#include <utility>
#include <iostream>

struct moveable
{
    moveable(int){};
    moveable(moveable&&){}
    moveable& operator=(moveable&&){return *this;}
    moveable()=delete;
    moveable(moveable const&)=delete;
    moveable& operator=(moveable const&)=delete;
};
bool operator==(moveable const&, moveable const&)
{
    return true;
}

template<class... Args>
struct get_vec_type
{
    typedef fusion::vector<Args...> type;
};

typedef get_vec_type<char,short,int,long,float,double,moveable>::type vec;
typedef const vec cvec;

typedef vec& lvec;
typedef vec&& rvec;
typedef const vec& clvec;
typedef const vec&& crvec;

template<typename Seq>
void seq_test(Seq&& seq)
{
    using namespace fusion;

    typedef typename detail::identity<Seq>::type id_seq;

    typename result_of::begin<id_seq>::type beg=begin(std::forward<Seq>(seq));
    typename result_of::begin<const id_seq>::type cbeg=begin(std::forward<Seq>(seq));

    typename result_of::begin<id_seq&>::type beg2=begin(std::forward<Seq>(seq));
    typename result_of::begin<const id_seq&>::type cbeg2=begin(std::forward<Seq>(seq));

    typename result_of::begin<id_seq&&>::type beg3=begin(std::forward<Seq>(seq));
    typename result_of::begin<const id_seq&&>::type cbeg3=begin(std::forward<Seq>(seq));

    beg=beg;
    cbeg=beg;

    beg2=beg2;
    cbeg2=beg2;

    beg3=beg3;
    cbeg3=beg3;

    beg=beg2;
    beg=beg3;
    cbeg=beg2;
    cbeg=cbeg2;
    cbeg=beg3;
    cbeg=cbeg3;

    beg2=beg;
    beg2=beg3;
    cbeg2=beg;
    cbeg2=cbeg;
    cbeg2=beg3;
    cbeg2=cbeg3;

    beg3=beg2;
    beg3=beg;
    cbeg3=beg2;
    cbeg3=cbeg2;
    cbeg3=beg;
    cbeg3=cbeg;

    //beg=cbeg2;

    {
        auto it=begin(std::forward<Seq>(seq));
        it=begin(std::forward<Seq>(seq));
    }
    {
        auto it=advance_c<1>(begin(std::forward<Seq>(seq)));
        it=advance_c<1>(begin(std::forward<Seq>(seq)));
    }
    {
        auto it=end(std::forward<Seq>(seq));
        it=end(std::forward<Seq>(seq));
    }
}

struct identity_int
{
    int operator()(int i)
    {
        return i;
    }
};

struct C
{};

int main()
{
    assign_test();

    {
        using namespace fusion;

        static_assert(std::is_same<traits::tag_of<int>::type,non_fusion_tag>::value,"");
        static_assert(std::is_same<traits::tag_of<boost::mpl::vector<> >::type,mpl_sequence_tag>::value,"");
        static_assert(std::is_same<traits::tag_of<boost::mpl::begin<boost::mpl::vector<> >::type>::type,mpl_iterator_tag>::value,"");
    }

    {
        using namespace fusion::result_of;

        static_assert(std::is_same<deref<begin<vec>::type>::type,char&>::value,"");
        static_assert(std::is_same<deref<begin<cvec>::type>::type,const char&>::value,"");
        static_assert(std::is_same<deref<begin<lvec>::type>::type,char&>::value,"");
        static_assert(std::is_same<deref<begin<rvec>::type>::type,char>::value,"");
        static_assert(std::is_same<deref<begin<clvec>::type>::type,const char&>::value,"");
        static_assert(std::is_same<deref<begin<crvec>::type>::type,const char>::value,"");

        static_assert(std::is_same<deref<begin<fusion::vector<C> >::type>::type,C&>::value,"");
        static_assert(std::is_same<deref<begin<fusion::vector<C> const>::type>::type,C const&>::value,"");
        static_assert(std::is_same<deref<begin<fusion::vector<C>&>::type>::type,C&>::value,"");
        static_assert(std::is_same<deref<begin<fusion::vector<C>&&>::type>::type,C&&>::value,"");
        static_assert(std::is_same<deref<begin<fusion::vector<C>const&>::type>::type,C const&>::value,"");
        static_assert(std::is_same<deref<begin<fusion::vector<C>const&&>::type>::type,C const&&>::value,"");
    }

    {
        using namespace fusion;
        vector<moveable> v(0);
        vector<moveable> v2=std::move(v);
        //vector<moveable> v2=v;
    }

    {
        using namespace fusion;
        deref(begin(vector_tie(int())));
    }

    {
        using namespace fusion;
        seq_test(make_vector(0,1));
        seq_test(make_cons(0,make_cons(1)));
        seq_test(make_list(0,1));
        seq_test(make_set(0,1.0f));
        seq_test(make_map<int,float>(0,1.0f));

        seq_test(single_view<int>(0));
        seq_test(filter_view<vector<int>&&,std::is_same<boost::mpl::_1,int> >(vector<int>()));
        vector<int> vec;
        seq_test(iterator_range<result_of::begin<vector<int>>::type,result_of::end<vector<int>>::type>(begin(vec),end(vec)));
        seq_test(joint_view<vector<int>&&,vector<int>&&>(vector<int>(),vector<int>()));
        seq_test(zip_view<vector<vector<int>&&, vector<int>&&> >(vector_tie(vector<int>(),vector<int>())));
        seq_test(transform_view<vector<int>&&,identity_int>(vector<int>(),identity_int()));
        seq_test(reverse_view<vector<int>&&>(vector<int>()));
        seq_test(repetitive_view<vector<int>&&>(vector<int>()));
    }
}
#endif
