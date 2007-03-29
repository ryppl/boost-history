#ifndef boost_xml_dom_validate_dtd_hpp_
#define boost_xml_dom_validate_dtd_hpp_

#include <libxml/valid.h>
#include <boost/xml/dom/node.hpp>
#include <vector>

namespace boost
{
namespace xml
{
namespace dom
{
namespace detail
{
template <typename S>
class validator : wrapper<xmlValidCtxt*>
{
public:
  validator() : wrapper<xmlValidCtxt*>(xmlNewValidCtxt()) {}
  ~validator() { xmlFreeValidCtxt(impl());}
  bool validate(document<S> const &);
  bool validate(node_ptr<element<S> >);
  std::vector<S> valid_elements(node_ptr<element<S> const>,
				typename element<S>::const_child_iterator);
};

template <typename S>
bool validator<S>::validate(document<S> const &doc)
{
  return xmlValidateDocument(impl(), impl(doc)) == 1;
}

template <typename S>
bool validator<S>::validate(node_ptr<element<S> > e)
{
  xmlNode *node = impl(*e);
  return xmlValidateElement(impl(), node->doc, node) == 1;
}

template <typename S>
std::vector<S> validator<S>::valid_elements(node_ptr<element<S> const> e,
					    typename element<S>::const_child_iterator i)
{
  xmlNode *node = i == e->end_children() ? 0 : impl(**i);
  std::vector<xmlChar const *> names(16);
  std::vector<S> result;
  do
  {
    int elements = xmlValidGetValidElements(node->prev, node,
					    &*names.begin(), names.size());
    if (elements < names.size())
    {
      names.resize(elements);
      break;
    }
    names.resize(names.size() * 2);
  }
  while (true);
  for (std::vector<xmlChar const *>::iterator j = names.begin();
       j != names.end();
       ++j)
    result.push_back(converter<S>::out(*j));
  return result;
}

} // namespace boost::xml::dom::detail

template <typename S>
inline bool validate(document<S> const &doc)
{
  detail::validator<S> v;
  return v.validate(doc);
}

template <typename S>
inline bool validate(node_ptr<element<S> > e)
{
  detail::validator<S> v;
  return v.validate(e);
}

template <typename S>
inline std::vector<S> 
valid_elements(node_ptr<element<S> const> e,
	       typename element<S>::const_child_iterator i)
{
  detail::validator<S> v;
  return v.valid_elements(e, i);
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
