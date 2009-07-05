// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <type_traits>
#include <utility>
#include <iostream>

#include <boost/fusion/sequence.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/view.hpp>

#include <boost/mpl/placeholders.hpp>

namespace fusion=boost::fusion;

struct moveable
{
    moveable()=default;
    moveable(const moveable&)=delete;
    moveable(moveable&&){}
};

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

    deref(begin(std::forward<Seq>(seq)));
    advance_c<1>(begin(std::forward<Seq>(seq)));
    end(std::forward<Seq>(seq));
}

struct identity_int
{
    template<typename>
    struct result
    {
        typedef int type;
    };

    int operator()(int i)
    {
        return i;
    }
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
        vec v;
        vec v2=std::move(v);
        //vec v2=v;
    }

    {
        using namespace fusion;
        seq_test(make_vector(0,1));
        seq_test(make_cons(0,make_cons(1)));
        seq_test(make_list(0,1));
        seq_test(make_set(0,1.0f));
        seq_test(make_map<int,float>(0,1.0f));

        seq_test(single_view<int>(0));
        seq_test(filter_view<vector<int>&&,std::is_same<boost::mpl::_,int> >(vector<int>()));
        vector<int> vec;
        seq_test(iterator_range<result_of::begin<vector<int>>::type,result_of::end<vector<int>>::type>(begin(vec),end(vec)));
        seq_test(joint_view<vector<int>&&,vector<int>&&>(vector<int>(),vector<int>()));
        seq_test(zip_view<vector<vector<int>&&, vector<int>&&> >(vector_tie(vector<int>(),vector<int>())));
        seq_test(transform_view<vector<int>&&,identity_int>(vector<int>(),identity_int()));
        seq_test(reverse_view<vector<int>&&>(vector<int>()));
    }
}
