#ifndef boost_xml_dom_comment_hpp_
#define boost_xml_dom_comment_hpp_

#include <boost/xml/dom/node.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> class element;

template <typename S>
class comment : public node<S>
{
friend class element<S>;
public:
  S content() const;
  void set_content(S const &);

// private:
  comment(xmlNode *a) : node<S>(a) {}
};

template <typename S>
S comment<S>::content() const
{
  xmlChar *content = xmlNodeGetContent(this->impl());
  S retn = converter<S>::out(content);
  xmlFree(content);
  return retn;
}

template <typename S>
void comment<S>::set_content(S const &content)
{
  xmlSetContent(this->impl(), converter<S>::in(content));
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
