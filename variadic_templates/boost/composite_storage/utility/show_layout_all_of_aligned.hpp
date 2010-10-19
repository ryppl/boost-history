//print layout for struct of types
#ifndef BOOST_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
#define BOOST_COMPOSITE_STORAGE_UTILITY_LAYOUT_AT_INCLUDED
#include <boost/composite_storage/layout/operators_all_of_aligned.hpp>
#include <boost/mpl/package.hpp>
#include <boost/mpl/fold_assoc_seq.hpp>

#include <boost/mpl/size.hpp>
#include <boost/mpl/for_each.hpp>
#include <iostream>
#include <iomanip>
#include <cstring>

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
    static char const delim=' ';
    enum col_enum
    { num
    , size
    , align
    , sum_size
    };
    static unsigned const ncols=sum_size+1;
        typedef 
      char const* 
    title_col;
        static 
      title_col 
    our_tcols(unsigned icol)
    {
        static title_col const titles[ncols]=
        { "num"
        , "size"
        , "align"
        , "sum_size"
        };
        return titles[icol];
    }
    static unsigned our_wcols(unsigned icol)
    {
        return std::strlen(our_tcols(icol));
    }
    int my_count;
    unsigned my_size_sum;
    print_layout_one(int a_count=0,unsigned a_size_sum=0)
    : my_count(a_count)
    , my_size_sum(a_size_sum)
    {
        std::cout
          <<our_tcols(num)<<delim
          <<our_tcols(size)<<delim
          <<our_tcols(align)<<delim
          <<our_tcols(sum_size)<<delim
          <<"\n";
    }
      template
      < std::size_t Size
      , std::size_t Align
      >
      void
    operator()(typename components_aligned_yes::template layout_data<Size,Align>)
    {
        my_size_sum+=Size;
        std::cout
        <<std::setw(our_wcols(num     ))<<++my_count <<delim
        <<std::setw(our_wcols(size    ))<<Size       <<delim
        <<std::setw(our_wcols(align   ))<<Align      <<delim
        <<std::setw(our_wcols(sum_size))<<my_size_sum<<delim
        <<"\n";
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
    static char const delm=' ';
    static char const none='_';
    enum col_enum
    { num
    , size
    , align
    , pad
    , sum_pad
    , offset
    };
    static unsigned const ncols=offset+1;
        typedef 
      char const* 
    title_col;
        static 
      title_col 
    our_tcols(unsigned icol)
    {
        static title_col const titles[ncols]=
        { "num"
        , "size"
        , "align"
        , "pad"
        , "sum_pad"
        , "offset"
        };
        return titles[icol];
    }
    static unsigned our_wcols(unsigned icol)
    {
        return std::strlen(our_tcols(icol));
    }
      int
    my_count
    ;
      std::size_t
    padding_all
    ;
    print_layout_all(int a_count,std::size_t a_padding=0)
    : my_count(a_count)
    , padding_all(a_padding)
    {}
        typedef
      components_aligned_yes::layout_data<>
    layout0
    ;
        static
      void
    print_all_but_offset
      ( unsigned v_num
      , unsigned v_size
      , unsigned v_align
      , unsigned v_pad
      , unsigned v_sum_pad
      )
    {
      std::cout
        <<std::setw(our_wcols(num))
        <<v_num<<delm
        <<std::setw(our_wcols(size))
        <<v_size<<delm
        <<std::setw(our_wcols(align))
        <<v_align<<delm
        <<std::setw(our_wcols(pad))
        <<v_pad<<delm
        <<std::setw(our_wcols(sum_pad))
        <<v_sum_pad<<delm;
    }
      void
    operator()(layout0)
    {
      std::cout
        <<std::setw(our_wcols(num))
        <<my_count<<delm
        <<std::setw(our_wcols(size))
        <<layout0::size<<delm
        <<std::setw(our_wcols(align))
        <<layout0::align<<delm
        <<std::setw(our_wcols(pad))
        <<none<<delm
        <<std::setw(our_wcols(sum_pad))
        <<none<<delm
        <<std::setw(our_wcols(offset))
        <<none<<delm
        <<"\n";
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
        std::size_t const padding_one=composition::offset-HeadComposed::size;
        padding_all+=padding_one;
        print_all_but_offset
          ( my_count--
          , composition::size
          , composition::align
          , padding_one
          , padding_all
          );
        std::cout
          <<std::setw(our_wcols(offset))
          <<composition::offset<<delm
          <<"\n";
        HeadComposed head;
        this->operator()(head);
    }
    
      template
      < typename Composed
      >
        static
      void
    _(int num_components)
    {
      for(unsigned icol=0; icol<ncols; ++icol)
      {
          std::cout
            <<our_tcols(icol)
            <<delm;
      }
      std::cout<<"\n";
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
      print_all_but_offset
        ( num_components+1
        , composite_size
        , Composed::align
        , composite_padding
        , composite_padding
        );
      std::cout
        <<std::setw(our_wcols(offset))
        <<none<<delm
        <<"\n";
      print_layout_all pla(num_components,composite_padding);
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
    std::cout<<"===========\n";
    std::cout<<title<<"\n";
    std::cout<<"===========\n";
        typedef
      typename size_align_all<Pack>::type
    size_align_result;
    std::cout<<"***components***\n";
    std::cout<<"----------------\n";
    print_layout_one plo;
    boost::mpl::for_each<size_align_result>(plo);
        typedef
      typename layout_all<Pack>::type
    layout_result;
    std::cout<<"***composite***\n";
    std::cout<<"---------------\n";
    print_layout_all::_<layout_result>(mpl::size<Pack>::type::value);
    std::cout<<"---------------\n";
}

}//exit layout namespace
}//exit composite_storage namespace
}//exit boost namespace

#endif
