//Purpose:
//  1) Show padding in aligned struct's.
//  2) Partial response to post:
//       http://lists.boost.org/Archives/boost/2010/10/171569.php
//     with:
//       Subject: Re: [boost] [optional] memory use for optional refs and ptrs
//       From: Rutger ter Borg (rutger_at_[hidden])
//       Date: 2010-10-06 14:33:43 
//
#include <boost/composite_storage/utility/show_layout_all_of_aligned.hpp>
using namespace boost::composite_storage::layout;

int main(void)
{
        typedef 
      boost::mpl::package
      < int
      , double
      , double
      , std::string
      >
    A_type;
    show_layout_all_of_aligned<A_type>("A_type");
        typedef 
      boost::mpl::package
      < int
      , double
      , double
      , std::string
      , bool
      , bool
      , bool
      , bool
      >
    A_bools_type;
    show_layout_all_of_aligned<A_bools_type>("A_bools_type");
        typedef 
      boost::mpl::package
      < bool
      , bool
      , bool
      , bool
      , int
      , double
      , double
      , std::string
      >
    bools_A_type;
    show_layout_all_of_aligned<bools_A_type>("bools_A_type");
    return 0;
}
