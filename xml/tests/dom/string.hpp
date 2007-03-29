#include <boost/xml/dom.hpp>
#include <string>

namespace boost
{
namespace xml
{
namespace dom
{
template <>
struct converter<std::string>
{
  static xmlChar *in(std::string const &buffer)
  {
    return (xmlChar *)buffer.c_str();
  }
  static std::string out(xmlChar const *buffer)
  {
    return buffer ? std::string((char *)buffer) : std::string();
  }
};
}
}
}

typedef boost::xml::dom::node_ptr<boost::xml::dom::node<std::string> > node_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::node<std::string> const> const_node_ptr;
typedef boost::xml::dom::element<std::string> element;
typedef boost::xml::dom::node_ptr<element> element_ptr;
typedef boost::xml::dom::node_ptr<element const> const_element_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::attribute<std::string> > attribute_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::attribute<std::string> const> const_attribute_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::text<std::string> > text_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::text<std::string> const> const_text_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::pi<std::string> > pi_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::pi<std::string> const> const_pi_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::comment<std::string> > comment_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::comment<std::string> const> const_comment_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::cdata<std::string> > cdata_ptr;
typedef boost::xml::dom::node_ptr<boost::xml::dom::cdata<std::string> const> const_cdata_ptr;
typedef std::auto_ptr<boost::xml::dom::document<std::string> > document_ptr;
typedef boost::xml::dom::dtd_ptr<std::string> dtd_ptr;
typedef boost::xml::dom::node_set<std::string> node_set;
typedef boost::xml::dom::xpath<std::string> xpath;
