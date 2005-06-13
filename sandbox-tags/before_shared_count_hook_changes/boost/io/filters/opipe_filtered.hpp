#ifndef BOOST_IO_FILTERS_OPIPE_FILTERED_HPP
#define BOOST_IO_FILTERS_OPIPE_FILTERED_HPP
namespace boost{namespace io{namespace filters{

template
  < typename OutFilter
  , typename DownstreamPipeline
  >
class opipe_filtered
  : public DownstreamPipeline
  , public OutFilter
  //. Purpose:
  //.   Represents a "pipe" with a "filter" in a pipeline.
  //. TemplateArgs:
  //.   DownstreamPipeline: superclass represents the
  //.     pipeline downstream from this "pipe".
  //.   OutFilter: superclass performs any "extra" processing
  //.     of the "fluid".
  {
  public:
        typedef
      DownstreamPipeline
    downstream_type
      ;
        typedef
      typename downstream_type::contents_type
    contents_type
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
        typedef
      OutFilter
    filter_type
      ;
    opipe_filtered(void)
      {}
      downstream_type*
    downstream_pipeline(void)
      { return static_cast<downstream_type*>(this)
      ;}
      downstream_type const*
    downstream_pipeline(void)const
      { return static_cast<downstream_type const*>(this)
      ;}
      int_type
    pumpout_fluid(fluid_type c)
      { downstream_type* l_pipeline = static_cast<downstream_type*>(this)
      ; int_type result = this->filter_type::filter_fluid_to_pipeline(c,l_pipeline)
      ; return result
      ;}
  };
  
}}}//exit namespace boost::io::filters
#endif
