#ifndef BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_NUMBERED_HPP
#define BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_NUMBERED_HPP
#include <ostream>
#include <iomanip>
#include "unsigned_safe.hpp"
namespace boost{namespace io{namespace filters{

template
  < typename PipelineContents
  >
class ofilter_leftmargin_numbered
  //. Provide a left margin with the line number.
  {
  public:
        typedef
      typename PipelineContents::fluid_type
    fluid_type
      ;
        typedef
      typename PipelineContents::traits_type
    traits_type
      ;
	      typedef
	    typename traits_type::int_type
    int_type
      ;
    ofilter_leftmargin_numbered
      ( unsigned a_number = 1
      , unsigned a_width =6
      )
      : m_number(a_number)
      , m_width(a_width)
      {}
			unsigned int
		get_number(void)const
		  { return m_number.as_unsigned()
			;}
			unsigned int
		get_width(void)const
		  { return m_width.as_unsigned()
			;}
      ofilter_leftmargin_numbered const&
    put_number(unsigned int a_number)
      {
      ; m_number = a_number
      ; return *this
      ;}
      ofilter_leftmargin_numbered const&
    put_width(unsigned int a_width)
      {
      ; m_width = a_width
      ; return *this
      ;}
      void
    insert_margin(std::basic_ostream<fluid_type,traits_type>& a_os)
      { 
      ; a_os<<std::setw(m_width.as_unsigned())<<++m_number<<":"
      ;}
  private:
      unsigned_safe
    m_number
      //. current number of lines
      ;
      unsigned_safe
    m_width
      //. margin width
      ;
  };//end ofilter_leftmargin_numbered class
  
}}}//exit namespace boost::io::filters
#endif
