//simplest smart_ptr
#ifndef BOOST_MANAGED_PTR_TYPED_PTR_LJE20040327_HPP
#define BOOST_MANAGED_PTR_TYPED_PTR_LJE20040327_HPP
namespace boost{namespace managed_ptr{

  class 
void_ptr
{
 public:
        typedef
      void
    referent_type
    ;
    void_ptr(referent_type* a_ref=0)
    : my_ref(a_ref)
    {}
      referent_type*
    get(void)const
    {
        return my_ref;
    }
      void
    reset(referent_type* a_ref=0)
    {
        my_ref=a_ref;
    }
 private:
      referent_type*
    my_ref
    ;
};//end void_ptr class

template
  < typename Referent
  >  
  class 
typed_ptr
: private void_ptr
{
 public:
        typedef
      Referent
    referent_type
    ;
    
    typed_ptr(referent_type* a_ref=0)
    : void_ptr(a_ref)
    {}
    
      referent_type*
    get(void)const
    {
        return static_cast<referent_type*>(void_ptr::get());
    }
    
      void
    reset(referent_type* a_ref=0)
    {
        void_ptr::reset(a_ref);
    }
};//end typed_ptr class

}}//exit boost::managed_ptr namespace
#endif
