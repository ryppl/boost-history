#ifndef boost_xml_dom_xinclude_hpp_
#define boost_xml_dom_xinclude_hpp_

#include <libxml/xinclude.h>
#include <boost/xml/dom/document.hpp>
#include <boost/xml/dom/element.hpp>

namespace boost
{
namespace xml
{
namespace dom
{
namespace detail
{
class xinclude_processor : public accessor
{
public:
  template <typename S>
  void process(document<S> &d) { xmlXIncludeProcess(impl(d));}
  template <typename S>
  void process(node_ptr<element<S> > e) { xmlXIncludeProcessTree(impl(*e));}
};

}

//. Process xincludes for the given document.
template <typename S>
inline void process_xinclude(document<S> &d)
{
  detail::xinclude_processor processor;
  processor.process(d);
}

//. Process xincludes within the given element.
template <typename S>
inline void process_xinclude(node_ptr<element<S> > e)
{
  detail::xinclude_processor processor;
  processor.process(e);
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
