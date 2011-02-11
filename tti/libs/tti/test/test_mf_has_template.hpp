#if !defined(TEST_MF_HAS_TEMPLATE_HPP)
#define TEST_MF_HAS_TEMPLATE_HPP

#include "TestStructs.hpp"
#include <boost/tti/TTIntrospection.hpp>

TTI_TRAIT_HAS_TEMPLATE(HaveMStr,MStrMemberTemplate)
TTI_HAS_TEMPLATE(TemplateNotExist)
TTI_HAS_TEMPLATE(ATPMemberTemplate)
TTI_TRAIT_HAS_TEMPLATE(HaveCL,CLMemberTemplate)
TTI_HAS_TEMPLATE(SimpleTMP)
TTI_TRAIT_HAS_TEMPLATE(AMT,AnotherMemberTemplate)
TTI_HAS_TEMPLATE(SomeMemberTemplate)

TTI_MTFC_TRAIT_HAS_TEMPLATE(MetaHaveMStr,MStrMemberTemplate)
TTI_MTFC_HAS_TEMPLATE(TemplateNotExist)
TTI_MTFC_HAS_TEMPLATE(ATPMemberTemplate)
TTI_MTFC_TRAIT_HAS_TEMPLATE(MFClassHaveCL,CLMemberTemplate)
TTI_MTFC_HAS_TEMPLATE(SimpleTMP)
TTI_MTFC_TRAIT_HAS_TEMPLATE(MFClassAMT,AnotherMemberTemplate)
TTI_MTFC_HAS_TEMPLATE(SomeMemberTemplate)

TTI_MEMBER_TYPE(AStructType)
TTI_TRAIT_MEMBER_TYPE(MT_BType,BType)

#endif // TEST_MF_HAS_TEMPLATE_HPP
