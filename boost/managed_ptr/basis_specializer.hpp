#ifndef BOOST_MANAGED_PTR_BASIS_SPECIALIZER_HPP_LJE200311120
#define BOOST_MANAGED_PTR_BASIS_SPECIALIZER_HPP_LJE200311120
namespace boost
{
  namespace managed_ptr
  {
  

    template
      < typename Referent
      , class Specializer
      >
      struct
    basis_specializer
    //Purpose:
    //  Provide a specializable class that defines the "basis type" to
    //  smart pointer class.  The basis_type is the argument to the
    //  smart_ptr CTOR which forms the "basis" of all other instances
    //  of the smart_ptr. IOW, all instances of smart_ptr are
    //  constructed either from the bais_type, or other smart_ptr's.
    //  It's somewhat like the basis step in of proof by induction.
    {
        typedef void basis_source_type;
    };
    
    template
      < typename Referent
      >
      struct
    basis_specializer
      < Referent
      , void
      >
    {
            typedef
          void
        nesting_type
        ;
            typedef
          Referent*
        basis_source_type
        ;
            typedef
          basis_source_type
        basis_sink_type
        ;
        template
          < typename Other
          >
          struct
        rebind
        {
                typedef
              basis_specializer<Other,nesting_type>
            other
            ;
        };
    };
    
  } //exit managed_ptr namespace
} //exit boost namespace
#endif
