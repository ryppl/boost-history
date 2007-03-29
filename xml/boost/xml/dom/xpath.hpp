#ifndef boost_xml_dom_xpath_hpp_
#define boost_xml_dom_xpath_hpp_

#include <libxml/xpath.h>
#include <boost/xml/dom/document.hpp>
#include <boost/xml/dom/node_set.hpp>
#include <boost/xml/dom/detail.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S>
class xpath : detail::wrapper<xmlXPathContext*>
{
public:
  xpath(S const &path)
    : detail::wrapper<xmlXPathContext*>(xmlXPathNewContext(0)),
      path_(xmlStrdup(converter<S>::in(path)))
  {}
  ~xpath()
  {
    xmlFree(this->path_);
    xmlXPathFreeContext(this->impl());
  }
  void declare_namespace(S const &prefix, S const &uri)
  {
    xmlXPathRegisterNs(this->impl(),
		       converter<S>::in(prefix), converter<S>::in(uri));
  }

  S string() const { return converter<S>::out(this->path_);}

  node_set<S> find(node_ptr<node<S> const>);
private:
  xmlChar         *path_;
};

// FIXME: the result of an xpath find call may have one of these types:
//        [node-set, boolean, number, string]
//        Use an appropriate boost type (variant ?) to represent the result.
template <typename S>
inline node_set<S> xpath<S>::find(node_ptr<node<S> const> base)
{
  this->impl()->node = impl(*base);
  if (!this->impl()->doc)
    this->impl()->doc = impl(*base)->doc;
  xmlXPathObject *result = xmlXPathEval(this->path_, this->impl());
  if (result->type != XPATH_NODESET)
  {
    xmlXPathFreeObject(result);
    throw std::invalid_argument("sorry, only nodeset result types supported for now.");
  }
  node_set<S> nodeset(result->nodesetval);
  xmlXPathFreeObject(result);
  return nodeset;
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
