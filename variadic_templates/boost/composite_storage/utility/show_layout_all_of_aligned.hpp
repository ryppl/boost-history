//print layout for struct of types
#ifndef BOOST_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
#define BOOST_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
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
    print_layout_all(std::size_t a_padding=0)
    : padding_all(a_padding)
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
    
      template
      < typename Composed
      >
        static
      void
    _(void)
    {
        std::size_t const
      composite_size
      = alignment::aligned_offset
        < Composed::size
        , Composed::align
        >::value
       /* aligned_size is the size required
        * to assure that when two elements of composite
        * are in a vector, the 2nd would be aligned
        * properly.
        */
      ;
        std::size_t const 
      composite_padding
      = composite_size-Composed::size
       /* padding at the end of the composite.
        */
      ;
      std::cout
        <<"[ composite_size="
        <<composite_size
        <<", composite_padding="
        <<composite_padding
        <<"]\n";
      print_layout_all pla(composite_padding);
      Composed composed;
      pla(composed);
    }
};

  template
  < typename Pack
  >
  void
show_layout_all_of_aligned(char const*title)
{
    std::cout<<title<<"\n";
    std::cout<<"===========\n";
        typedef
      typename size_align_all<Pack>::type
    size_align_result;
    print_layout_one plo;
    boost::mpl::for_each<size_align_result>(plo);
        typedef
      typename layout_all<Pack>::type
    layout_result;
    print_layout_all::_<layout_result>();
}

}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif
