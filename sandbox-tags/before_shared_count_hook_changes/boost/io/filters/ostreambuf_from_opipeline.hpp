#ifndef BOOST_IO_FILTERS_OSTREAMBUF_FROM_OPIPELINE_HPP
#define BOOST_IO_FILTERS_OSTREAMBUF_FROM_OPIPELINE_HPP
//  (C) Copyright Larry Joe Evans 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
#include <streambuf>
namespace boost{namespace io{namespace filters{

template<typename OutputPipeline>
class ostreambuf_from_opipeline
  //. Purpose:
  //.   Adapt an OutputPipeline to
  //.   ostreambuf interface.
  : public std::basic_streambuf
    < typename OutputPipeline::fluid_type
    , typename OutputPipeline::traits_type
    >
  {
  public:
        typedef
      OutputPipeline
    opipeline_type
      ;
        typedef
      typename opipeline_type::fluid_type
    fluid_type
      ;
        typedef
      typename opipeline_type::traits_type
    traits_type
      ;
        typedef
      typename traits_type::int_type
    int_type
      ;
        typedef
      std::basic_streambuf<fluid_type,traits_type>
    super_type
      ;
    ostreambuf_from_opipeline(opipeline_type* a_pipeline)
      : m_pipeline(a_pipeline)
      , m_overflow_result('\0') //arbitrary choice
      {
      ; super_type::setbuf(0,0) //so all xsputn char's passed to overflow immediately.
      ;}
      int_type
    overflow(int_type i=traits_type::eof())
      { fluid_type c = fluid_type(i)
      ; m_overflow_result = m_pipeline->pumpout_fluid(c)
      ; return m_overflow_result
      ;}
      int_type
    overflow_result(void)const
      { return m_overflow_result
      ;}
      opipeline_type*
    out_pipeline(void)
      { return m_pipeline
      ;}
  private:
      opipeline_type*
    m_pipeline
      ;
      int_type
    m_overflow_result
      ;
  };
}}}//exit namespace boost::io::filters
#endif
