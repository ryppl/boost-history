#ifndef BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_ADJUSTABLE_HPP
#define BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_ADJUSTABLE_HPP
#include <string>
#include <ostream>
#include <iomanip>
#include "unsigned_safe.hpp"
namespace boost{namespace io{namespace filters{

template
  < typename PipelineContents
  >
class ofilter_leftmargin_adjustable
  //. Provide an adjustable left margin as a PipelineContents filter
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
        typedef
      std::basic_string<fluid_type>
    marg_string
      ;
    ofilter_leftmargin_adjustable
      ( marg_string const& a_margin=""
      )
      : m_margin(a_margin)
      , m_width(0)
      , m_delta(2)
      {}
      marg_string const&
    get_margin(void)const
      { return m_margin
      ;}
			unsigned int
		get_width(void)const
		  { return m_width.as_unsigned()
			;}
			unsigned int
		get_delta(void)const
		  { return m_width.as_unsigned()
			;}
      ofilter_leftmargin_adjustable const&
    put_margin(marg_string const& a_margin)
		  //set margin width
      {
      ; m_margin = a_margin
      ; return *this
      ;}
      ofilter_leftmargin_adjustable const&
    put_delta(unsigned int a_delta)
		  //. set margin delta
      {
      ; m_delta = a_delta
      ; return *this
      ;}
      ofilter_leftmargin_adjustable const&
    put_width(unsigned int a_width)
		  //. set margin width
      {
      ; m_width = a_width
      ; return *this
      ;}
      ofilter_leftmargin_adjustable const& 
    operator+=(int w)
		  //. increment width
      { m_width+=w
      ; return *this
      ;}
      ofilter_leftmargin_adjustable const&
    operator-=(int w)
		  //. decrement width
      { return operator+=(-w)
      ;}
      ofilter_leftmargin_adjustable const&
    operator++(void)
      { return operator+=(m_delta)
      ;}
      ofilter_leftmargin_adjustable const&
    operator--(void)
      { return operator-=(m_delta)
      ;}
      void
    insert_margin(std::basic_ostream<fluid_type,traits_type>& a_os)
      {
      ; a_os<<std::setw(m_width.as_unsigned())<<m_margin
      ;}
  private:
      marg_string
    m_margin
      //. The non-blank part of margin that is right justified in the margin
      ;
      unsigned_safe
    m_width
      //. margin width
      ;
      unsigned int
    m_delta
      //. amount m_width changed by operator{++,--}
      ;
  };//end ofilter_leftmargin_adjustable class

}}}//exit namespace boost::io::filters
#endif
