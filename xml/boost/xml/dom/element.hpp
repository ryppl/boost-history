#ifndef boost_xml_dom_element_hpp_
#define boost_xml_dom_element_hpp_

#include <boost/xml/dom/node.hpp>
#include <boost/xml/dom/attribute.hpp>
#include <boost/xml/dom/text.hpp>
#include <boost/xml/dom/comment.hpp>
#include <boost/xml/dom/pi.hpp>
#include <boost/xml/dom/cdata.hpp>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S>
class element : public node<S>
{
  friend class document<S>;
  friend node_ptr<element<S> > detail::ptr_factory<>(xmlNode *);
public:
  typedef detail::node_iterator<node<S> > child_iterator;
  typedef detail::node_iterator<node<S> const> const_child_iterator;

  typedef detail::node_iterator<dom::attribute<S> > attribute_iterator;
  typedef detail::node_iterator<dom::attribute<S> const> const_attribute_iterator;

  //. Return the attribute for the given name, if it exists.
  node_ptr<dom::attribute<S> const> attribute(S const &name) const;
  //. Return the attribute for the given name, if it exists.
  node_ptr<dom::attribute<S> > attribute(S const &name);
  //. Set an attribute with the given name and value.
  node_ptr<dom::attribute<S> > set_attribute(S const &name, S const &value);
  //. Remove the attribute with the given name and value, if it exists.
  void unset_attribute(S const &name);

  attribute_iterator begin_attributes() 
  {
    xmlAttr *attr = this->impl()->properties;
    return attribute_iterator(reinterpret_cast<xmlNode *>(attr));
  }
  const_attribute_iterator begin_attributes() const 
  {
    xmlAttr *attr = this->impl()->properties;
    return const_attribute_iterator(reinterpret_cast<xmlNode *>(attr));
  }
  attribute_iterator end_attributes() 
  {
    return attribute_iterator();
  }
  const_attribute_iterator end_attributes() const 
  {
    return const_attribute_iterator();
  }

  //. Append a new child element to this element.
  node_ptr<element<S> > append_element(S const &name);
  //. Insert a new child element.
  node_ptr<element<S> > insert_element(child_iterator, S const &name);

  //. Append text to this element.
  node_ptr<text<S> > append_text(S const &content);
  //. Insert text.
  node_ptr<text<S> > insert_text(child_iterator, S const &content);

  //. Append a new comment to this element.
  node_ptr<comment<S> > append_comment(S const &content);
  //. Insert a new comment.
  node_ptr<comment<S> > insert_comment(child_iterator, S const &content);

  //. Append a new processing instruction to this element.
  node_ptr<pi<S> > append_pi(S const &name, S const &content);
  //. Insert a new processing instruction.
  node_ptr<pi<S> > insert_pi(child_iterator, S const &name, S const &content);

  //. Append a new CDATA block to this element.
  node_ptr<cdata<S> > append_cdata(S const &content);
  //. Insert a new CDATA block.
  node_ptr<cdata<S> > insert_cdata(child_iterator, S const &content);

  //. Move the given node from its present location into this element.
  node_ptr<node<S> > insert(child_iterator, node_ptr<node<S> > node);
  void remove(child_iterator);

  // FIXME: the implementation of has_content needs a full traversal
  //        of the subtree. 
  // bool has_content() const;
  S content() const;
  void set_content(S const &content);
  void append_content(S const &content);

  child_iterator begin_children() 
  { return child_iterator(this->impl()->children);}
  const_child_iterator begin_children() const 
  { return const_child_iterator(this->impl()->children);}
  child_iterator end_children() 
  { return child_iterator();}
  const_child_iterator end_children() const 
  { return const_child_iterator();}

  //. Declare the namespace prefix to be an alias for uri.
  void declare_namespace(S const &prefix, S const &uri);

  //. Lookup the declaration of the namespace prefix, and return its uri.
  S lookup_namespace(S const &prefix) const;

  const_child_iterator find(node_ptr<node<S> const>) const;
  child_iterator find(node_ptr<node<S> const>);

protected:
  element(xmlNode *e) : node<S>(e) {}
};

template <typename S>
inline node_ptr<dom::attribute<S> const> element<S>::attribute(S const &name) const
{
  xmlChar *n = converter<S>::in(name);
  for(xmlAttr *attr = this->impl()->properties; attr; attr = attr->next)
  {
    if(xmlStrEqual(attr->name, n))
      return dom::attribute<S>((xmlNode *)attr);
  }
  return dom::attribute<S>(0);
}

template <typename S>
inline node_ptr<dom::attribute<S> > element<S>::attribute(S const &name)
{
  xmlChar *n = converter<S>::in(name);
  for(xmlAttr *attr = this->impl()->properties; attr; attr = attr->next)
  {
    if(xmlStrEqual(attr->name, n))
      return dom::attribute<S>((xmlNode *)attr);
  }
  return dom::attribute<S>(0);
}

template <typename S>
inline node_ptr<attribute<S> > 
element<S>::set_attribute(S const &name, S const &value)
{
  xmlAttr *attr = xmlSetProp(this->impl(),
                             converter<S>::in(name),
                             converter<S>::in(value));
  return dom::attribute<S>(attr);
}

template <typename S>
inline void element<S>::unset_attribute(S const &name)
{
  xmlUnsetProp(this->impl(), converter<S>::in(name));
}

