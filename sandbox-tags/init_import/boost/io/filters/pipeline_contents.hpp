#ifndef BOOST_IO_FILTERS_PIPELINE_CONTENTS_HPP
#define BOOST_IO_FILTERS_PIPELINE_CONTENTS_HPP
#include <string>
namespace boost{namespace io{namespace filters{
template
  < typename CharType=char
  , typename TraitsType=std::char_traits<CharType> 
  >
class pipeline_contents
  //. Purpose:
  //.   Convenient place for renaming streambuf
  //.   names to suggest pipeline filter analogy.
  {
  public:
          typedef 
        CharType
      fluid_type
        ;
          typedef 
        TraitsType
      traits_type
        ;
  };
}}}//exit namespace boost::io::filters
#endif
