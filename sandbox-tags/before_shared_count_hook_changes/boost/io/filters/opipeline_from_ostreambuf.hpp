#ifndef BOOST_IO_FILTERS_OPIPELINE_FROM_OSTREAMBUF_HPP
#define BOOST_IO_FILTERS_OPIPELINE_FROM_OSTREAMBUF_HPP
//  (C) Copyright Larry Joe Evans 2003.  Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies.  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
#include "pipeline_contents.hpp"
#include <streambuf>
namespace boost{namespace io{namespace filters{
template
  < typename DownstreamPipelineContents=pipeline_contents<char>
  >
class opipeline_from_ostreambuf
  //. Purpose:
  //.   Adapt an output streambuf
  //.   to pipeline interface (i.e. has a pumpout_fluid method).
  {
  public:
        typedef
      DownstreamPipelineContents
    contents_type
      ;
        typedef
      typename std::basic_streambuf
        < typename contents_type::fluid_type
        , typename contents_type::traits_type
        >
    sink_type
      ;
        typedef
      typename contents_type::fluid_type
    fluid_type
      ;
        typedef
      typename contents_type::traits_type
    traits_type
      ;
        typedef
      typename traits_type::int_type
    int_type
      ;
    opipeline_from_ostreambuf(sink_type* a_sink=0)
      : m_sink(a_sink)
      {}
      void
    attach_sink(sink_type* a_sink=0)
      { m_sink = a_sink
      ;}
      bool
    attached(void)const
      { return m_sink != 0
      ;}
      int_type
    pumpout_fluid(fluid_type c)
      { 
      ; int_type result = m_sink->sputc(c)
      ; return result
      ;}
  private:
      sink_type*
    m_sink
      ;
  };
}}}//exit namespace boost::io::filters
#endif
