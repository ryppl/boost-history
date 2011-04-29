#if !defined(PROPERTY_TAG_HPP)
#define PROPERTY_TAG_HPP

namespace properties
{

  struct read_tag { };
  struct write_tag { };
  struct read_write_tag : public read_tag, public write_tag { };
  template <class T> class default_policy_tag { };

}

#endif