template <typename S>
inline node_ptr<element<S> > 
element<S>::append_element(S const &name)
{
  xmlNode *node = xmlAddChild(this->impl(), xmlNewNode(0, converter<S>::in(name)));
  return element<S>(node);
}

template <typename S>
inline node_ptr<element<S> > 
element<S>::insert_element(child_iterator i, S const &name)
{
  xmlNode *node = xmlNewNode(0, converter<S>::in(name));
  if (impl(i))
    node = xmlAddPrevSibling(impl(i), node);
  else
    node = xmlAddChild(this->impl(), node);
  return element<S>(node);
}

template <typename S>
inline node_ptr<text<S> > 
element<S>::append_text(S const &name)
{
  xmlNode *node = xmlAddChild(this->impl(), xmlNewText(converter<S>::in(name)));
  return text<S>(node);
}

template <typename S>
inline node_ptr<text<S> > 
element<S>::insert_text(child_iterator i, S const &name)
{
  xmlNode *node = xmlNewText(converter<S>::in(name));
  if (impl(i))
    node = xmlAddPrevSibling(impl(i), node);
  else
    node = xmlAddChild(this->impl(), node);
  return text<S>(node);
}

template <typename S>
inline node_ptr<comment<S> >
element<S>::append_comment(S const &content)
{
  xmlNode *node = xmlAddChild(this->impl(),
			      xmlNewComment(converter<S>::in(content)));
  return comment<S>(node);
}

template <typename S>
inline node_ptr<comment<S> >
element<S>::insert_comment(child_iterator i, S const &content)
{
  xmlNode *node = xmlNewComment(converter<S>::in(content));
  if (impl(i))
    node = xmlAddPrevSibling(impl(i), node);
  else
    node = xmlAddChild(this->impl(), node);
  return comment<S>(node);
}

template <typename S>
inline node_ptr<pi<S> >
element<S>::append_pi(S const &name, S const &content)
{
  xmlNode *node = xmlAddChild(this->impl(), xmlNewPI(converter<S>::in(name),
						     converter<S>::in(content)));
  return pi<S>(node);
}

template <typename S>
inline node_ptr<pi<S> >
element<S>::insert_pi(child_iterator i,
		      S const &name,
		      S const &content)
{
  xmlNode *node = xmlNewPI(converter<S>::in(name), converter<S>::in(content));
  if (impl(i))
    node = xmlAddPrevSibling(impl(i), node);
  else
    node = xmlAddChild(this->impl(), node);
  return pi<S>(node);
}

template <typename S>
inline node_ptr<cdata<S> >
element<S>::append_cdata(S const &content)
{
  xmlDoc *doc = this->impl()->doc;
  xmlChar *data = converter<S>::in(content);
  size_t length = xmlStrlen(data);
  xmlNode *node = xmlAddChild(this->impl(), xmlNewCDataBlock(doc, data, length));
  return cdata<S>(node);
}

template <typename S>
inline node_ptr<cdata<S> >
element<S>::insert_cdata(child_iterator i, S const &content)
{
  xmlDoc *doc = this->impl()->doc;
  xmlChar *data = converter<S>::in(content);
  size_t length = xmlStrlen(data);
  xmlNode *node = xmlNewCDataBlock(doc, data, length);
  if (impl(i))
    node = xmlAddPrevSibling(impl(i), node);
  else
    node = xmlAddChild(this->impl(), node);
  return cdata<S>(node);
}

template <typename S>
inline node_ptr<node<S> > element<S>::insert(child_iterator i,
					     node_ptr<node<S> > child)
{
  xmlUnlinkNode(impl(*child));
  if (impl(i))
    return node<S>(xmlAddPrevSibling(impl(i), impl(*child)));
  else
    return node<S>(xmlAddChild(this->impl(), impl(*child)));
}

template <typename S>
inline void element<S>::remove(typename element<S>::child_iterator i)
{
  if (i != end_children())
  {
    xmlUnlinkNode(impl(i));
    xmlFreeNode(impl(i));
  }
}

template <typename S>
inline S element<S>::content() const
{
  xmlChar *content = xmlNodeGetContent(this->impl());
  S retn = converter<S>::out(content);
  xmlFree(content);
  return retn;
}

template <typename S>
inline void element<S>::set_content(S const &content)
{
  xmlNodeSetContent(this->impl(), converter<S>::in(content));
}

template <typename S>
inline void element<S>::append_content(S const &content)
{
  xmlNodeAddContent(this->impl(), converter<S>::in(content));
}

template <typename S>
inline void element<S>::declare_namespace(S const &prefix, S const &uri)
{
  xmlNewNs(this->impl(), converter<S>::in(uri), converter<S>::in(prefix));
}

template <typename S>
S element<S>::lookup_namespace(S const &prefix) const
{
  xmlNode const *self = this->impl();
  xmlNs const *ns = xmlSearchNs(self->doc, self, converter<S>::in(prefix));
  return converter<S>::out(ns ? ns->href : (xmlChar*)"");
}

template <typename S>
typename element<S>::const_child_iterator 
element<S>::find(node_ptr<node<S> const> n) const
{
  xmlNode *child = impl(*n);
  for (const_child_iterator i = begin_children(); i != end_children(); ++i)
    if (**i == n) return i;
  return end_children();
}

template <typename S>
typename element<S>::child_iterator 
element<S>::find(node_ptr<node<S> const> n)
{
  //  xmlNode *child = impl(*n);
  for (child_iterator i = begin_children(); i != end_children(); ++i)
    if (**i == *n) return i;
  return end_children();
}
} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
