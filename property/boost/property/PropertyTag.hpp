#pragma once

namespace properties
{

  struct ReadTag { };
  struct WriteTag { };
  struct ReadWriteTag : public ReadTag, public WriteTag { };
  template <class T> class DefaultPolicyTag { };

}
