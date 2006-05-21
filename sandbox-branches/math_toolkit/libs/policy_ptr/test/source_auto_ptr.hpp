#ifndef LIBS_POLICY_PTR_TEST_SOURCE_AUTO_PTR_HPP
#define LIBS_POLICY_PTR_TEST_SOURCE_AUTO_PTR_HPP
#include <memory> //std::auto_ptr

namespace boost
{

  template<class Referent>
  struct
source_auto_ptr
/**@brief
 *  auto_ptr which creates its pointee
 */
  : public std::auto_ptr<Referent>
{
        typedef
      std::auto_ptr<Referent>
    super_type
    ;
    source_auto_ptr(void)
      : super_type(new Referent)
    {}
};

}//exit boost

#endif
