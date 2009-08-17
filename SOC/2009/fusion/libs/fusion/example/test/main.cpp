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
#include <boost/fusion/adapted.hpp>
#include <boost/fusion/tuple.hpp>
#include <boost/fusion/support.hpp>
#include <boost/fusion/functional.hpp>

#include <boost/mpl/placeholders.hpp>

namespace fusion=boost::fusion;

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

template<class>
class C;

int main()
{
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
        //TODO cschmidt: !!!
        static_assert(std::is_same<deref<begin<crvec>::type>::type,const char>::value,"");
    }

    {
        fusion::vector<moveable> v(0);
        fusion::vector<moveable> v2=std::move(v);
        //fusion::vector<moveable> v2=v;
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
    }
}

/*#include <boost/mpl/transform.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include <utility>

namespace mpl=boost::mpl;

struct my_make_pair {
  template<typename T1, typename T2>
  struct apply {
    typedef std::pair<T1,T2> type;
  };
};



int main(int argc, char *argv[])
{
    {
  typedef mpl::vector<int,long> seq1;
  typedef mpl::vector<float,double> seq2;
  typedef mpl::vector<std::pair<int,float>,std::pair<long,double> > pairs;

  typedef mpl::transform<seq1,seq2,my_make_pair>::type result;

  static_assert( mpl::equal<result,pairs>::type::value, "");
    }

  typedef fusion::vector<int,long> seq1;

  typedef fusion::vector<float,double> seq2;
  typedef fusion::vector<std::pair<int,float>,std::pair<long,double> >
pairs;

  typedef mpl::transform<seq1,seq2,my_make_pair>::type result;

  static_assert( mpl::equal<result,pairs>::type::value, "");
  mpl::push_back<fusion::vector<>,int>::type z;

  return 0;
}*/
