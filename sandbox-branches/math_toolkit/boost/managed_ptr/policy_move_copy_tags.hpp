//policy, move, copy tags
#ifndef BOOST_MANAGED_PTR_POLICY_POLICY_MOVE_COPY_TAGS_HPP_LJE20031123
#define BOOST_MANAGED_PTR_POLICY_POLICY_MOVE_COPY_TAGS_HPP_LJE20031123
namespace boost
{

//namespace managed_ptr{
//////////////////////////////////////////////////////////////////////////////
// Ownership categories that each OwnershipPolicy must declare
//
// The were actually those from the
//   smart_ptr template class 
// as they appeared around Oct 22 19:05 in:
//   http://cvs.sourceforge.net/viewcvs.py/boost-sandbox/boost-sandbox/
//////////////////////////////////////////////////////////////////////////////

    class copy_semantics_tag { };
    class move_semantics_tag { };
    class no_copy_semantics_tag { };

    class storage_policy_tag { };
    class ownership_policy_tag { };
    class conversion_policy_tag { };
    class checking_policy_tag { };
    
//}//exit managed_ptr namespace
}//exit boost namespace
#endif
