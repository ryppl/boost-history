#if !defined(PROPERTY_TAG_HPP)
#define PROPERTY_TAG_HPP

namespace properties
{

  struct ReadTag { };
  struct WriteTag { };
  struct ReadWriteTag : public ReadTag, public WriteTag { };
  template <class T> class DefaultPolicyTag { };

}

#endif
