#ifndef BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_ABSTRACT_HPP
#define BOOST_IO_FILTERS_OFILTER_LEFTMARGIN_ABSTRACT_HPP
#include "pipeline_contents.hpp"
#include "ostreambuf_from_opipeline.hpp"
namespace boost{namespace io{namespace filters{
template
  < template <typename F>class OutFilterLeftMarginConcrete
  , typename PipelineContents=pipeline_contents<char>
  >
class ofilter_leftmargin_abstract
  : public OutFilterLeftMarginConcrete<PipelineContents>
  {
  public:
        typedef
      OutFilterLeftMarginConcrete<PipelineContents>
    super_type
      ;
        typedef
      typename super_type::int_type
    int_type
      ;
        typedef
      typename super_type::fluid_type
    fluid_type
      ;
    ofilter_leftmargin_abstract(void)
      : m_bol(false)
      {}
    template<typename DownstreamPipeline>
      int_type
    filter_fluid_to_pipeline(fluid_type c, DownstreamPipeline* a_pipeline)
      { if(m_bol)
        { 
        ; ostreambuf_from_opipeline<DownstreamPipeline> l_sbuf(a_pipeline)
        ; typedef typename DownstreamPipeline::contents_type::fluid_type fluid_type
        ; typedef typename DownstreamPipeline::contents_type::traits_type traits_type
        ; std::basic_streambuf<fluid_type,traits_type>* p_sbuf = &l_sbuf
        ; std::basic_ostream<fluid_type,traits_type> l_os(p_sbuf)
        ; super_type::insert_margin(l_os)
        ;}
      ; m_bol = (c == '\n')
      ; int_type l_result = a_pipeline->pumpout_fluid(c)
      ; return l_result
      ;}
      bool
    bol(void)const
      //. Indicates whether at the "Beginning-Of-Line"
      { return m_bol
      ;}
  private:
      bool
    m_bol
      //. records whether last character forwarded was an end-of-line.
      ;
  };
}}}//exit namespace boost::io::filters
#endif
