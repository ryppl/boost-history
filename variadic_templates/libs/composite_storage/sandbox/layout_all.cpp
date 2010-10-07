//Purpose:
//  1) Show padding in aligned struct's.
//  2) Partial response to post:
//       http://lists.boost.org/Archives/boost/2010/10/171569.php
//     with:
//       Subject: Re: [boost] [optional] memory use for optional refs and ptrs
//       From: Rutger ter Borg (rutger_at_[hidden])
//       Date: 2010-10-06 14:33:43 
//
#include <boost/composite_storage/layout/operators_all_of_aligned.hpp>
#include <boost/mpl/package.hpp>
#include <boost/mpl/fold_assoc_seq.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <iostream>

namespace boost
{
namespace composite_storage
{
namespace layout
{
  template
  < typename Pack
  >
struct size_align_all
;
  template
  < typename... T
  >
struct size_align_all
  < mpl::package<T...>
  >
{
        typedef
      mpl::package
      < typename components_aligned_yes::layout_of<T>...
      >
    type
    ;
};

struct print_layout_one
{
      template
      < std::size_t Size
      , std::size_t Align
      >
      void
    operator()(typename components_aligned_yes::template layout_data<Size,Align>)
    {
        std::cout<<"( size="<<Size<<", align="<<Align<<")\n";
    }
};

  template
  < typename Pack
  >
struct layout_all
;
  template
  < typename... T
  >
struct layout_all
  < mpl::package<T...>
  >
{
        typedef
      typename size_align_all<mpl::package<T...> >::type
    layouts
    ;
        typedef
      components_aligned_yes::layout_data<>
    layout0
    ;
        typedef
      typename mpl::fold_assoc_seq
      < mpl::assoc_left
      , operators<tags::all_of_aligned>::template
        compose_layout
        < mpl::arg<1>
        , mpl::arg<2>
        >
      , layout0
      , layouts
      >::type
    type
    ;
};

struct print_layout_all
{
      std::size_t
    padding_all
    ;
    print_layout_all(void)
    : padding_all(0)
    {}
        typedef
      components_aligned_yes::layout_data<>
    layout0
    ;
      void
    operator()(layout0)
    {
        std::cout<<"layout0"
        <<"\n< size="<<layout0::size
        <<"\n, align="<<layout0::align
        <<"\n>\n";
        padding_all=0;
    }
      template
      < typename HeadComposed
      , typename TailLayout
      >
      void
    operator()
      ( operators<tags::all_of_aligned>::template
        compose_layout
        < HeadComposed
        , TailLayout
        >
      )
    {
            typedef
          operators<tags::all_of_aligned>::template
          compose_layout
          < HeadComposed
          , TailLayout
          >
        composition
        ;
        std::cout<<"composition"
        <<"\n< offset="<<composition::offset
        <<"\n, size="<<composition::size
        <<"\n, align="<<composition::align
        <<"\n>\n";
        std::size_t const padding_one=composition::offset-HeadComposed::size;
        std::cout<<"padding_one="<<padding_one<<"\n";
        padding_all+=padding_one;
        std::cout<<"padding_all="<<padding_all<<"\n";
        HeadComposed head;
        this->operator()(head);
    }
};

}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace

using namespace boost::composite_storage::layout;

int main(void)
{
        typedef
      boost::mpl::package<char,int,double>
    pack;
        typedef
      size_align_all<pack>::type
    size_align_result;
    std::cout<<"mpl::size<result_t>="<<boost::mpl::size<size_align_result>::type::value<<"\n";
    print_layout_one plo;
    boost::mpl::for_each<size_align_result>(plo);
        typedef
      layout_all<pack>::type
    layout_result;
    print_layout_all pla;
    pla(layout_result());
    return 0;
}
