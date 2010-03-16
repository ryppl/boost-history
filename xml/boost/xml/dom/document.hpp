#ifndef boost_xml_dom_document_hpp_
#define boost_xml_dom_document_hpp_

#include <boost/xml/dom/element.hpp>
#include <boost/xml/dom/dtd.hpp>
#include <boost/xml/dom/io.hpp>
#include <string>
#include <memory>
#include <stdexcept>
#include <cassert>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> void process_xinclude(document<S> &);

template <typename S>
class document : public detail::wrapper<xmlDoc*>
{
  friend std::auto_ptr<document<S> > detail::factory<S>(xmlDoc *);
  friend void process_xinclude<>(document<S> &);
public:
  document(std::string const &version = "1.0")
    : detail::wrapper<xmlDoc*>(xmlNewDoc((xmlChar *)version.c_str())) {}
  ~document() { xmlFreeDoc(this->impl());}
  std::string encoding() const;
  dtd_ptr<S> internal_subset() const;
  dtd_ptr<S> create_internal_subset(std::string const &name,
				    std::string const &external_id,
				    std::string const &system_id);

  node_ptr<element<S> const> root() const;
  node_ptr<element<S> > root();
  node_ptr<element<S> > create_root(std::string const &name,
				    std::string const &ns = std::string(),
				    std::string const &ns_prefix = std::string());
private:
  document(xmlDoc *doc) : detail::wrapper<xmlDoc*>(doc) {}
};

template <typename S>
inline std::string document<S>::encoding() const
{
  std::string e;
  if(this->impl()->encoding) e = static_cast<char *>(this->impl()->encoding);
  return e;
}

template <typename S>
inline dtd_ptr<S>
document<S>::internal_subset() const
{
  return dtd<S>(xmlGetIntSubset(this->impl()));
}

template <typename S>
inline dtd_ptr<S>
document<S>::create_internal_subset(std::string const &name,
				    std::string const &external_id,
				    std::string const &system_id)
{
  xmlDtd *d = xmlCreateIntSubset(this->impl(),
				 reinterpret_cast<xmlChar const *>(name.c_str()),
				 reinterpret_cast<xmlChar const *>(external_id.c_str()),
				 reinterpret_cast<xmlChar const *>(system_id.c_str()));
  return dtd<S>(d);
}

template <typename S>
inline node_ptr<element<S> const> document<S>::root() const
{
  xmlNode *root = xmlDocGetRootElement(this->impl());
  assert(root);
  return detail::ptr_factory<element<S> const>(root);
}

template <typename S>
inline node_ptr<element<S> > document<S>::root()
{
  xmlNode *root = xmlDocGetRootElement(this->impl());
  assert(root);
  return element<S>(root);
}

template <typename S>
inline node_ptr<element<S> > 
document<S>::create_root(std::string const &name,
			 std::string const &ns_href,
			 std::string const &ns_prefix)
{
  xmlNode *node = xmlNewDocNode(this->impl(), 0, (xmlChar *)name.c_str(), 0);
  if(!ns_prefix.empty())
    xmlNewNs(node,
	     (xmlChar *)(ns_href.empty() ? 0 : ns_href.c_str()),
	     (xmlChar *) ns_prefix.c_str());

  xmlDocSetRootElement(this->impl(), node);

  return root();
}

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
