#include "boost/utility/trace_scope.hpp"
#include "libs/utility/src/obj_id.cpp"
#include "libs/utility/src/object_tracked.cpp"
struct chk_dbl_die
: public utility::object_tracked
{
    ~chk_dbl_die(void)
    {
        if(!(am_i_live()))
        {
            mout()<<"~check_dbl_die:not live:this="<<this->id_get()<<"\n";
        }
    }
};

#include "boost/managed_ptr/marg_ostream_refcnt_overhead_ptr.hpp"
#include "boost/managed_ptr/prox_visitor_refcycle_abs.cpp"
#include "basis_adaptor.hpp"
#include <boost/mpl/assert_is_same.hpp>

// Hack for VC's lack of ADL
namespace boost
{

using mpl::_;

class object
: public chk_dbl_die
//***DIFF_POLICY_PTR : rm'ed dead member variable because
//  its purpose is now handled by object_tracked::our_members
//  and because basis_adaptor template requires Referent
//  to have default CTOR.
{
public:
    object(void) : dummy_(3.14159) { }
    virtual ~object(void) 
    { 
    }

    int foo(void) const { return 42; }
    double bar(void) const { return dummy_; }
private:
    double dummy_;
};

class child : public object
//***DIFF_POLICY_PTR : rm'ed dead member variable
//  and made CTOR default for same reasons as cited in
//  object.
{
public:
    child(int age=0) : age_(age) { }

    int age(void) const { return age_; }
    void age_put(int a_age) { age_=a_age; }
private:
    int age_;
};

template
  < template
    < typename //ReferentPtr
    >class OwnershipPolicy
  >
  std::string
owner_policy_name
  (void)
;
template
  <
  >
  std::string
owner_policy_name
  < ref_counted
  >
  (void)
{
    return std::string("<ref_counted>::");
}

template
  <
  >
  std::string
owner_policy_name
  < managed_ptr::curry_prox_visitor_refcycle_counted
    <managed_ptr::prox_visitor_refcycle_abs>
    ::owner
  >
  (void)
{
    return std::string("<refcycle_counted>::");
}

template
  < template
    < typename //ReferentPtr
    >class OwnershipPolicy
  >
struct owner_tests
{

    static
void copy_test(void)
{
    {
        typedef smart_ptr<object, OwnershipPolicy<_>, assert_check<_> > 
          sp_object_type;
        typedef managed_ptr::basis_adaptor<OwnershipPolicy<object*> > 
          basis_adaptor_object_type;
        basis_adaptor_object_type l_object_adaptor;
        typedef typename sp_object_type::basis_spec_type basis_spec_object_type;
        typedef typename basis_spec_object_type::basis_source_type src_object_type;
        src_object_type l_object_src(l_object_adaptor.as_basis_source());
        sp_object_type l_object_sp(l_object_src);
        typedef managed_ptr::basis_adaptor<OwnershipPolicy<child*> > 
          basis_adaptor_child_type;
        basis_adaptor_child_type l_child_adaptor;
        typedef basis_spec_object_type::rebind<child>::other::basis_source_type src_child_type;
        src_child_type l_child_src(l_child_adaptor.as_basis_source());
//        sp_object_type l_child2object_sp(l_child_src);
    }
}

};//end owner_tests<OwnershipPolicy> class
    
}//exit boost namespace

using namespace boost;

int main(void)
{
    owner_tests
      <managed_ptr::curry_prox_visitor_refcycle_counted
        <managed_ptr::prox_visitor_refcycle_abs
        >
        ::owner
      >::copy_test();
    return 0;
}
