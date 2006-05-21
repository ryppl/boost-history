#ifndef BOOST_IO_FILTERS_MARG_OSTREAM_HPP
#define BOOST_IO_FILTERS_MARG_OSTREAM_HPP
#include "opipe_filtered.hpp"
#include "ofilter_leftmargin_abstract.hpp"
#include "ofilter_leftmargin_adjustable.hpp"
#include "opipeline_from_ostreambuf.hpp"
#include "ostream_from_opipeline.hpp"
namespace boost{namespace io{namespace filters{
      typedef 
    ostream_from_opipeline
      < opipe_filtered
          < ofilter_leftmargin_abstract
              < ofilter_leftmargin_adjustable
              >
          , opipeline_from_ostreambuf
              < pipeline_contents
                  < char
                  >
              > 
          >
      > 
  ostream_leftmargin_adjustable
    ;
}}}//exit namespace boost::io::filters
    typedef
  boost::io::filters::ostream_leftmargin_adjustable
marg_ostream
  ;
#endif
