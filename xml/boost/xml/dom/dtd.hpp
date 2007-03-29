#ifndef boost_xml_dom_dtd_hpp_
#define boost_xml_dom_dtd_hpp_

#include <libxml/tree.h>
#include <string>

namespace boost
{
namespace xml
{
namespace dom
{

template <typename S> class document;
template <typename S> class dtd_ptr;

template <typename S>
class dtd
{
  friend class document<S>;
  friend class dtd_ptr<S>;
public:
  std::string name() const { return static_cast<char *>(impl_->name);}
  std::string external_id() const { return static_cast<char *>(impl_->ExternalID);}
  std::string system_id() const { return static_cast<char *>(impl_->SystemID);}
  
private:
  dtd(xmlDtd *dtd) : impl_(dtd) {}
  ~dtd() {}

  xmlDtd *impl_;
};

template <typename S>
class dtd_ptr
{
public:
  dtd_ptr() : dtd_(0) {}
  dtd_ptr(dtd<S> const &d) : dtd_(d) {}
  dtd<S> &operator*() { return this->dtd_;}
  dtd<S> *operator->() { return &this->dtd_;}
  dtd<S> *get() { return &this->dtd_;}
  operator bool() const { return this->dtd_.impl_;}

private:
  dtd<S> dtd_;
};

} // namespace boost::xml::dom
} // namespace boost::xml
} // namespace boost

#endif
